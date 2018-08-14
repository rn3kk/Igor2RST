#include "RadioDevice.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QMutex>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

RadioDevice::RadioDevice(QObject *parent) :
    QObject(parent), m_port(nullptr), m_mutex(new QMutex)
{
}

void RadioDevice::read(QByteArrayList &chData, QByteArray &tone2,
                       QByteArray &tone5)
{
  if(m_port == nullptr)
  {
    QMessageBox::critical(nullptr, tr("Error"), tr("Device is not connected"));
    return;
  }
  qDebug() << "-------------------------------read----------------------------------------\n\t"
              "----------------------------------------------------------------------------------------";
  chData.clear();
  for(int i=1; i<=208; i++)
    chData << readMemory(i);
  QByteArray registry = readRegistry();
  if(registry.isEmpty())
  {
    qCritical() << "registry not read";
  }
  else
  {
                   tone2.clear();
                   tone5.clear();
                   tone5.append(registry.mid(0, 2));
                   tone5.append(registry.mid(12, 2));
                   tone5.append(registry.mid(16, 2));
                   tone5.append(registry.mid(26, 8));
                   tone5.append(registry.mid(38));
                   tone2.append(registry.mid(2, 10));
                   tone2.append(registry.mid(14, 2));
                   tone2.append(registry.mid(18, 8));
                   tone2.append(registry.mid(34, 4));
  }
  QByteArray debugMsg;
  for(const QByteArray &i : chData)
    debugMsg += "\"" + i.toHex() + "\" ";
  debugMsg += "}\ntone2 " + tone2.toHex();
  debugMsg += "\ntone5 " + tone5.toHex();
  qDebug() << debugMsg.data();
  qDebug() << "-------------------------------------------------------------------------\n\t"
              "----------------------------------------------------------------------------------------";
}

void RadioDevice::write(const QByteArrayList &chData,
                        const QByteArray &tone2,
                        const QByteArray &tone5)
{
  if(m_port == nullptr)
  {
    QMessageBox::critical(nullptr, tr("Error"), tr("Device is not connected"));
    return;
  }
    qDebug() << "---------------------------------write-----------------------------\n\t"
                "----------------------------------------------------------------------------------------";
  QByteArray debugMsg;
  debugMsg += "\nmemory {";
  for(const QByteArray &i : chData)
    debugMsg += "\"" + i.toHex() + "\" ";
  debugMsg += "}\ntone2 " + tone2.toHex();
  debugMsg += "\ntone5 " + tone5.toHex();
  qDebug() << debugMsg.data();
  for(const QByteArray &i : chData)
    writeMemory(i);
  QByteArray registry;
  registry.append(tone5.mid(0, 2));
  registry.append(tone2.mid(0,10));
  registry.append(tone5.mid(2, 2));
  registry.append(tone2.mid(10, 2));
  registry.append(tone5.mid(4, 2));
  registry.append(tone2.mid(12, 8));
  registry.append(tone5.mid(6, 8));
  registry.append(tone2.mid(20));
  registry.append(tone5.mid(14));
  writeRegistry(registry);
  qDebug() << "-------------------------------------------------------------------------\n\t"
              "----------------------------------------------------------------------------------------";
}

quint16 crc(const QByteArray &data, int numByte)
{
  quint16 w0 = 0xffff, w1, w2, w3, w4 = 0;
  do
  {
    w1 = data[w4++];
    w2 = 0x00ff;
    w1 &= w2;
    w0 ^= w1;
    w3 = 0x0009;
    while(--w3)
    {
      bool c = w0 & 0x0001;
      w0 = (w0 >> 1) & 0x7fff;
      if(c)
      {
        w2 = 0x8005;
        w0 ^= w2;
      }
    }
  }
  while(--numByte);
  w0 = ((w0 << 8) & 0xff00) | ((w0 >> 8) & 0x00ff);
  return w0;
}
//b4ac  4ef3
void RadioDevice::reconnect()
{/*
  const char dataPtr[] = {
//    '\x21', '\x11', '\x00', '\x0a', '\x41', '\x52',
//                     '\x2d', '\x34', '\x33', '\x76', '\x32', '\x2e',
//                     '\x30', '\x35', '\xed', '\xe2'

    '\x21', '\x09', '\x01', '\xBF', '\x00', '\x14', '\x00', '\x24', '\x20',
    '\xA8', '\x20', '\xA8', '\x02', '\xED', '\x02', '\xED', '\x00', '\x00',
    '\x53', '\x4B', '\x4F', '\x52', '\x41', '\x4A', '\x41', '\x00', '\x9A', '\xB0'
  };
 // QByteArray data(dataPtr, 28);
  QByteArray data = QByteArray::fromHex("210c00000005000a000103a7032602a40221");
  quint16 result = crc(data, data.size());*/
  if(!m_mutex->tryLock(1))
    return;
  if(m_port != nullptr)
  {
    m_port->close();
    delete m_port;
    m_port = nullptr;
  }
  QElapsedTimer timer;
  for(const QSerialPortInfo &i : QSerialPortInfo::availablePorts())
  {
    qDebug() << "probe port " << i.portName();
    m_port = new QSerialPort(i);
    m_port->setBaudRate(QSerialPort::Baud115200);
    m_port->setDataBits(QSerialPort::Data8);
    m_port->setParity(QSerialPort::NoParity);
    m_port->setStopBits(QSerialPort::OneStop);
    if(!m_port->open(QIODevice::ReadWrite))
    {
      delete m_port;
      m_port = nullptr;
      continue;
    }
    m_port->waitForReadyRead(1000);
    timer.start();
    do
    {
      QThread::currentThread()->msleep(5);
      qApp->processEvents(QEventLoop::AllEvents, 10);
    }
    while(!timer.hasExpired(5000));
    QByteArray data = m_port->readAll();
    qDebug() << "recive data " << data.toHex();
    if(!data.isEmpty())
    {
        int start = data.indexOf("\x11\0") - 1;
        data = data.mid(start, 16);
        qDebug() << "packet " << data.toHex();
        if(isValidPackage(data))
        {
            QString name = data.mid(4, 5);
            QString version = data.mid(9, 5);
            m_adress = data[0];
            emit connected(name, version);
            readMemory(1);
            m_mutex->unlock();
            QMessageBox::information(nullptr, tr("Connected"), tr("Connect to device is sucefull"));
            return;
        }
    }
    m_port->close();
    delete m_port;
    m_port = nullptr;
  }
  qDebug("exit from connect");
  m_mutex->unlock();
  QMessageBox::critical(nullptr, tr("Error connect"), tr("Radio device not found"));
}

bool RadioDevice::isValidPackage(const QByteArray &packet)
{
  int size = packet.size();
  QByteArray crcPacket = packet.right(2);
  qint16 checksumPacket = ((packet[size - 2] & 0xffff) << 8) |
      (packet[size - 1] & 0x00ff);
  qint16 result = crc(packet, size - 2);
  qDebug() << "packet crc " << QString::number(checksumPacket, 16)
           << " calculate crc " << QString::number(result, 16);
  return checksumPacket == result;
}

QByteArray RadioDevice::readMemory(int index)
{
  QByteArray request;
  QDataStream stream(&request, QIODevice::WriteOnly);
  stream << m_adress;
  quint8 command = 0x09;
  stream << command;
  quint8 byte;
  quint8 num;
  if(index > 199)
  {
    byte = 0x0c;
    num = index - 199;
  }
  else
  {
    byte = 0x01;
    num = index;
  }
  stream << byte << num;
  stream << crc(request, 4);
//  quint16 checksum = crc(request, 4);
//  request.append(checksum >> 8);
//  request.append(checksum & 0x00ff);
  QElapsedTimer allTimer;
  allTimer.start();
  while(!allTimer.hasExpired(20000))
  {
    const int responseLen = 28;
    qDebug() << "write to radio " << request.toHex();
    m_port->write(request);
    QElapsedTimer timer;
    timer.start();
    QByteArray response;
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
      response.append(m_port->readAll());
      response = response.mid(response.indexOf(command) - 1, responseLen);
    }
    while(response.size() < responseLen && !timer.hasExpired(4000));
    qDebug() << "read " << response.toHex();
//    while()
//    {
//      qApp->processEvents(QEventLoop::AllEvents, 1);
//      QThread::currentThread()->msleep(1);
//    }
//    response = response.left(28);
//    qDebug() << "packet " << response.toHex();
    qDebug() << "recive data " << response.toHex();
    if(response.size() >= responseLen && isValidPackage(response))
    {
      qDebug() << "recive package ";
      return response.mid(6, 20);
    }
    else
      qDebug("not find valid response read memory to read");
  }
  qCritical() << "timeout for " << __func__;
  return QByteArray();
}

QByteArray RadioDevice::readRegistry()
{
  const int responseLen = 1320;
  QByteArray request;
  QDataStream stream(&request, QIODevice::WriteOnly);
  stream << m_adress;
  quint8 command = 0x0b;
  stream << command;
  quint32 dat = 0x00000291;
  stream << dat;
  quint16 checksum = crc(request, 6);
  stream << checksum;
//  request.append(checksum >> 8);
//  request.append(checksum & 0x00ff);
  QElapsedTimer allTimer;
  allTimer.start();
  while(!allTimer.hasExpired(20000))
  {
    qDebug() << "write to radio " << request.toHex();
    m_port->write(request);
    QElapsedTimer timer;
    QByteArray response;
    timer.start();
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
      response += m_port->readAll();
      response = response.mid(response.indexOf(command) - 1, responseLen);
    }
    while(response.size() < responseLen && !timer.hasExpired(5000));
    qDebug() << "recive data " << response.toHex();
    if(response.size() >= responseLen && isValidPackage(response))
    {
      qDebug() << "recive package ";
      return response.mid(4, 1242);
    }
    else
      qCritical("read registry recive error");
  }
  qCritical() << "timeout for " << __func__;
  return QByteArray();
}

void RadioDevice::writeMemory(const QByteArray &data)
{
  const int responseLen = 8;
  QByteArray request;
  QDataStream stream(&request, QIODevice::WriteOnly);
  stream << m_adress;
  quint8 command = 0x0a;
  stream << command;
  request.append(data);
  quint16 checksum = crc(request, 26);
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  QElapsedTimer allTimer;
  allTimer.start();
  while(!allTimer.hasExpired(20000))
  {
    qDebug() << "write memory to radio " << request.toHex();
    m_port->write(request);
    QElapsedTimer timer;
    QByteArray response;
    timer.start();
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
      response += m_port->readAll();
      response = response.mid(response.indexOf(command) - 1, responseLen);
    }
    while(response.size() < responseLen && !timer.hasExpired(4000));
    qDebug() << "recive data " << response.toHex();
    if(response.size() >= responseLen && isValidPackage(response))
    {
      qDebug() << "recive valid answer ";
      return;
    }
    else
      qCritical("write memory recive error");
  }
  qCritical() << "timeout for " << __func__;
}

void RadioDevice::writeRegistry(const QByteArray &data)
{
  const int responseLen = 8;
  QByteArray request;
  QDataStream stream(&request, QIODevice::WriteOnly);
  stream << m_adress;
  quint8 command = 0x0c;
  stream << command;
  quint16 dat = 0;
  stream << dat;
  dat = data.size() / 2;
  stream << dat;
  dat = data.size();
  stream << dat;
  request.append(data);//.left(10));
  quint16 checksum = crc(request, request.size());
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  QElapsedTimer allTimer;
  allTimer.start();
  while(!allTimer.hasExpired(20000))
  {
    qDebug() << "write to radio " << request.toHex();
    m_port->write(request);
    QElapsedTimer timer;
    QByteArray response;
    timer.start();
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
      response += m_port->readAll();
      response = response.mid(response.indexOf(command) - 1, responseLen);
    }
    while(response.size() < responseLen && !timer.hasExpired(5000));
    qDebug() << "recive data " << response.toHex();
    if(response.size() >= responseLen && isValidPackage(response))
    {
      qDebug("recive answer");
      return;
    }
    else
      qCritical("write registry recive error");
  }
  qCritical() << "timeout for " << __func__;
}

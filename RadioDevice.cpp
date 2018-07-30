#include "RadioDevice.h"

#include <QApplication>
#include <QDeadLineTimer>
#include <QElapsedTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

RadioDevice::RadioDevice(QObject *parent) :
    QObject(parent), m_port(nullptr)
{
}

void RadioDevice::read(QByteArrayList &chData, QByteArray &tone2,
                       QByteArray &tone5)
{
  chData.clear();
  for(int i=1; i<=208; i++)
    chData << readMemory(i);
  QByteArray registry = readRegistry();
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

void RadioDevice::write(const QByteArrayList &chData,
                        const QByteArray &tone2,
                        const QByteArray &tone5)
{
  for(int i=0; i<208;)
    writeMemory(chData[i], ++i);
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
      w0 >>= 1;
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
  QByteArray data(dataPtr, 28);
  quint16 result = crc(data, 26);*/
  if(m_port != nullptr)
  {
    m_port->close();
    delete m_port;
    m_port = nullptr;
  }
  QElapsedTimer timer;
  for(const QSerialPortInfo &i : QSerialPortInfo::availablePorts())
  {
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
    while(!timer.hasExpired(1000));
    QByteArray data = m_port->readAll();
    int start = data.indexOf("\x11\0") - 1;
    data = data.mid(start, 16);
    if(isValidPackage(data))
    {
      QString name = data.mid(4, 5);
      QString version = data.mid(9, 5);
      m_adress = data[0];
      emit connected(name, version);
      readMemory(1);
      break;
    }
    m_port->close();
    delete m_port;
    m_port = nullptr;
  }
}

bool RadioDevice::isValidPackage(const QByteArray &packet)
{
  int size = packet.size();
  qint16 checksumPacket = ((packet[size - 2] & 0xffff) << 8) |
      (packet[size - 1] & 0xffff);
  return checksumPacket == crc(packet, size - 2);
}

QByteArray RadioDevice::readMemory(int index)
{
  QByteArray request = "r\x09";
  if(index > 199)
  {
    request.append('\x0c');
    quint8 num = index - 199;
    request.append(num);
  }
  else
  {
    request.append('\x01');
    quint8 num = index;
    request.append(num);
  }
  quint16 checksum = crc(request, 4);
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  while(true)
  {
    m_port->write(request);
    QDeadlineTimer timer(10);
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
    }
    while(!timer.hasExpired());
    QByteArray response = m_port->readAll();
    if(response.size() >= 28 && isValidPackage(response))
      return response.mid(6, 20);
  }
}

QByteArray RadioDevice::readRegistry()
{
  QByteArray request("r\x0b\0\0\x02\x6d", 6);
  quint16 checksum = crc(request, 6);
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  while(true)
  {
    m_port->write(request);
    QDeadlineTimer timer(10);
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
    }
    while(!timer.hasExpired());
    QByteArray response = m_port->readAll();
    if(response.size() >= 28 && isValidPackage(response))
      return response.mid(4, 1242);
  }
}

void RadioDevice::writeMemory(const QByteArray &data, quint8 index)
{
  QByteArray request = "r\x0a";
  request.append(data);
  quint16 checksum = crc(request, 26);
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  while(true)
  {
    m_port->write(request);
    QDeadlineTimer timer(10);
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
    }
    while(!timer.hasExpired());
    QByteArray response = m_port->readAll();
    if(isValidPackage(response))
      return;
  }
}

void RadioDevice::writeRegistry(const QByteArray &data)
{
  QByteArray request("r\x0c\0\x00\x02\x6d", 6);
  request.append(data);
  quint16 checksum = crc(request, 26);
  request.append(checksum >> 8);
  request.append(checksum & 0x00ff);
  while(true)
  {
    m_port->write(request);
    QDeadlineTimer timer(5000);
    m_port->waitForReadyRead(10);
    do
    {
      qApp->processEvents(QEventLoop::AllEvents, 1);
      QThread::currentThread()->msleep(1);
    }
    while(!timer.hasExpired());
    QByteArray response = m_port->readAll();
    if(isValidPackage(response))
      return;
  }
}

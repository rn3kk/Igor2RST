#include "RadioDevice.h"

#include <QApplication>
#include <QElapsedTimer>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThread>

RadioDevice::RadioDevice(QObject *parent) :
    QObject(parent), m_port(nullptr)
{
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
{
  const char dataPtr[] = {
//    '\x21', '\x11', '\x00', '\x0a', '\x41', '\x52',
//                     '\x2d', '\x34', '\x33', '\x76', '\x32', '\x2e',
//                     '\x30', '\x35', '\xed', '\xe2'

    '\x21', '\x09', '\x01', '\xBF', '\x00', '\x14', '\x00', '\x24', '\x20',
    '\xA8', '\x20', '\xA8', '\x02', '\xED', '\x02', '\xED', '\x00', '\x00',
    '\x53', '\x4B', '\x4F', '\x52', '\x41', '\x4A', '\x41', '\x00', '\x9A', '\xB0'
  };
  QByteArray data(dataPtr, 28);
  quint16 result = crc(data, 26);
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
    int start = data.indexOf('\x11') - 1;
    data = data.mid(start, 16);
  }
}

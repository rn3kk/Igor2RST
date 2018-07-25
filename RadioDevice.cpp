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

void RadioDevice::reconnect()
{
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

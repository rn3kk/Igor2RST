#ifndef RADIODEVICE_H
#define RADIODEVICE_H

#include <QObject>

class RadioDevice : public QObject
{
  Q_OBJECT
public:
  explicit RadioDevice(QObject *parent = nullptr);
  QByteArray read();
  void write(const QByteArray &chData, const QByteArray &tone2,
             const QByteArray &tone5);

signals:
  void connected(QString name, QString version);

public slots:
  void reconnect();

private:
  class QSerialPort *m_port;
};

#endif // RADIODEVICE_H

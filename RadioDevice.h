#ifndef RADIODEVICE_H
#define RADIODEVICE_H

#include <QObject>

class RadioDevice : public QObject
{
  Q_OBJECT
public:
  explicit RadioDevice(QObject *parent = nullptr);
  void read(QByteArrayList &chData, QByteArray &tone2,
            QByteArray &tone5);
  void write(const QByteArrayList &chData, const QByteArray &tone2,
             const QByteArray &tone5);

signals:
  void connected(QString name, QString version);

public slots:
  void reconnect();

private:
  class QSerialPort *m_port;
  quint8 m_adress;

  bool isValidPackage(const QByteArray &packet);
  QByteArray readMemory(int index);
  QByteArray readRegistry();
  void writeMemory(const QByteArray &data, quint8 index);
  void writeRegistry(const QByteArray &data);
};

#endif // RADIODEVICE_H

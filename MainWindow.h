#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);
  ~MainWindow();

public slots:
  void read();
  void write();
  void radioConnect(const QString &name,
                    const QString &version);

private:
  class MemoryTableModel *m_memory;
  QLabel *m_name;
  QLabel *m_version;
  class RadioDevice *m_device;
};

#endif // MAINWINDOW_H

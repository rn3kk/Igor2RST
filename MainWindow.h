#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow(QWidget *parent = 0);

public slots:
  void read();
  void write();
  void radioConnect(const QString &name,
                    const QString &version);
  void importData();
  void exportData();

private:
  class Tab2Tone *m_tone2;
  class Tab5Tone *m_tone5;
  class MemoryTableModel *m_memory;
  QLabel *m_name;
  QLabel *m_version;
  QLabel *m_busy;
  class RadioDevice *m_device;
};

#endif // MAINWINDOW_H

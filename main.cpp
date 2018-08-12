#include "MainWindow.h"
#include "Log.h"

#include <QApplication>


int main(int argc, char *argv[])
{
  qInstallMessageHandler(writeLog);
  QApplication a(argc, argv);
  MainWindow w;
  w.show();

  return a.exec();
}

#include "Log.h"

#include <QDateTime>
#include <QFile>
#include <QMutex>


QMutex mutex;

void writeLog(QtMsgType type, const QMessageLogContext &context,
              const QString &msg)
{
  QString str = QDateTime::currentDateTime().
      toString("dd.MM.yyyy hh:mm:ss.zzz ");
  switch (type) {
  case QtDebugMsg:
    str += "[DEBUG  ]";
    break;
  case QtInfoMsg:
    str += "[INFO   ]";
    break;
  case QtWarningMsg:
    return;
    str += "[WARNING]";
    break;
  case QtCriticalMsg:
    str += "[ERROR  ]";
    break;
  case QtFatalMsg:
    str += "[FATAL  ]";
    break;
  }
  str += " " + msg + "\n";
  QFile file("UserTRX.log");
  if(!file.open(QFile::Append | QFile::Text))
  {
    QString error = "log file open error ";
    error += file.errorString();
    qFatal(error.toStdString().c_str());
  }
  mutex.lock();
  file.write(str.toUtf8());
  file.close();
  mutex.unlock();
}

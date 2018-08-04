#ifndef LOG_H
#define LOG_H

#include <QString>


void writeLog(QtMsgType type,
              const QMessageLogContext &context,
              const QString &msg);

#endif // LOG_H

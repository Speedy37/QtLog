#include "qtlog.h"
#include <iostream>

QtLog * QtLog::m_instance = nullptr;
QtLog::LogLevel QtLog::m_level = QtLog::Info;
QtLog::LogOutput QtLog::m_output = QtLog::StandardError;

QtLog::QtLog(QObject * parent) : QObject(parent)
{

}

QString QtLog::type(int t)
{
    switch(t)
    {
    case Fatal:
        return QStringLiteral("FATAL");
    case Error:
        return QStringLiteral("ERROR");
    case Warn:
        return QStringLiteral("WARN");
    case Info:
        return QStringLiteral("INFO");
    case Debug:
        return QStringLiteral("DEBUG");
    case Trace:
        return QStringLiteral("TRACE");
    }

    return QString();
}

void QtLog::slog(QtLog::LogLevel type, const QString &msg)
{
    if(level() >= type)
    {
        instance()->log(type, msg);
    }
}

void QtLog::log(LogLevel type, const QString &msg)
{
    QMutexLocker lock(&mutex);
    switch(output())
    {
    case StandardOutput:
        std::cout << msg.toStdString() << std::endl;
        break;
    case StandardError:
        std::cerr << msg.toStdString() << std::endl;
        break;
    default:
        break;
    }
    emit logged(type, msg);
}

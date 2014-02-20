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

#include <QRegExp>
void QtLog::log(QtLog::LogLevel type, const QString &msg, const char *function)
{
    if(level() >= type)
    {
        QString functionStr(function);
        int end = functionStr.indexOf('(');
        if(end > 0)
        {
            int start = functionStr.lastIndexOf(QRegExp("[^A-Za-z0-9_:]"), end - 1) + 1;
            functionStr = functionStr.mid(start, end - start);
        }
        instance()->log(type, QStringLiteral("%1 : %2").arg(functionStr).arg(msg));
    }
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
        std::cout << this->type(type).leftJustified(6).toStdString() << " : " << msg.toStdString() << std::endl;
        break;
    case StandardError:
        std::cerr << this->type(type).leftJustified(6).toStdString() << " : " << msg.toStdString() << std::endl;
        break;
    default:
        break;
    }
    emit logged(type, msg);
}

#ifndef QTLOG_H
#define QTLOG_H

#include "qtlog_global.h"
#include <QMutex>
#include <QObject>

#define LOG(Type, QStringMessage) QtLog::log(QtLog::Type, QStringMessage, Q_FUNC_INFO);

#ifndef NOLOGERROR
#define LOG_ERROR(QStringMessage) LOG(Error, QStringMessage);
#else
#define qt_noop();
#endif

#ifndef NOLOGWARN
#define LOG_WARN(QStringMessage) LOG(Warn, QStringMessage);
#else
#define qt_noop();
#endif

#ifndef NOLOGINFO
#define LOG_INFO(QStringMessage) LOG(Info, QStringMessage);
#else
#define qt_noop();
#endif

#ifndef NOLOGDEBUG
#define LOG_DEBUG(QStringMessage) LOG(Debug, QStringMessage);
#else
#define qt_noop();
#endif

#ifndef NOLOGTRACE
#define LOG_TRACE(QStringMessage) LOG(Trace, QStringMessage);
#else
#define qt_noop();
#endif

class QTLOGSHARED_EXPORT QtLog : public QObject
{
    Q_OBJECT
public:
    static QtLog * instance(QObject * parent = 0)
    {
        if(m_instance == nullptr)
            m_instance = new QtLog(parent);
        return m_instance;
    }

    enum LogOutput
    {
        NoOutput,
        StandardOutput,
        StandardError
    };

    enum LogLevel
    {
        Off,
        Fatal,
        Error,
        Warn,
        Info,
        Debug,
        Trace
    };

    static QString type(int t);
    static void fatal(const QString & msg) { slog(Fatal, msg); }
    static void error(const QString & msg) { slog(Error, msg); }
    static void warn(const QString & msg) { slog(Warn, msg); }
    static void info(const QString & msg) { slog(Info, msg); }
    static void debug(const QString & msg) { slog(Debug, msg); }
    static void trace(const QString & msg) { slog(Trace, msg); }
    static void log(LogLevel type, const QString & msg, const char * function);

    static bool isFatalEnabled() { return m_level >= Fatal; }
    static bool isWarnEnabled() { return m_level >= Error; }
    static bool isErrorEnabled() { return m_level >= Warn; }
    static bool isInfoEnabled() { return m_level >= Info; }
    static bool isDebugEnabled() { return m_level >= Debug; }
    static bool isTraceEnabled() { return m_level >= Trace; }

    template<typename Func>
    static void connect(Func handler)
    {
        QObject::connect(instance(), &QtLog::logged, handler);
    }

    static LogLevel level() { return m_level; }
    static void setLevel(const LogLevel &level) { m_level = level; }

    static LogOutput output() { return m_output; }
    static void setOutput(const LogOutput &output){ m_output = output; }

signals:
    void logged(int type, const QString & msg);

private:
    QMutex mutex;
    static void slog(LogLevel type, const QString & msg);
    void log(LogLevel type, const QString & msg);
    static QtLog * m_instance;
    static LogLevel m_level;
    static LogOutput m_output;

    QtLog(QObject * parent = 0);
};

#endif // QTLOG_H

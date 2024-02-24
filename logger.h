#ifndef LOGGER_H
#define LOGGER_H

#include <QTextBrowser>
#include <QDateTime>

class Logger
{
public:
    enum LogType {
        Info,
        Warn,
        Error
    };

public:
    static Logger& instance();
    void setOutputWidget(QTextBrowser *textBrowser);
    void addLogImpl(const QString &message, const QString &color, const QString &typeString);

    void info(const QString &message);
    void warn(const QString &message);
    void error(const QString &message);

private:
    QTextBrowser *m_textBrowser;
    Logger();
    ~Logger() {}
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;
};

#endif // LOGGER_H

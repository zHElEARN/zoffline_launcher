#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QTextBrowser>
#include <QDateTime>

class LogManager
{
public:
    enum LogType {
        Info,
        Warn,
        Error
    };

public:
    static LogManager& instance();
    void setOutputWidget(QTextBrowser *textBrowser);
    void addLogImpl(const QString &message, const QString &color, const QString &typeString);

    void info(const QString &message);
    void warn(const QString &message);
    void error(const QString &message);

private:
    QTextBrowser *m_textBrowser;
    LogManager();
    ~LogManager() {}
    LogManager(const LogManager &) = delete;
    LogManager &operator=(const LogManager &) = delete;
};

#endif // LOGMANAGER_H

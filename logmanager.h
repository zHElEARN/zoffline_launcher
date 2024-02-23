#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <QTextBrowser>

class LogManager
{
public:
    static LogManager& instance();
    void setOutputWidget(QTextBrowser *textBrowser);
    void addLog(const QString &message);

private:
    QTextBrowser *m_textBrowser;
    LogManager();
    ~LogManager() {}
    LogManager(const LogManager &) = delete;
    LogManager &operator=(const LogManager &) = delete;
};

#endif // LOGMANAGER_H

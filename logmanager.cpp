#include "logmanager.h"

LogManager::LogManager() : m_textBrowser(nullptr) {}

LogManager& LogManager::instance()
{
    static LogManager instance;
    return instance;
}

void LogManager::setOutputWidget(QTextBrowser *textBrowser)
{
    m_textBrowser = textBrowser;
}

void LogManager::addLogImpl(const QString &message, const QString &color, const QString &typeString)
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString formattedMessage = QString("<font color=\"%1\">[%2] %3:</font> %4")
        .arg(color, currentTime, typeString, message);

    if (m_textBrowser)
    {
        m_textBrowser->append(formattedMessage);
    }

    QString consoleMessage = QString("[%1] %2: %3")
        .arg(currentTime, typeString, message);

    qDebug().noquote() << consoleMessage;
}

void LogManager::info(const QString &message)
{
    addLogImpl(message, "black", "INFO");
}

void LogManager::warn(const QString &message)
{
    addLogImpl(message, "orange", "WARN");
}

void LogManager::error(const QString &message)
{
    addLogImpl(message, "red", "ERROR");
}

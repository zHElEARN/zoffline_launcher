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

void LogManager::addLog(const QString &message)
{
    if (m_textBrowser)
    {
        m_textBrowser->append(message);
    }
}

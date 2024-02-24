#include "logger.h"

Logger::Logger() : m_textBrowser(nullptr) {}

Logger& Logger::instance()
{
    static Logger instance;
    return instance;
}

void Logger::setOutputWidget(QTextBrowser *textBrowser)
{
    m_textBrowser = textBrowser;
}

void Logger::addLogImpl(const QString &message, const QString &color, const QString &typeString)
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

void Logger::info(const QString &message)
{
    addLogImpl(message, "black", "INFO");
}

void Logger::warn(const QString &message)
{
    addLogImpl(message, "orange", "WARN");
}

void Logger::error(const QString &message)
{
    addLogImpl(message, "red", "ERROR");
}

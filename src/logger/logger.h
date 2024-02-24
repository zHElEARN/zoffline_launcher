#ifndef LOGGER_H
#define LOGGER_H

#include <QTextBrowser>
#include <QDateTime>
#include <QDebug>
#include <tuple>

class Logger {
public:
    enum LogType {
        Info,
        Warn,
        Error
    };

    static Logger& instance();
    void setOutputWidget(QTextBrowser *textBrowser);

    template<typename... Args>
    void info(const Args&... args);

    template<typename... Args>
    void warn(const Args&... args);

    template<typename... Args>
    void error(const Args&... args);

private:
    QTextBrowser *m_textBrowser;

    Logger();
    ~Logger() {}
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    template<typename... Args, std::size_t... Is>
    QString concatArgs(const std::tuple<Args...>& tuple, std::index_sequence<Is...>);

    template<typename... Args>
    QString concatArgs(const std::tuple<Args...>& tuple);

    template<typename... Args>
    void addLogImpl(const QString &typeString, const QString &color, const Args&... args);
};

#include "logger.tpp"

#endif // LOGGER_H

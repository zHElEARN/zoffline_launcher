template<typename... Args, std::size_t... Is>
QString Logger::concatArgs(const std::tuple<Args...>& tuple, std::index_sequence<Is...>) {
    return (QString() + ... + (Is == sizeof...(Args) - 1 ? QString(std::get<Is>(tuple)) : QString(std::get<Is>(tuple)) + " "));
}

template<typename... Args>
QString Logger::concatArgs(const std::tuple<Args...>& tuple) {
    return concatArgs(tuple, std::make_index_sequence<sizeof...(Args)>());
}

template<typename... Args>
void Logger::addLogImpl(const QString &typeString, const QString &color, const Args&... args) {
    QString message = concatArgs(std::make_tuple(args...));
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString formattedMessage = QString("<font color=\"%1\">[%2] %3:</font> %4")
                                   .arg(color)
                                   .arg(currentTime)
                                   .arg(typeString)
                                   .arg(message);

    if (m_textBrowser) {
        m_textBrowser->append(formattedMessage);
    }

    QString consoleMessage = QString("[%1] %2: %3")
                                 .arg(currentTime)
                                 .arg(typeString)
                                 .arg(message);
    qDebug().noquote() << consoleMessage;
}

template<typename... Args>
void Logger::info(const Args&... args) {
    addLogImpl("INFO", "white", args...);
}

template<typename... Args>
void Logger::warn(const Args&... args) {
    addLogImpl("WARN", "orange", args...);
}

template<typename... Args>
void Logger::error(const Args&... args) {
    addLogImpl("ERROR", "red", args...);
}

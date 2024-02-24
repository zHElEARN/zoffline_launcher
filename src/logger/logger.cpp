#include "Logger.h"

Logger::Logger() : m_textBrowser(nullptr) {}

Logger& Logger::instance() {
    static Logger instance;
    return instance;
}

void Logger::setOutputWidget(QTextBrowser *textBrowser) {
    m_textBrowser = textBrowser;
}

#ifndef UTILS_H
#define UTILS_H

#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QEventLoop>

#include <functional>

class Utils
{
public:
    Utils();

public:
    static bool isZwiftInstalled();
    static void getLatestZofflineInfo(std::function<void(const QString&, const QString&)> callback);
    static void downloadFile(const QString& path, const QString& name, const QString& url, std::function<void()> callback);
};

#endif // UTILS_H

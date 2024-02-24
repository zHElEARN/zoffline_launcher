#ifndef UTILS_H
#define UTILS_H

#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDomDocument>
#include <QEventLoop>
#include <QRegularExpression>

#include <functional>

#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <psapi.h>

class Utils
{
public:
    Utils();

public:
    static QString getInstalledZwiftVersion(const QString& zwiftInstallFolderPath);
    static bool getZwiftInstallLocation(QString &installLocation);
    static void getLatestZofflineInfo(std::function<void(const QString&, const QString&)> callback);
    static QString parseZofflineVersion(const QString& zofflineFileName);
    static void downloadFile(const QString& path, const QString& name, const QString& url, std::function<void()> callback);
    static int compareVersion(const QString& version1, const QString& version2);
    static void terminate(const QString& zofflineName);
    static bool isZwiftProcessExists();
};

#endif // UTILS_H

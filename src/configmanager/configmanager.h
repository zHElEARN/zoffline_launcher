#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <QFile>

class ConfigManager
{
public:
    static ConfigManager& instance();

    void initialize(const QString& fileName, const QString& directory);

    void addServer(const QString& alias, const QString& address);
    void removeServer(const QString& alias);

    void setMirror(const QString& mirror);

    QJsonObject getServers();
    QString getServerAddress(const QString& alias);
    QString getMirror();

    void setServerAddress(const QString& alias, const QString& newAddress);

    void save();
    void load();

private:
    ConfigManager() {}
    ConfigManager(ConfigManager const&) = delete;
    void operator =(ConfigManager const&) = delete;

    QFile file;
    QJsonObject servers;
    QString mirror;
};

#endif // CONFIGMANAGER_H

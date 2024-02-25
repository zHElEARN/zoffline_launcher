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

    QJsonObject getServers();
    QString getServerAddress(const QString& alias);

    void setServerAddress(const QString& alias, const QString& newAddress);

    void save();
    void load();

private:
    ConfigManager() {}
    ConfigManager(ConfigManager const&) = delete;
    void operator =(ConfigManager const&) = delete;

    QFile file;
    QJsonObject jsonObject;
};

#endif // CONFIGMANAGER_H

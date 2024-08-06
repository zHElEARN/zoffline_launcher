#include "configmanager.h"

ConfigManager& ConfigManager::instance()
{
    static ConfigManager instance;
    return instance;
}

void ConfigManager::initialize(const QString &fileName, const QString &directory)
{
    file.setFileName(directory + QDir::separator() + fileName);
    load();
}

void ConfigManager::load()
{
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        servers = doc["servers"].toObject();
        mirror = doc["mirror"].toString();
        file.close();
    }
}

void ConfigManager::addServer(const QString &alias, const QString &address)
{
    servers[alias] = address;
}

void ConfigManager::removeServer(const QString &alias)
{
    servers.remove(alias);
}

void ConfigManager::setMirror(const QString &mirror)
{
    this->mirror = mirror;
}

QJsonObject ConfigManager::getServers()
{
    return servers;
}

QString ConfigManager::getServerAddress(const QString &alias)
{
    return servers[alias].toString();
}

QString ConfigManager::getMirror()
{
    return mirror.isEmpty() ? "ghproxy" : mirror;
}

void ConfigManager::setServerAddress(const QString &alias, const QString &newAddress)
{
    servers[alias] = newAddress;
}

void ConfigManager::save()
{
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonObject jsonObject;
        jsonObject.insert("servers", getServers());
        jsonObject.insert("mirror", getMirror());
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();
    }
}

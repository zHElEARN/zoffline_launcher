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
        jsonObject = doc.object();
        file.close();
    }
}

void ConfigManager::addServer(const QString &alias, const QString &address)
{
    jsonObject[alias] = address;
}

void ConfigManager::removeServer(const QString &alias)
{
    jsonObject.remove(alias);
}

QJsonObject ConfigManager::getServers()
{
    return jsonObject;
}

QString ConfigManager::getServerAddress(const QString &alias)
{
    return jsonObject[alias].toString();
}

void ConfigManager::setServerAddress(const QString &alias, const QString &newAddress)
{
    jsonObject[alias] = newAddress;
}

void ConfigManager::save()
{
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QJsonDocument doc(jsonObject);
        file.write(doc.toJson());
        file.close();
    }
}

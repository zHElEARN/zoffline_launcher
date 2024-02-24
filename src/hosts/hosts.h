#ifndef HOSTS_H
#define HOSTS_H

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QProcessEnvironment>
#include <QDir>
#include <QRegularExpression>

class Hosts
{
public:
    Hosts();
    void load();
    QStringList getHosts(const QString &ip);
    void addHosts(const QString &ip, const QStringList &hostnames);
    void removeHost(const QString &hostname);
    void save();
    void backup();
    void restore();

private:
    struct HostsEntry {
        bool isComment;
        QString ip;
        QStringList hostnames;
    };

    QList<HostsEntry> entries;
    static QRegularExpression whitespace;
    QString getHostsFilePath();
};

#endif // HOSTS_H

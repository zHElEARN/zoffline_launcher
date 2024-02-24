#include "hosts.h"

QRegularExpression Hosts::whitespace("\\s+");

Hosts::Hosts() {
    load();
}

void Hosts::load() {
    QString hostsFilePath = getHostsFilePath();
    QFile file(hostsFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    entries.clear();

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        HostsEntry entry;
        if (line.trimmed().startsWith("#")) {
            entry.isComment = true;
            entry.ip = line;
        } else {
            QStringList fields = line.split(whitespace, Qt::SkipEmptyParts);
            if (fields.size() >= 2) {
                entry.isComment = false;
                entry.ip = fields[0];
                for (int i = 1; i < fields.size(); ++i)
                    entry.hostnames.append(fields[i]);
            }
        }
        entries.append(entry);
    }
}

QStringList Hosts::getHosts(const QString &ip) {
    QStringList hostnames;
    for (const HostsEntry &entry : entries) {
        if (!entry.isComment && entry.ip == ip)
            hostnames.append(entry.hostnames);
    }
    return hostnames;
}

void Hosts::addHosts(const QString &ip, const QStringList &hostnames) {
    HostsEntry entry;
    entry.isComment = false;
    entry.ip = ip;
    entry.hostnames = hostnames;
    entries.append(entry);
}

void Hosts::removeHost(const QString &hostname) {
    for (HostsEntry &entry : entries) {
        if (!entry.isComment)
            entry.hostnames.removeAll(hostname);
    }
}

void Hosts::save() {
    QString hostsFilePath = getHostsFilePath();
    QFile file(hostsFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    for (const HostsEntry &entry : entries) {
        if (entry.isComment) {
            out << entry.ip << "\n";
        } else {
            out << entry.ip;
            for (const QString &hostname : entry.hostnames)
                out << " " << hostname;
            out << "\n";
        }
    }
}

void Hosts::backup() {
    QString hostsFilePath = getHostsFilePath();
    QFile::copy(hostsFilePath, hostsFilePath + ".bak");
}

void Hosts::restore() {
    QString hostsFilePath = getHostsFilePath();
    QFile::remove(hostsFilePath);
    QFile::copy(hostsFilePath + ".bak", hostsFilePath);

    // Reload the hosts data from the restored file
    load();
}

QString Hosts::getHostsFilePath() {
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    QString systemRoot = env.value("SystemRoot", "C:/Windows");
    return QDir(systemRoot).filePath("System32/drivers/etc/hosts");
}

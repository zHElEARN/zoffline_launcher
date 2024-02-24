#include "utils.h"

Utils::Utils()
{

}

QString Utils::getInstalledZwiftVersion(const QString &zwiftInstallFolderPath)
{
    QString version = "";

    QFile file(zwiftInstallFolderPath + "/Zwift_ver_cur.xml");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading: " << file.errorString();
        return version;
    }

    QDomDocument document;
    if (!document.setContent(&file))
    {
        qDebug() << "Failed to parse the file into a DOM tree.";
        file.close();
        return version;
    }

    file.close();

    QDomElement root = document.documentElement();
    if (root.tagName() != "Zwift")
    {
        qDebug() << "Root element name error.";
        return version;
    }

    version = root.attribute("version", "");

    return version;
}

bool Utils::getZwiftInstallLocation(QString &installLocation)
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "reg query HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall /s");
    process.waitForFinished();

    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());

    QStringList lines = output.split("\n");
    QString zwiftKey;

    for (int i = 0; i < lines.size(); ++i)
    {
        QString line = lines[i];

        if (line.contains("DisplayName") && line.contains("Zwift"))
        {
            for (int j = i - 1; j >= 0; --j)
            {
                if (lines[j].startsWith("HKEY_LOCAL_MACHINE"))
                {
                    zwiftKey = lines[j].trimmed();
                    break;
                }
            }
        }
    }

    if (!zwiftKey.isEmpty())
    {
        process.start("cmd.exe", QStringList() << "/c" << "reg query " + zwiftKey + " /v InstallLocation");
        process.waitForFinished();

        output = QString::fromLocal8Bit(process.readAllStandardOutput());
        int index = output.indexOf("REG_SZ");
        if (index != -1)
        {
            installLocation = output.mid(index + 6).trimmed();
            return true;
        }
    }

    installLocation = QString();
    return false;
}

void Utils::getLatestZofflineInfo(std::function<void (const QString &, const QString &)> callback)
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();
    QUrl url(QString("https://api.github.com/repos/%1/%2/releases/latest").arg("zoffline", "zwift-offline"));
    QNetworkRequest request(url);

    QNetworkReply *reply = manager->get(request);
    QObject::connect(reply, &QNetworkReply::finished, [reply, manager, callback]() {
        if (reply->error() == QNetworkReply::NoError)
        {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
            QJsonObject jsonObj = jsonDoc.object();

            QJsonArray assets = jsonObj["assets"].toArray();
            if (!assets.isEmpty())
            {
                QJsonObject firstAsset = assets.first().toObject();
                QString name = firstAsset["name"].toString();
                QString url = firstAsset["browser_download_url"].toString();

                callback(name, url);
            }
            else
            {
                qDebug() << "No assets found.";
            }
        }
        else
        {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
        manager->deleteLater();
    });
}

QString Utils::parseZofflineVersion(const QString &zofflineFileName)
{
    QRegularExpression re("zoffline_(.*)\\.exe");
    QRegularExpressionMatch match = re.match(zofflineFileName);
    if (match.hasMatch())
    {
        return match.captured(1);
    }
    else
    {
        return QString();
    }
}

void Utils::downloadFile(const QString &path, const QString &name, const QString &url, std::function<void ()> callback)
{
    QNetworkAccessManager manager;
    QNetworkReply *reply = manager.get(QNetworkRequest(QUrl(url)));
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QFile file(path + name);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(reply->readAll());
        file.close();
        callback();
    }
    reply->deleteLater();
}

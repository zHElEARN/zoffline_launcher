#include "utils.h"

Utils::Utils()
{

}

bool Utils::isZwiftInstalled()
{
    QProcess process;
    process.start("cmd.exe", QStringList() << "/c" << "reg query HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall /s");
    process.waitForFinished();

    QString output = QString::fromLocal8Bit(process.readAllStandardOutput());

    QStringList lines = output.split("\n");

    for (const QString& line : lines)
    {
        if (line.contains("DisplayName") && line.contains("Zwift"))
        {
            return true;
        }
    }

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

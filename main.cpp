#include "mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QProcess>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QString appDir = a.applicationDirPath();
    QString toolsetDirPath = QDir::cleanPath(appDir + QDir::separator() + "toolset");
    QDir toolsetDir(toolsetDirPath);
    if (!toolsetDir.exists()) {
        qDebug() << "'toolset'文件夹不存在，创建";
        if (toolsetDir.mkpath(".")) {
            qDebug() << "'toolset'文件夹创建成功";
        } else {
            qDebug() << "'toolset'文件夹无法创建";
        }
    } else {
        qDebug() << "'toolset'文件夹已存在";
    }

    MainWindow w;
    w.show();
    return a.exec();
}

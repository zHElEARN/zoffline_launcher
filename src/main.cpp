#include "mainwindow/mainwindow.h"

#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QProcess>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setStyle(QStyleFactory::create("Fusion"));

    MainWindow w;
    w.show();
    return a.exec();
}

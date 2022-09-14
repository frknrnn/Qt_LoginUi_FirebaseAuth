#include <QCoreApplication>
#include "mainwindow.h"
#include <QApplication>
#include "authhandler.h"
#include <QSslSocket>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qDebug()<< QSslSocket::sslLibraryBuildVersionString();
    qDebug() << QSslSocket::supportsSsl();
    qDebug() << QSslSocket::sslLibraryVersionString();
    w.show();
    return a.exec();
}

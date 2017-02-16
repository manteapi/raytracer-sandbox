#include <QGuiApplication>
#include <QtQuick/QQuickView>
#include <QtQml/QQmlApplicationEngine>
#include <QQmlContext>
#include "viewer.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);
    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    qmlRegisterType<Viewer>("RTApplicationComponents", 1, 0, "Viewer");

    QQmlApplicationEngine engine(QUrl("../qml/main.qml"));

    return app.exec();
}


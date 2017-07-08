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

    qmlRegisterType<Viewer>("RTApplicationComponents", 1, 0, "Viewer");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:///qml/main.qml"));
    view.show();

    return app.exec();
}


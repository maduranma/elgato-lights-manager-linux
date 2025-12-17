#include <QQmlApplicationEngine>
#include <QApplication>
#include <QQuickWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QQmlContext>

#include "lights.h"

QQmlApplicationEngine *engine = nullptr;

QObject *root = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/svg/bulb.svg"));

    engine = new QQmlApplicationEngine;

    Lights lights;
    qmlRegisterType<Light>("ElgatoLightsManagerLinux", 1, 0, "Light");

    engine->rootContext()->setContextProperty("lightsModel", &lights);
    engine->load(QUrl("qrc:/qml/Main.qml"));
    root = engine->rootObjects().first();

    QSystemTrayIcon tray;
    tray.setIcon(QIcon(":/svg/bulb.svg"));
    tray.setToolTip("Elgato Lights Linux");
    tray.show();

    QObject::connect(&tray, &QSystemTrayIcon::activated, [&](QSystemTrayIcon::ActivationReason reason){
        if (reason == QSystemTrayIcon::Trigger) {
            QMetaObject::invokeMethod(root, [] {
                auto window = static_cast<QQuickWindow *>(root);
                if (!window->isVisible()) {
                    window->show();
                    return;
                }
                if (!window->isActive()) {
                    window->raise();
                    window->requestActivate();
                    return;
                }
                window->hide();
            });
        }
    });

    QMenu menu;
    menu.addAction("Show", &app, [] {
        auto window = static_cast<QQuickWindow *>(root);
        window->show();
        window->raise();
        window->requestActivate();
    });
    menu.addAction("Quit", &app, [&app] {
        delete root;
        app.exit();
    });
    tray.setContextMenu(&menu);

    return app.exec();
}

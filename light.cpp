#include "light.h"
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QJsonArray>
#include <QJsonObject>

Light::Light(QString name, std::string address, u_int16_t port, QObject *parent) : QObject(parent) {
    m_name = name;
    this->address = address;
    this->port = port;

    manager = new QNetworkAccessManager(this);

    throttleTimer.setInterval(50);
    throttleTimer.setSingleShot(true);
    connect(&throttleTimer, &QTimer::timeout, this, &Light::sendCommand);

    pollTimer.setInterval(1000);
    connect(&pollTimer, &QTimer::timeout, this, &Light::pollState);
    pollTimer.start();
    pollState();
}

void Light::setBrightness(int b, bool fromNetwork) {
    if (b == m_brightness) return;
    m_brightness = b;
    emit brightnessChanged();

    if (!fromNetwork) {
        throttleTimer.start();
        pollTimer.start();
    }
}

void Light::setColorTemperature(int c, bool fromNetwork) {
    if (c == m_colorTemperature) return;
    m_colorTemperature = c;
    emit colorTemperatureChanged();

    if (!fromNetwork) {
        throttleTimer.start();
        pollTimer.start();
    }
}

void Light::setEnabled(bool e, bool fromNetwork) {
    if (e == m_enabled) return;
    m_enabled = e;
    emit enabledChanged();

    if (!fromNetwork) {
        throttleTimer.start();
        pollTimer.start();
    }
}

void Light::sendCommand() {
    QUrl url(QString("http://%1:%2/elgato/lights").arg(address).arg(QString::number(port)));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject lightObj;
    lightObj["brightness"] = m_brightness;
    lightObj["temperature"] = m_colorTemperature;
    lightObj["on"] = m_enabled ? 1 : 0;

    QJsonArray lightsArray;
    lightsArray.append(lightObj);

    QJsonObject payload;
    payload["numberOfLights"] = 1;
    payload["lights"] = lightsArray;

    QNetworkReply *reply = manager->put(request, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, [reply]() {
        if (reply->error() != QNetworkReply::NoError)
            qDebug() << "HTTP error:" << reply->errorString();
        reply->deleteLater();
    });
}

void Light::pollState() {
    QUrl url(QString("http://%1:%2/elgato/lights").arg(address).arg(QString::number(port)));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, [this, reply]() {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "HTTP error:" << reply->errorString();
            reply->deleteLater();
            return;
        }

        QJsonObject lightData = QJsonDocument::fromJson(reply->readAll()).object()["lights"].toArray()[0].toObject();
        QMetaObject::invokeMethod(this, [this, lightData]{
            setBrightness(lightData["brightness"].toInt(), true);
            setColorTemperature(lightData["temperature"].toInt(), true);
            setEnabled(lightData["on"].toInt() == 1, true);
        }, Qt::QueuedConnection);

        reply->deleteLater();
    });
}

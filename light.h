#pragma once

#include <QObject>
#include <QMetaObject>
#include <thread>
#include <chrono>
#include <QNetworkAccessManager>
#include <QTimer>

class Light : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(int brightness READ brightness NOTIFY brightnessChanged)
    Q_PROPERTY(int colorTemperature READ colorTemperature NOTIFY colorTemperatureChanged)
    Q_PROPERTY(bool enabled READ enabled NOTIFY enabledChanged)

public:
    explicit Light(QString name, std::string address, uint16_t port, QObject *parent = nullptr);

    QString m_name = "";
    QString name() const { return m_name; }
    int brightness() const { return m_brightness; }
    int colorTemperature() const { return m_colorTemperature; }
    bool enabled() const { return m_enabled; }

    signals:
    void nameChanged();
    void brightnessChanged();
    void colorTemperatureChanged();
    void enabledChanged();

public slots:
    void setBrightness(int b, bool fromNetwork = false);
    void setColorTemperature(int c, bool fromNetwork = false);
    void setEnabled(bool e, bool fromNetwork = false);

private:
    void sendCommand();
    void pollState();

    QTimer pollTimer;
    QTimer throttleTimer;

    QNetworkAccessManager* manager;

    std::string address;
    uint16_t port;

    int m_brightness = 0;
    int m_colorTemperature = 2700;
    bool m_enabled = true;
};

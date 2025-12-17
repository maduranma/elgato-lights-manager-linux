#pragma once

#include <QObject>
#include <QList>
#include <thread>
#include <atomic>

#include "light.h"

class Lights : public QObject {
    Q_OBJECT
    Q_PROPERTY(QList<QObject*> items READ items NOTIFY itemsChanged)

public:
    explicit Lights(QObject *parent = nullptr);
    ~Lights();

    std::thread worker;
    std::atomic_bool running{true};

    QList<QObject*>& items();

    signals:
        void itemsChanged();

private:
    void threadLoop();

    QList<QObject*> m_items;
};

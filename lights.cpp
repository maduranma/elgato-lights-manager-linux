#include "lights.h"

#include <QCoreApplication>
#include <QMetaObject>
#include <avahi-client/client.h>
#include <avahi-client/lookup.h>
#include <avahi-common/simple-watch.h>

Lights::Lights(QObject *parent) : QObject(parent) {
    worker = std::thread([this] { threadLoop(); });
}

Lights::~Lights() {
    running.store(false);
    if (worker.joinable()) worker.join();

    qDeleteAll(m_items);
    m_items.clear();
}

QList<QObject*>& Lights::items() {
    return m_items;
}

static std::vector<std::string> resolvedServices;

static void resolve_callback(
    AvahiServiceResolver *r,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiResolverEvent event,
    const char *name,
    const char *type,
    const char *domain,
    const char *host_name,
    const AvahiAddress *address,
    uint16_t port,
    AvahiStringList *txt,
    AvahiLookupResultFlags flags,
    void *userdata)
{
    auto *lights = static_cast<Lights *>(userdata);
    if (event == AVAHI_RESOLVER_FOUND) {
        char addr[AVAHI_ADDRESS_STR_MAX];
        avahi_address_snprint(addr, sizeof(addr), address);
        resolvedServices.push_back(std::string(name));

        QString qName = QString::fromLatin1(name);
        QMetaObject::invokeMethod(lights, [qName, addr, port, lights] {
            for (QObject* obj : lights->items()) {
                auto light = static_cast<Light*>(obj);
                if (light->m_name == qName)
                    return;
            }
            Light* light = new Light(qName, std::string(addr), port, lights);
            lights->items().append(light);
            emit lights->itemsChanged();
        }, Qt::QueuedConnection);
    }

    avahi_service_resolver_free(r);
}

struct UserData {
    AvahiClient *client;
    Lights *lights;
};

static void browse_callback(
    AvahiServiceBrowser *b,
    AvahiIfIndex interface,
    AvahiProtocol protocol,
    AvahiBrowserEvent event,
    const char *name,
    const char *type,
    const char *domain,
    AvahiLookupResultFlags flags,
    void *userdata)
{
    auto *userDataStruct = static_cast<UserData *>(userdata);
    if (event == AVAHI_BROWSER_NEW) {
        avahi_service_resolver_new(
            userDataStruct->client,
            interface,
            protocol,
            name,
            type,
            domain,
            AVAHI_PROTO_UNSPEC,
            static_cast<AvahiLookupFlags>(0),
            resolve_callback,
            userDataStruct->lights
        );
    }
}


void Lights::threadLoop() {
    while (running) {
        AvahiSimplePoll *poll = avahi_simple_poll_new();
        AvahiClient *client = avahi_client_new(
            avahi_simple_poll_get(poll),
            static_cast<AvahiClientFlags>(0),
            nullptr,
            nullptr,
            nullptr
        );

        auto *browser = avahi_service_browser_new(
            client,
            AVAHI_IF_UNSPEC,
            AVAHI_PROTO_UNSPEC,
            "_elg._tcp",
            nullptr,
            static_cast<AvahiLookupFlags>(0),
            browse_callback,
            new UserData { client, this }
        );

        for (int i = 0; i < 50 && running; i++) {
            avahi_simple_poll_iterate(poll, 100);
        }

        avahi_service_browser_free(browser);
        avahi_client_free(client);
        avahi_simple_poll_free(poll);

        QMetaObject::invokeMethod(this, [this] {
            for (QObject* obj : m_items) {
                auto light = static_cast<Light*>(obj);
                auto it = std::find(resolvedServices.begin(), resolvedServices.end(), light->m_name.toStdString());
                if (it != resolvedServices.end()) {
                    continue;
                }

                m_items.removeAll(light);
                Q_EMIT itemsChanged();

                delete light;
            }

            resolvedServices.clear();
        }, Qt::QueuedConnection);
    }
}

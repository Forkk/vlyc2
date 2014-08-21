/*****************************************************************************
 * vlyc2 - A Desktop YouTube client
 * Copyright (C) 2013 Orochimarufan <orochimarufan.x3@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#include "VlycDBus.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtDBus/QDBusError>

VlycDBus::VlycDBus(VlycApp* app)
{
    mp_app = app;
}

VlycDBus::~VlycDBus()
{

}

bool VlycDBus::tryRegister()
{
    // Check if the service is already registered.
    QDBusInterface iface(DBUS_SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
    if (iface.isValid())
    {
        m_alreadyRunning = true;
        return false;
    }
    
    // Register the service.
    if (!QDBusConnection::sessionBus().registerService(DBUS_SERVICE_NAME))
    {
        fprintf(stderr, "DBus connection failed: %s %s\n",
                qPrintable(QDBusConnection::sessionBus().lastError().message()));
        return false;
    }

    QDBusConnection::sessionBus().registerObject("/", this, QDBusConnection::ExportAllSlots);
    return true;
}

void VlycDBus::playVideo(const QString &url)
{
    mp_app->play(url);
}

void VlycDBus::queueVideo(const QString &url)
{
    // TODO: Implement queueVideo.
    printf("queueVideo is not implemented yet.\n");
}

bool VlycDBus::playRunning(const QString &url, bool play)
{
    QDBusInterface iface(DBUS_SERVICE_NAME, "/", "", QDBusConnection::sessionBus());
    if (iface.isValid())
    {
        QDBusReply<void> reply = iface.call(play ? "playVideo" : "queueVideo", url);
        if (reply.isValid())
        {
            return true;
        }

        fprintf(stderr, "Call failed: %s\n", qPrintable(reply.error().message()));
        return false;
    }
    return false;
}


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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QString>

#include "vlyc.h"

#define DBUS_SERVICE_NAME "me.sodimm.oro.vlyc"


/**
 * @brief A DBus interface for Vlyc.
 * This class exports various slots which serve as an interface between Vlyc
 * and DBus.
 */
class VlycDBus : public QObject
{
    Q_OBJECT
public:
    VlycDBus(VlycApp *app);
    virtual ~VlycDBus();

    /**
     * @brief Tries to register with DBus.
     * Attempts to register vlyc's interface with DBus. If another instance of
     * vlyc is already running, this function will fail and return false.
     * Otherwise, returns true.
     */
    bool tryRegister();

    /**
     * @brief Checks if connection failed because another process is running.
     * If tryRegister fails because another instance of Vlyc is already running,
     * this function will return true.
     */
    bool alreadyRunning() { return m_alreadyRunning; }


    /**
     * @brief Plays the given video on the already running instance of Vlyc.
     * Returns false on failure.
     * If the "play" argument is true, the video will be played immediately
     * (via a call to playVideo). Otherwise it will simply be queued.
     */
    bool playRunning(const QString &url, bool play = true);

public slots:
    /**
     * @brief Starts playing the video at the given URL.
     */
    Q_SCRIPTABLE void playVideo(const QString &url);

    /**
     * @brief Adds the given URL to the current playlist.
     */
    Q_SCRIPTABLE void queueVideo(const QString &url);

private:
    VlycApp *mp_app;

    bool m_alreadyRunning;
};


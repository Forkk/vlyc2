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
#include <QtWidgets/QShortcut>

#include <QtVlc/VlcMediaPlayer.h>
#include <QtVlc/VlcMediaPlayerAudio.h>

#include "logic/VlycPlayer.h"

class MainWindow;


/**
 * @brief The object responsible for managing QShortcuts.
 * The HotkeyManager is responsible for managing QShortcuts defined in vlyc's
 * config file.
 */
class HotkeyManager : public QObject
{
public:
    HotkeyManager(VlycApp *app, MainWindow *parent);
    virtual ~HotkeyManager();

    /**
     * @brief Reads the config and binds hotkeys based on the settings.
     * This function reads through the config file and binds signals for the
     * hotkeys specified in the config.
     *
     * If this function is called twice, the previous QShortcut objects will be
     * destroyed and their hotkeys will be unbound.
     */
    void bindHotkeys();

private:
    /**
     * @brief Clears the hotkey list.
     * This function destroys all of the QShortcut objects in the hotkeys,
     * unbinding their keys, and then clears the hotkey list.
     */
    void clearHotkeys();

    /**
     * @brief The hotkey list.
     */
    QList<QShortcut*> m_hotkeys;

    /**
     * @brief A pointer to the VlycApp object.
     */
    VlycApp *m_app;

    MainWindow *m_window;

    VlcMediaPlayer m_player;
    VlcMediaPlayerAudio m_audio;
};


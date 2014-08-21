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

#include "Hotkeys.h"

#include "vlyc.h"
#include "gui/mainwindow.h"
#include "logic/VlycPlayer.h"

HotkeyManager::HotkeyManager(VlycApp *app, MainWindow *parent)
    : QObject(parent)
    , m_player(app->player()->player())
    , m_audio(m_player.audio())
{
    m_window = parent;
    m_app = app;
}

HotkeyManager::~HotkeyManager()
{
    clearHotkeys();
}


// Macro for defining a new hotkey.
// SHORTCUT is the name of the hotkey variable.
// CONF_KEY is the config file key for the hotkey.
// DEF_KEYS is the default key sequence to activate the hotkey.
// Any arguments after the above three arguments are passed to QObject::connect
#define HOTKEY(SHORTCUT, CONF_KEY, DEF_KEYS, ...) \
    QShortcut *SHORTCUT = new QShortcut(QKeySequence(DEF_KEYS), m_window); \
    m_hotkeys.append(SHORTCUT); \
    QObject::connect(SHORTCUT, &QShortcut::activated, \
                     __VA_ARGS__);


void HotkeyManager::bindHotkeys()
{
    // Clear the previous hotkeys.
    clearHotkeys();

    // Load the config file.
    QSettings config;


    // Play/Pause
    HOTKEY(hkPlaypause, "PlayPause", " ",
           &m_player, &VlcMediaPlayer::togglePause);

    // Seek (2 seconds)
    HOTKEY(hkFwd2Secs,  "Forward2Secs", "Shift+Right",
           [this]() { m_player.setTime(m_player.time() + 2*1000); });
    HOTKEY(hkBack2Secs, "Rewind2Secs",  "Shift+Left",
           [this]() { m_player.setTime(m_player.time() - 2*1000); });
    // Seek (10 seconds)
    HOTKEY(hkFwd10Secs,  "Forward10Secs", "Alt+Right",
           [this]() { m_player.setTime(m_player.time() + 10*1000); });
    HOTKEY(hkBack10Secs, "Rewind10Secs",  "Alt+Left",
           [this]() { m_player.setTime(m_player.time() - 10*1000); });
    // Seek (1 minute)
    HOTKEY(hkFwd1Min,  "Forward1Min", "Ctrl+Right",
           [this]() { m_player.setTime(m_player.time() + 60*1000); });
    HOTKEY(hkBack1Min, "Rewind1Min",  "Ctrl+Left",
           [this]() { m_player.setTime(m_player.time() - 60*1000); });

    // Volume
    HOTKEY(hkVolUp,   "VolumeUp",   "Ctrl+Up",
           [this]() { m_audio.setVolume(m_audio.volume() + 5); });
    HOTKEY(hkVolDown, "VolumeDown", "Ctrl+Down",
           [this]() { m_audio.setVolume(m_audio.volume() - 5); });
    HOTKEY(hkToggleMute, "ToggleMute", "M",
           &m_audio, &VlcMediaPlayerAudio::toggleMuted);

    // Playlist Next/Prev
    HOTKEY(hkNextVideo, "NextVideo", "N",
           m_app->player(), &VlycPlayer::next);
    HOTKEY(hkPrevVideo, "PrevVideo", "P",
           m_app->player(), &VlycPlayer::prev);

    // Fullscreen
    HOTKEY(hkToggleFull, "ToggleFullscreen", "Alt+Return",
           m_window, &MainWindow::toggleFullScreen);
}

void HotkeyManager::clearHotkeys()
{
    // Delete all of the shortcut objects.
    for (auto iter = m_hotkeys.begin(); iter != m_hotkeys.end(); iter++)
    {
        delete *iter;
    }
    // Clear the list.
    m_hotkeys.clear();
}


/*
    Copyright © 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of HeyTrack.

    HeyTrack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    HeyTrack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "VlcPlayer.h"

#include <QtDBus/QDBusInterface>

namespace HeyTrack { namespace Core {

VlcPlayer::VlcPlayer(QObject* parent): AbstractPlayer(parent) {
    playerInterface = new QDBusInterface("org.mpris.vlc", "/Player", "", QDBusConnection::sessionBus(), this);
    tracklistInterface = new QDBusInterface("org.mpris.vlc", "/TrackList", "", QDBusConnection::sessionBus(), this);
}

void VlcPlayer::play(const QString& url) {
    /* Add track and start playing immediately */
    tracklistInterface->call("AddTrack", url, true);
}

void VlcPlayer::stop() {
    playerInterface->call("Stop");
}

}}

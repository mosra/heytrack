/*
    Copyright © 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
#include <QtDBus/QDBusReply>

#include "Mpris1.h"

namespace HeyTrack { namespace Core {

VlcPlayer::VlcPlayer(QObject* parent): AbstractPlayer(parent) {
    playerInterface = new QDBusInterface("org.mpris.vlc", "/Player", "", QDBusConnection::sessionBus(), this);
    tracklistInterface = new QDBusInterface("org.mpris.vlc", "/TrackList", "", QDBusConnection::sessionBus(), this);
}

bool VlcPlayer::isPlaying() {
    QDBusReply<Mpris1::Status> reply = playerInterface->call("GetStatus");

    if(reply.isValid())
        return reply.value().play == 0;
    return false;
}

void VlcPlayer::play(const QString& url) {
    /* Add track and start playing immediately */
    tracklistInterface->call("AddTrack", url, true);
}

void VlcPlayer::stop() {
    playerInterface->call("Stop");
}

}}

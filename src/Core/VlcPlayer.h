#ifndef HeyTrack_Core_VlcPlayer_h
#define HeyTrack_Core_VlcPlayer_h
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

/** @file
 * @brief Class HeyTrack::Core::VlcPlayer
 */

#include "AbstractPlayer.h"

class QDBusInterface;

namespace HeyTrack { namespace Core {

class VlcPlayer: public AbstractPlayer {
    PLAYER_DEFINE(VlcPlayer)

    public:
        VlcPlayer(QObject* parent = 0);
        virtual bool isPlaying();
        virtual void play(const QString& url);
        virtual void stop();

    private:
        QDBusInterface *playerInterface,
            *tracklistInterface;
};

}}

#endif

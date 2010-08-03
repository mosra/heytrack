#ifndef HeyTrack_Core_PhononPlayer_h
#define HeyTrack_Core_PhononPlayer_h
/*
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

/** @file
 * @brief Class HeyTrack::Core::PhononPlayer
 *
 * @todo add error reporting
 */

#include "AbstractPlayer.h"

namespace Phonon {
    class AudioOutput;
    class MediaObject;
}

namespace HeyTrack { namespace Core {

class PhononPlayer: public AbstractPlayer {
    PLAYER_DEFINE(PhononPlayer)

    public:
        PhononPlayer(QObject* parent = 0);
        virtual void play(const QString& url);
        virtual void stop();

    private:
        Phonon::AudioOutput* ao;
        Phonon::MediaObject* mo;
};

}}

#endif

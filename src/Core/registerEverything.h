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

#include "AbstractServer.h"
#include "AbRadioServer.h"
#include "RockmaxServer.h"
#include "SomaFmServer.h"
#include "PhononPlayer.h"

/* D-BUS is Unix-only */
#ifndef Q_WS_WIN
#include "VlcPlayer.h"
#endif

namespace HeyTrack { namespace Core {

inline void registerEverything() {
    SERVER_REGISTER("ABRadio.cz", AbRadioServer)
    SERVER_REGISTER("Rockmax.cz", RockmaxServer)
    SERVER_REGISTER("SomaFM.com", SomaFmServer)
    PLAYER_REGISTER("Phonon", PhononPlayer)

    /* D-BUS is Unix-only */
    #ifndef Q_WS_WIN
    PLAYER_REGISTER("VLC", VlcPlayer)
    #endif
}

}}

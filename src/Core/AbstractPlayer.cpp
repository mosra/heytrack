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

#include "AbstractPlayer.h"

namespace HeyTrack { namespace Core {

QHash<QString, AbstractPlayer* (*)(QObject*)> AbstractPlayer::_players;

void AbstractPlayer::registerPlayer(const QString& name, AbstractPlayer* (*instancer)(QObject*)) {
    _players.insert(name, instancer);
}

QList<QString> AbstractPlayer::players() {
    return _players.keys();
}

AbstractPlayer* AbstractPlayer::instance(const QString& name, QObject* parent) {
    if(!_players.contains(name)) return 0;

    return _players[name](parent);
}

}}

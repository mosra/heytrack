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

#include "PhononPlayer.h"

#include <phonon/AudioOutput>
#include <phonon/MediaObject>
#include <QtCore/QUrl>

namespace HeyTrack { namespace Core {

PhononPlayer::PhononPlayer(QObject* parent): AbstractPlayer(parent)
{
    ao = new Phonon::AudioOutput(this);
    mo = new Phonon::MediaObject(this);

    //Connect audio source with output
    Phonon::createPath(mo, ao);
}

bool PhononPlayer::isPlaying() {
    return mo->state() == Phonon::PlayingState;
}

void PhononPlayer::play(const QString& url) {
    QUrl _url(url);
    mo->setCurrentSource(_url);

    mo->play();
}

void PhononPlayer::stop() {
    mo->stop();
}

}}

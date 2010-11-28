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

#include "AmarokPlayer.h"

#include <QtCore/QUrl>
#include <QtCore/QRegExp>
#include <QtNetwork/QNetworkReply>
#include <QtDBus/QDBusInterface>

namespace HeyTrack { namespace Core {

AmarokPlayer::AmarokPlayer(QObject* parent): AbstractPlayer(parent), manager(new QNetworkAccessManager(this)) {
    playerInterface = new QDBusInterface("org.kde.amarok", "/Player", "", QDBusConnection::sessionBus(), this);
    tracklistInterface = new QDBusInterface("org.kde.amarok", "/TrackList", "", QDBusConnection::sessionBus(), this);
}

bool AmarokPlayer::isPlaying() {
    return playerInterface->property("PlaybackStatus").toString() == "Playing";
}

void AmarokPlayer::play(const QString& url) {
    QRegExp rxAsx("\\.asx$");

    /* Url is not asx playlist, play it directly */
    if(rxAsx.indexIn(url) == -1)
        tracklistInterface->call("AddTrack", url, true);

    /* Otherwise parse asx playlist */
    else {
        manager->get(QNetworkRequest(QUrl(url)));
        connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(processPlaylist(QNetworkReply*)));
    }
}

void AmarokPlayer::processPlaylist(QNetworkReply* reply) {
    QRegExp rxUrl("<ref href\\s?=\\s?\"([^\"]+)\"", Qt::CaseInsensitive);

    if(rxUrl.indexIn(QString::fromLatin1(reply->readAll())) != -1) {
        QString url = rxUrl.cap(1);
        tracklistInterface->call("AddTrack", url, true);
    }

    reply->deleteLater();
}

void AmarokPlayer::stop() {
    playerInterface->call("Stop");
}

}}

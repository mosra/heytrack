/*
    Copyright © 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010, 2011 Jan Dupal <dupal.j@seznam.cz>

    This file is part of HeyTrack.

    HeyTrack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    HeyTrack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "RockmaxServer.h"

#include <QtCore/QUrl>
#include <QtCore/QRegExp>
#include <QtCore/QTextCodec>
#include <QtNetwork/QNetworkReply>

namespace HeyTrack { namespace Core {

RockmaxServer::RockmaxServer(QObject* parent): AbstractServer(parent) {
    codec = QTextCodec::codecForName("Windows-1250");
    decoder = codec->makeDecoder();
}

RockmaxServer::~RockmaxServer() {
    delete decoder;
}

void RockmaxServer::getStations(const Genre& genre) {
    QList<Station> list;
    list.append(Station(0, "rockmax", "Rockmax Live"));
    list.append(Station(1, "old", "Rockmax Oldies"));
    list.append(Station(2, "hard", "Rockmax Hard"));
    emit stations(list);
}

void RockmaxServer::getFormats(const Station& station) {
    QList<Format> list;
    list.append(Format(1, "64", QString("MP3 %0 kbps").arg(64)));
    list.append(Format(2, "128", QString("MP3 %0 kbps").arg(128)));
    list.append(Format(3, "256", QString("MP3 %0 kbps").arg(256)));
    emit formats(list);
}

void RockmaxServer::getTrack(const Station& station) {
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(
	station.id() > 0
	    ? QString("http://www.rockmax.cz/stream_%0/all_songs_%0.txt").arg(station.nick())
	    : QString("http://www.rockmax.cz/stream_live/all_songs.txt")
    )));
    connect(reply, SIGNAL(finished()), SLOT(processTrack()));
}

void RockmaxServer::processTrack() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) return;

    QString content = decoder->toUnicode(reply->readAll());
    reply->deleteLater();

    /* Regexp for getting song artist and title */
    QRegExp rxSong("(?:songactual|hraje)=(.*)&(?:songactualcas|hralo)");
    int songPos = rxSong.indexIn(content);
    if(songPos == -1) {
        emit error("Cannot parse song name");
        return;
    }

    QString song = rxSong.cap(1);
    if(song == "neco jineho nez hudba -")
        return;

    QRegExp rxSplit("(.+) - (.+)");
    songPos = rxSplit.indexIn(song);
    if(songPos == -1) {
        emit error("Cannot parse song name");
        return;
    }

    /* If current artist and title is the same as in last check, exit */
    QString currentArtist = rxSplit.cap(1),
        currentTitle = rxSplit.cap(2);
    if(currentArtist == lastArtist && currentTitle == lastTitle) return;

    /* Save artist and title for next check */
    lastArtist = currentArtist;
    lastTitle = currentTitle;

    emit track(Track(currentArtist, currentTitle));
}

QString RockmaxServer::streamUrl(const Station& station, const Format& format) const {
    return QString("http://212.111.2.151:8000/%0%1_%2.mp3")
        .arg(station.id()>0 ? "rm_":"").arg(station.nick()).arg(format.nick());
}

}}

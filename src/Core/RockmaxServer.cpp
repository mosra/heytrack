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

#include "RockmaxServer.h"

#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkReply>
#include <QDebug>


namespace HeyTrack { namespace Core {

void RockmaxServer::getStations(const Genre& genre) {
    QList<Station> list;
    list.append(Station(1, "old", "Rockmax Oldies"));
    list.append(Station(2, "hard", "Rockmax Hard"));
    emit stations(list);
}

void RockmaxServer::getFormats(const Station& station) {
    QList<Format> list;
    list.append(Format(1, "64", tr("format MP3 (%0 kbps)").arg(64)));
    list.append(Format(2, "128", tr("format MP3 (%0 kbps)").arg(128)));
    list.append(Format(3, "256", tr("format MP3 (%0 kbps)").arg(256)));
    emit formats(list);
}

void RockmaxServer::getTrack(const Station& station) {
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(QString("http://www.rockmax.cz/stream_%0/load_song_flash.php").arg(station.nick()))));
    connect(reply, SIGNAL(finished()), SLOT(processTrack()));
}

void RockmaxServer::processTrack() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) return;

    QByteArray content = reply->readAll();
    reply->deleteLater();

    /* Regexp for getting song artist and title */
    QRegExp rxSong("songactual=(.+) - (.+)&songactualcas");
    int songPos = rxSong.indexIn(content);
    if(songPos == -1) {
        emit error("Cannot parse song name");
        return;
    }

    /* Regexp for getting timestamp */
    QRegExp rxStamp("stamp=\\d{3}(\\d+)");
    int stampPos = rxStamp.indexIn(content);
    if(stampPos == -1) {
        emit error("Cannot parse timestamp");
        return;
    }

    /* If track wasn't updated, nothing to do */
    quint32 update = rxStamp.cap(1).toUInt();
    if(lastUpdate == update && update != 0) return;
    lastUpdate = update;

    qDebug() << update;

    Track t(rxSong.cap(1), rxSong.cap(2));
    emit track(t);
}

QString RockmaxServer::streamUrl(const Station& station, const Format& format) const {
    return QString("http://212.111.2.151:8011/rm_%0_%1.mp3")
        .arg(station.nick()).arg(format.nick());
}

}}

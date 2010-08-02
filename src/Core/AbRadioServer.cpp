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

#include "AbRadioServer.h"

#include <QtCore/QUrl>
#include <QtCore/QStringList>
#include <QtNetwork/QNetworkReply>
#include <QtXmlPatterns/QXmlQuery>
#include <qjson/parser.h>

namespace HeyTrack { namespace Core {

void AbRadioServer::getGenres() {
    QList<Genre> list;
    list.append(Genre(58,   "Český rozhlas"));
    list.append(Genre(8,    "Dance"));
    list.append(Genre(54,   "Folk & Country"));
    list.append(Genre(1,    "Hip Hop & R'n'B"));
    list.append(Genre(29,   "Jazz & Blues"));
    list.append(Genre(57,   "Mluvené slovo"));
    list.append(Genre(7,    "Pop"));
    list.append(Genre(2,    "Rock"));
    list.append(Genre(59,   "Sólo pro..."));
    list.append(Genre(64,   "World Music"));
    emit genres(list);
}

void AbRadioServer::getStations(Genre genre) {
    if(genre.id() == 0) {
        emit stations(QList<Station>());
        return;
    }

    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(QString("http://www.abradio.cz/data/stationlist/%0").arg(genre.id()))));
    connect(reply, SIGNAL(finished()), SLOT(processStations()));
}

void AbRadioServer::processStations() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) return;

    QXmlQuery query;
    query.setFocus("<root>" + QString::fromUtf8(reply->readAll()) + "</root>");
    reply->deleteLater();

    QStringList ids, names;

    /* Station IDs */
    query.setQuery("root/option/@value/string()");
    if(!query.isValid()) {
        emit error("Cannot parse station list");
        return;
    }
    query.evaluateTo(&ids);

    /* Station names */
    query.setQuery("root/option/string()");
    if(!query.isValid()) {
        emit error("Cannot parse station list");
        return;
    }
    query.evaluateTo(&names);

    /* If station id count is not the same as station name count, error */
    if(ids.count() != names.count()) {
        emit error("Cannot parse station list");
        return;
    }

    /* Create station list */
    QList<Station> list;
    for(int i = 0; i != ids.size(); ++i) {
        QStringList idnick = ids[i].split('/', QString::SkipEmptyParts);

        list.append(Station(idnick[0].toUInt(), idnick[1], names[i]));
    }

    emit stations(list);
}

void AbRadioServer::getTrack(Station station) {
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl(QString("http://static.abradio.cz/data/ct/%0.json").arg(station.id()))));
    connect(reply, SIGNAL(finished()), SLOT(processTrack()));
}

void AbRadioServer::processTrack() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) return;

    QJson::Parser parser;
    QXmlQuery query;
    QString artist, title;
    bool parsedOk = false;

    /* Parse server result */
    QVariantMap result = parser.parse(reply->readAll().data(), &parsedOk).toMap();
    if(!parsedOk) {
        emit error("Cannot parse JSON");
        return;
    }
    reply->deleteLater();

    /* If track wasn't updated, nothing to do */
    quint32 update = result["lastchange"].toUInt();
    if(lastUpdate == update && update != 0) return;
    lastUpdate = update;

    /* Root item for getting data */
    query.setFocus("<root>" + result["html"].toString().replace("&nbsp;","") + "</root>");

    /* Artist */
    query.setQuery("root/li[@class='current']/span[@class='artistname']//text()");
    if (!query.isValid()) {
        emit error("Cannot parse artist");
        return;
    }
    query.evaluateTo(&artist);

    /* Track name */
    query.setQuery("root/li[@class='current']/span[@class='trackname']//text()");
    if (!query.isValid()) {
        emit error("Cannot parse track");
        return;
    }
    query.evaluateTo(&title);

    Track t(artist.trimmed(), title.trimmed());
    emit track(t);
}

}}

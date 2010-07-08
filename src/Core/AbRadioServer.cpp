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
#include <QtNetwork/QNetworkReply>
#include <QtXmlPatterns/QXmlQuery>
#include <qjson/parser.h>

namespace HeyTrack { namespace Core {

AbRadioServer::AbRadioServer(QObject* parent): AbstractServer(parent), lastUpdate(0) {
    connect(manager, SIGNAL(finished(QNetworkReply*)), SLOT(processTrack(QNetworkReply*)));
}

void AbRadioServer::getStations(Genre genre) {
    QList<Station> list;
    list << Station(36, "RockRadio Prácheň");
    emit stations(list);
}

void AbRadioServer::getTrack(Station station) {
    manager->get(QNetworkRequest(QUrl(QString("http://static.abradio.cz/data/ct/%0.json").arg(station.id()))));
}

void AbRadioServer::processTrack(QNetworkReply* reply) {
    QJson::Parser parser;
    QXmlQuery query;
    QString artist, title;
    bool parsedOk = false;

    /* Parse server result */
    QVariantMap result = parser.parse(reply->readAll().data(), &parsedOk).toMap();
    if(!parsedOk) emit error("An error occurred during parsing JSON");
    reply->deleteLater();

    /* If track wasn't updated, nothing to do */
    quint32 update = result["lastchange"].toUInt();
    if(lastUpdate == update && update != 0) return;
    lastUpdate = update;

    /* Root item for getting data */
    query.setFocus("<root>" + result["html"].toString().replace("&nbsp;","") + "</root>");

    /* Artist */
    query.setQuery("root/li[@class='current']/span[@class='artistname']//text()");
    if (!query.isValid()) emit error("An error occurred during parsing artist");
    query.evaluateTo(&artist);

    /* Track name */
    query.setQuery("root/li[@class='current']/span[@class='trackname']//text()");
    if (!query.isValid()) emit error("An error occurred during parsing track");
    query.evaluateTo(&title);

    Track t(artist.trimmed(), title.trimmed());
    emit track(t);
}

}}

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

#include "SomaFmServer.h"

#include <QtCore/QStringList>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtXmlPatterns/QXmlQuery>
#include <QtXmlPatterns/QXmlResultItems>

namespace HeyTrack { namespace Core {

SomaFmServer::SomaFmServer(QObject* parent): AbstractServer(parent), formatMapper(new QSignalMapper(this)), trackMapper(new QSignalMapper(this)) {
    connect(formatMapper, SIGNAL(mapped(QString)), SLOT(processFormats(QString)));
    connect(trackMapper, SIGNAL(mapped(QString)), SLOT(processTrack(QString)));
}

void SomaFmServer::getStations(const Genre& genre) {
//     QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://somafm.com/listen/")));
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://somafm.com/channels.xml")));
    connect(reply, SIGNAL(finished()), SLOT(processStations()));
}

void SomaFmServer::processStations() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if(!reply) return;

//     QString data = QString::fromUtf8(reply->readAll())
//         .replace('&', "") /** @todo Fix for HTML entities */
//
//         /* Fix one extra div */
//         .replace("<!-- End of stations -->\n\n</div>\n\n<!-- start right side -->", "")
//
//         /* Strip <img>s without /> */
//         .replace(QRegExp("<img[^>]+>"), "");
//
//     QXmlQuery query;
//     query.setFocus(data.toUtf8());
//
//     /* Everything. Every <li> has station name, all formats and current track. */
//     query.setQuery("*:html/*:body/*:div[@id='stations']/*:ul/*:li");

    QXmlQuery query;
    query.setFocus(QString::fromLatin1(reply->readAll()));
    reply->deleteLater();

    query.setQuery("channels/channel");
    if(!query.isValid()) {
        emit error("Cannot parse station list");
        return;
    }

    QXmlResultItems result;
    query.evaluateTo(&result);

    /* All stations */
    int id = 0;
    QList<Station> list;
    for(QXmlItem station(result.next()); !station.isNull(); station = result.next()) {
        query.setFocus(station);

        /* Station nick */
        QString nick;
        query.setQuery("@id/string()");
        if(!query.isValid()) {
            emit error("Cannot parse station");
            return;
        }
        query.evaluateTo(&nick);

        /* Station name */
        QString name;
        query.setQuery("title/string()");
        if(!query.isValid()) {
            emit error("Cannot parse station");
            return;
        }
        query.evaluateTo(&name);

        list << Station(id++, nick.trimmed(), name.trimmed());
    }

    emit stations(list);
}

void SomaFmServer::getFormats(const Station& station) {
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://somafm.com/channels.xml")));
    connect(reply, SIGNAL(finished()), formatMapper, SLOT(map()));
    formatMapper->setMapping(reply, station.nick());
}

void SomaFmServer::processFormats(const QString& station) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(formatMapper->mapping(station));
    if(!reply) return;

    QXmlQuery query;
    query.setFocus(QString::fromLatin1(reply->readAll()));
    reply->deleteLater();

    query.setQuery(QString("channels/channel[@id='%0']/fastpls | channels/channel[@id='%0']/slowpls").arg(station));
    if(!query.isValid()) {
        emit error("Cannot parse format list");
        return;
    }

    QXmlResultItems result;
    query.evaluateTo(&result);

    QRegExp rx("\\d*(?=\\.pls)");

    /* List of all formats */
    int id = 0;
    QList<Format> list;
    for(QXmlItem format(result.next()); !format.isNull(); format = result.next()) {
        query.setFocus(format);

        /* MP3 / AAC+ */
        QString type;
        query.setQuery("@format/string()");
        if(!query.isValid()) {
            emit error("Cannot parse format");
            return;
        }
        query.evaluateTo(&type);

        /* Cleanup type */
        type = type.trimmed();
        if(type == "mp3") type = "MP3";
        else if(type == "aacp") type = "AAC+";

        /* Stream URL */
        QString url;
        query.setQuery("string()");
        if(!query.isValid()) {
            emit error("Cannot parse format");
            return;
        }
        query.evaluateTo(&url);

        /* Get bitrate from URL */
        if(rx.indexIn(url) == -1) {
            emit error("Cannot parse format");
            return;
        }
        int bitrate = rx.cap(0).toInt();
        if(!bitrate) bitrate = 128;

        /* Format name */
        list << Format(id++, url.trimmed(), QString("%0 %1 kbps").arg(type).arg(bitrate));
    }

    emit formats(list);
}

void SomaFmServer::getTrack(const Station& station) {
    QNetworkReply* reply = manager->get(QNetworkRequest(QUrl("http://somafm.com/channels.xml")));
    connect(reply, SIGNAL(finished()), trackMapper, SLOT(map()));
    trackMapper->setMapping(reply, station.nick());
}

void SomaFmServer::processTrack(const QString& station) {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(trackMapper->mapping(station));
    if(!reply) return;

    QXmlQuery query;
    query.setFocus(QString::fromLatin1(reply->readAll()));
    reply->deleteLater();

    QString current;
    query.setQuery(QString("channels/channel[@id='%0']/lastPlaying/string()").arg(station));
    if(!query.isValid()) {
        emit error("Cannot parse current track");
        return;
    }
    query.evaluateTo(&current);

    QString currentArtist, currentTitle;
    QStringList c = current.trimmed().split(" - ");
    if(c.size() >= 2) {
        currentArtist = c[0];
        currentTitle = c[1];
    }

    if(currentArtist == lastArtist && currentTitle == lastTitle) return;

    /* Save artist and title for next check */
    lastArtist = currentArtist;
    lastTitle = currentTitle;

    emit track(Track(currentArtist, currentTitle));
}

}}

#ifndef HeyTrack_Core_AbstractServer_h
#define HeyTrack_Core_AbstractServer_h
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

/** @file
 * @brief Class HeyTrack::Core::AbstractServer
 */

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtNetwork/QNetworkAccessManager>

#include "Genre.h"
#include "Station.h"
#include "Format.h"
#include "Track.h"

class QNetworkReply;

namespace HeyTrack { namespace Core {

/** @brief Abstract class for stream servers */
class AbstractServer: public QObject {
    Q_OBJECT

    protected:
        /** @brief Network access manager */
        QNetworkAccessManager* manager;

    public:
        static void registerServer(const QString& name, AbstractServer* (*instancer)(QObject*));
        static QList<QString> servers();
        static AbstractServer* instance(const QString& name, QObject* parent = 0);

        /**
         * @brief Constructor
         *
         * @param parent    Parent object
         */
        inline AbstractServer(QObject* parent = 0): QObject(parent), manager(new QNetworkAccessManager(this)) {}

    public slots:
        /**
         * @brief Available genres on server
         *
         * Emits AbstractServer::genres() with list of available genres.
         */
        inline virtual void getGenres() { emit genres(QList<Genre>()); }

        /**
         * @brief Available stations for given genre
         *
         * Emits AbstractServer::stations() with list of stations.
         * If an invalid genre (with zero ID) was given, the list contains all
         * stations on the server. On error emits AbstractServer::error().
         */
        virtual void getStations(const Genre& genre = Genre()) = 0;

        /**
         * @brief Available formats for given station
         *
         * Emits AbstractServer::formats() with list of formats. On error emits
         * AbstractServer::error().
         */
        virtual void getFormats(const Station& station) = 0;

        /**
         * @brief Current track
         *
         * If the track changed since last request, emits
         * AbstractServer::currentTrack() with current track. If the track
         * didn't change, doesn't emit anything. On error emits
         * AbstractServer::error().
         */
        virtual void getTrack(const Station& station) = 0;

        /**
         * @brief Stream URL
         * @return URL for stream of given station
         */
        virtual QString streamUrl(const Station& station, const Format& format) const = 0;

    signals:
        void genres(QList<Core::Genre> list);
        void stations(QList<Core::Station> list);
        void formats(QList<Core::Format> list);
        void track(Core::Track t);
        void error(QString message);

    private:
        static QHash<QString, AbstractServer* (*)(QObject*)> _servers;
};

}}

#define SERVER_DEFINE(class) public: \
    static inline AbstractServer* instancer(QObject* parent = 0) \
        { return new class(parent); } \
    private:

#define SERVER_REGISTER(name, class) AbstractServer::registerServer(name, class::instancer);

#endif

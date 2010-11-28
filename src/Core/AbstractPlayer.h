#ifndef HeyTrack_Core_AbstractPlayer_h
#define HeyTrack_Core_AbstractPlayer_h
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

/** @file
 * @brief Class HeyTrack::Core::AbstractPlayer
 */

#include <QtCore/QObject>
#include <QtCore/QHash>

namespace HeyTrack { namespace Core {

/** @brief Player interface */
class AbstractPlayer: public QObject {
    Q_OBJECT

    public:
        static void registerPlayer(const QString& name, AbstractPlayer* (*instancer)(QObject*));
        static QList<QString> players();
        static AbstractPlayer* instance(const QString& name, QObject* parent = 0);

        inline AbstractPlayer(QObject* parent = 0): QObject(parent) {}

        virtual bool isPlaying() = 0;

    public slots:
        /**
         * @brief Play given URL
         * @param url       Stream URL
         */
        virtual void play(const QString& url) = 0;

        /**
         * @brief Stop playing
         */
        virtual void stop() = 0;

    private:
        static QHash<QString, AbstractPlayer* (*)(QObject*)> _players;
};

}}

#define PLAYER_DEFINE(class) public: \
    static inline AbstractPlayer* instancer(QObject* parent = 0) \
        { return new class(parent); } \
    private:

#define PLAYER_REGISTER(name, class) AbstractPlayer::registerPlayer(name, class::instancer);

#endif

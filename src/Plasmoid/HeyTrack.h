#ifndef HeyTrack_Plasmoid_HeyTrack_h
#define HeyTrack_Plasmoid_HeyTrack_h
/*
    Copyright © 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class Heytrack::Plasmoid::HeyTrack
 */

#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <Plasma/Applet>

#include "Core/Station.h"
#include "Core/Track.h"

namespace HeyTrack {

namespace Core {
    class AbstractServer;
}

namespace Plasmoid {

class HeyTrack: public Plasma::Applet {
    Q_OBJECT

    private:
        QSettings settings;
        QTimer* timer;

        Core::AbstractServer* server;
        Core::Station station;

        QString text;

    public:
        HeyTrack(QObject *parent, const QVariantList &args);
        virtual ~HeyTrack() {}

        virtual void init();
        virtual void paintInterface(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& contentsRect);

    private slots:
        /** @brief Request track info update */
        void getTrack();

        /**
         * @brief Update track info
         *
         * Called automatically after request from server. Updates track in
         * window, in tray hint and shows tray message.
         * @param t         Current track
         */
        void track(Core::Track t);

        /** @brief Error message */
        void error(const QString& message);
};

}}

#endif

#ifndef HeyTrack_Tray_HeyTrack_h
#define HeyTrack_Tray_HeyTrack_h
/*
    Copyright © 2010 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class HeyTrack::Tray::HeyTrack
 */

#include <QtCore/QSettings>
#include <QtGui/QWidget>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QCloseEvent>

#include <Core/Track.h>
#include <Core/Station.h>

class QNetworkAccessManager;
class QLabel;
class QTimer;
class QNetworkReply;

namespace HeyTrack {

namespace Core {
    class AbstractServer;
}

namespace Tray {

/** @brief Widget displaying currently played track in a window and in tray */
class HeyTrack: public QWidget {
    Q_OBJECT

    public:
        /** @brief Constructor */
        HeyTrack(QWidget* parent = 0);

    private:
        /**
         * @brief Event of closing window
         *
         * Window is not closed, but hidden in tray
         * @param event     Close event
         */
        inline virtual void closeEvent(QCloseEvent* event)
            { hide(); event->ignore(); }

        QTimer* timer;                  /**< @brief Timer for scheduling next update */
        QLabel* nowPlaying;             /**< @brief Now playing label in the window */
        Core::AbstractServer* server;   /**< @brief Stream server */
        Core::Station station;          /**< @brief Currently displayed station */
        QSystemTrayIcon* tray;          /**< @brief Tray icon */

        QSettings settings;             /**< @brief Settings */

    private slots:
        /** @brief Request track info update */
        void getUpdate();

        /**
         * @brief Update track info
         *
         * Called automatically after request from server. Updates track in
         * window, in tray hint and shows tray message.
         * @param t         Current track
         */
        void updateTrack(Core::Track t);

        /**
         * @brief Show / hide window
         *
         * Toggles visibility only after clicking on tray icon.
         * @param reason    Activation reason
         */
        void toggleVisibility(QSystemTrayIcon::ActivationReason reason);

        /**
         * @brief Open settings dialog
         *
         * Called after clicking on item of tray icon context menu.
         */
        void openSettings();
};

}}

#endif
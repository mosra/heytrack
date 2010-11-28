#ifndef HeyTrack_Tray_HeyTrack_h
#define HeyTrack_Tray_HeyTrack_h
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
 * @brief Class HeyTrack::Tray::HeyTrack
 */

#include <QtCore/QSettings>
#include <QtGui/QWidget>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QCloseEvent>

#include "Core/Track.h"
#include "Core/Station.h"
#include "Core/Format.h"

class QNetworkAccessManager;
class QLabel;
class QTimer;
class QPushButton;
class QNetworkReply;

namespace HeyTrack {

namespace Core {
    class AbstractServer;
    class AbstractPlayer;
}

namespace Tray {

/** @brief Widget displaying currently played track in a window and in tray */
class HeyTrack: public QWidget {
    Q_OBJECT

    public:
        /** @brief Constructor */
        HeyTrack(QWidget* parent = 0);

    private:
        QTimer* timer;                  /**< @brief Timer for scheduling next update */
        QLabel* nowPlaying;             /**< @brief Now playing label in the window */
        QPushButton* settingsButton;    /**< @brief Button for opening configuration */
        Core::AbstractServer* server;   /**< @brief Stream server */
        Core::AbstractPlayer* player;   /**< @brief Stream player */
        Core::Station station;          /**< @brief Currently displayed station */
        Core::Format format;            /**< @brief Stream format for current station */
        QSystemTrayIcon* tray;          /**< @brief Tray icon */

        QSettings settings;             /**< @brief Settings */

    private slots:
        /**
         * @brief Initialize
         *
         * Tries to get station ID and name from settings, if not present,
         * displays a button for opening configuration.
         */
        void initialize();

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

        /**
         * @brief Handle tray click
         *
         * Handles clicking on tray icon
         * @param reason    Activation reason
         */
        void handleTrayClick(QSystemTrayIcon::ActivationReason reason);

        /**
         * @brief Open settings dialog
         *
         * Called after clicking on item of tray icon context menu.
         */
        void openSettings();

        /** @brief Play current stream in player */
        void play();

        /** @brief Stop player */
        void stop();
};

}}

#endif

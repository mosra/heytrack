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

#include "HeyTrack.h"

#include <QtCore/QTimer>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QAction>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

#include "Core/AbstractServer.h"
#include "Core/AbstractPlayer.h"
#include "Core/Style.h"
#include "Settings/SettingsDialog.h"

namespace HeyTrack { namespace Tray {

using namespace Core;
using namespace Settings;

HeyTrack::HeyTrack(QWidget* parent): QWidget(parent), server(0), player(0) {
    settings.setIniCodec("UTF-8");

    /* Initalize style */
    new Style(":/icons.png", this);

    nowPlaying = new QLabel(tr("Initialization..."));
    settingsButton = new QPushButton(tr("Open settings"));
    connect(settingsButton, SIGNAL(clicked(bool)), SLOT(openSettings()));

    /* Layout */
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(settingsButton, 0, Qt::AlignCenter);
    layout->addWidget(nowPlaying, 0, Qt::AlignCenter);
    setLayout(layout);

    /* Initialize timer */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getTrack()));

    /* Tray icon */
    tray = new QSystemTrayIcon(this);
    tray->setIcon(Style::style()->bigIcon(Style::HeyTrack));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(handleTrayClick(QSystemTrayIcon::ActivationReason)));

    /* Tray icon context menu */
    QMenu* menu = new QMenu(this);
    menu->addAction(
        Style::style()->icon(Style::Settings),
        tr("Settings"), this, SLOT(openSettings()));
    menu->addAction(
        Style::style()->icon(Style::Play),
        tr("Play stream in player"), this, SLOT(play()));
    menu->addAction(
        Style::style()->icon(Style::Stop),
        tr("Stop player"), this, SLOT(stop()));
    menu->addAction(
        Style::style()->icon(Style::Exit),
        tr("Exit"), qApp, SLOT(quit()));
    tray->setContextMenu(menu);
    tray->show();

    /* Window icon, title, fixed size */
    setWindowIcon(Style::style()->bigIcon(Style::HeyTrack));
    setFixedSize(320,50);

    initialize();
}

void HeyTrack::initialize() {
    if(settings.contains("server") && settings.contains("station/id") && settings.contains("station/nick") && settings.contains("station/name") && settings.contains("format/id") && settings.contains("format/nick") && settings.contains("format/name")) {
        server = AbstractServer::instance(settings.value("server").toString(), qApp);
        connect(server, SIGNAL(track(Core::Track)), SLOT(track(Core::Track)));
        connect(server, SIGNAL(error(QString)), SLOT(error(QString)));

        player = AbstractPlayer::instance(settings.value("player").toString(), qApp);

        station = Station(settings.value("station/id").toUInt(),
                          settings.value("station/nick").toString(),
                          settings.value("station/name").toString());

        format = Format(settings.value("format/id").toUInt(),
                        settings.value("format/nick").toString(),
                        settings.value("format/name").toString());

        settingsButton->setVisible(false);
        nowPlaying->setVisible(true);

        setWindowTitle(tr("%0: now playing").arg(station.name()));

        /* Request track update */
        timer->start(1000);

    /* Display button instead of "Initialization..." label */
    } else {
        settingsButton->setVisible(true);
        nowPlaying->setVisible(false);

        setWindowTitle("HeyTrack");
        tray->setToolTip("HeyTrack");

        /* Stop all track updates */
        timer->stop();
    }
}

void HeyTrack::getTrack() { server->getTrack(station); }

void HeyTrack::track(Track t) {
    /* Update window label */
    nowPlaying->setText("<strong>" + t.artist() + "</strong> - " + t.title());

    /* Tray icon message */
    tray->showMessage(tr("%0: now playing").arg(station.name()), t.artist() + " - " + t.title());
    tray->setToolTip(tr("%0: ").arg(station.name()) + t.artist() + " - " + t.title());

    /* Next update after 20 seconds */
    timer->start(20*1000);
}

void HeyTrack::error(const QString& message) {
    tray->setToolTip("");
    tray->showMessage(tr("Server error"), message);
    nowPlaying->setText("<strong>Server error:</strong> " + message);

    /* Wait a minute... */
    timer->start(60*1000);
}

void HeyTrack::handleTrayClick(QSystemTrayIcon::ActivationReason reason) {
    switch(reason) {
        /* Left click -> toggle visibility */
        case QSystemTrayIcon::Trigger:
            isHidden() ? show() : hide();
            break;
        /* Middle click -> toggle playing */
        case QSystemTrayIcon::MiddleClick:
            player->isPlaying() ? stop() : play();
            break;
    }
}

void HeyTrack::openSettings() {
    SettingsDialog dialog(&settings, &server, &player, this);
    if(dialog.exec() == QDialog::Accepted) initialize();
}

void HeyTrack::play() {
    if(!player || !server) return;
    player->play(server->streamUrl(station, format));
}

void HeyTrack::stop() {
    if(!player) return;
    player->stop();
}

}}

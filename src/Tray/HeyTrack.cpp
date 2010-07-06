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

#include "HeyTrack.h"

#include <QtCore/QTimer>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QAction>
#include <QtGui/QStyle>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

#include "Core/AbRadioServer.h"

#include "HeySettings.h"

namespace HeyTrack { namespace Tray {

using namespace Core;

HeyTrack::HeyTrack(QWidget* parent): QWidget(parent) {
    /* Layout */
    nowPlaying = new QLabel(tr("Initialization..."));
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(nowPlaying, 0, Qt::AlignCenter);
    setLayout(layout);

    /* Initialize server */
    server = new AbRadioServer(this);
    connect(server, SIGNAL(track(Core::Track)), SLOT(updateTrack(Core::Track)));
    /** @todo This is UGLY HACK! */
    station = Station(36, "RockRádio Prácheň");

    /* Initialize timer and schedule first update one second after start */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getUpdate()));
    timer->start(1000);

    /* Tray icon */
    tray = new QSystemTrayIcon(this);
    tray->setIcon(style()->standardIcon(QStyle::SP_MediaStop).pixmap(16,16));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(toggleVisibility(QSystemTrayIcon::ActivationReason)));

    /* Tray icon context menu */
    QMenu* menu = new QMenu(this);
    menu->addAction(
        style()->standardIcon(QStyle::SP_DriveFDIcon).pixmap(16,16),
        tr("Settings"), this, SLOT(openSettings()));
    menu->addAction(
        style()->standardIcon(QStyle::SP_DialogCloseButton).pixmap(16,16),
        tr("Exit"), qApp, SLOT(quit()));
    tray->setContextMenu(menu);
    tray->show();

    /* Window icon, title, fixed size */
    setWindowTitle(tr("%0: now playing").arg(station.name()));
    setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
    setFixedSize(320,50);
}

void HeyTrack::getUpdate() { server->getTrack(station); }

void HeyTrack::updateTrack(Track t) {
    /* Update window label */
    nowPlaying->setText("<strong>" + t.artist() + "</strong> - " + t.title());

    /* Tray icon message */
    tray->showMessage(tr("%0: now playing").arg(station.name()), t.artist() + " - " + t.title());
    tray->setIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
    tray->setToolTip(tr("%0: ").arg(station.name()) + t.artist() + " - " + t.title());

    /* Next update after 20 seconds */
    timer->start(20*1000);
}

void HeyTrack::toggleVisibility(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger)
        isHidden() ? show() : hide();
}

void HeyTrack::openSettings() {
    HeySettings window(&settings);
    window.exec();
}

}}

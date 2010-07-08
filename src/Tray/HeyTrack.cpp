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
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>

#include "Core/AbRadioServer.h"
#include "Settings/SettingsDialog.h"

namespace HeyTrack { namespace Tray {

using namespace Core;
using namespace Settings;

HeyTrack::HeyTrack(QWidget* parent): QWidget(parent) {
    nowPlaying = new QLabel(tr("Initialization..."));
    settingsButton = new QPushButton(tr("Open settings"));
    connect(settingsButton, SIGNAL(clicked(bool)), SLOT(openSettings()));

    /* Layout */
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(settingsButton, 0, Qt::AlignCenter);
    layout->addWidget(nowPlaying, 0, Qt::AlignCenter);
    setLayout(layout);

    /* Initialize server */
    server = new AbRadioServer(this);
    connect(server, SIGNAL(track(Core::Track)), SLOT(track(Core::Track)));

    /* Initialize timer */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getTrack()));

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
    setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
    setFixedSize(320,50);

    initialize();
}

void HeyTrack::initialize() {
    if(settings.contains("station/id") && settings.contains("station/name")) {
        station = Station(settings.value("station/id").toUInt(), settings.value("station/name").toString());

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
    SettingsDialog dialog(&settings);
    if(dialog.exec() == QDialog::Accepted) initialize();
}

}}

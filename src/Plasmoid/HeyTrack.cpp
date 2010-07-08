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

#include <QtGui/QApplication>
#include <QtGui/QPainter>
#include <Plasma/Theme>

#include "Core/AbRadioServer.h"

#include "HeyTrack.moc"

namespace HeyTrack { namespace Plasmoid {

using namespace Core;

HeyTrack::HeyTrack(QObject* parent, const QVariantList& args): Applet(parent, args) {
    /* Set up QApplication save */
    qApp->setApplicationName("HeyTrack");
    qApp->setOrganizationName("Mosra");

    settings = new QSettings();

    setBackgroundHints(DefaultBackground);

    /* Initialize server */
    server = new AbRadioServer(this);
    connect(server, SIGNAL(track(Core::Track)), SLOT(track(Core::Track)));
    connect(server, SIGNAL(error(QString)), SLOT(error(QString)));

    /* Initialize timer */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getTrack()));
}

void HeyTrack::init() {
    if(!settings->contains("station/id") || !settings->contains("station/name"))
        setFailedToLaunch(true, tr("Required settings not available"));

    station = Station(settings->value("station/id").toUInt(), settings->value("station/name").toString());

    text = tr("Initializing...");
    timer->start(1000);
}

void HeyTrack::paintInterface(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& contentsRect) {
    Q_UNUSED(option);

    painter->save();
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    painter->drawText(contentsRect, Qt::AlignCenter, text);
    painter->restore();
}

void HeyTrack::getTrack() { server->getTrack(station); }

void HeyTrack::track(Track t) {
    /* Update window label */
    text = t.artist() + " - " + t.title();

    /* Next update after 20 seconds */
    timer->start(20*1000);

    update();
}

void HeyTrack::error(const QString& message) {
    text = tr("Server error: %0").arg(message);

    /* Wait a minute... */
    timer->start(60*1000);

    update();
}

K_EXPORT_PLASMA_APPLET(heytrack, HeyTrack);

}}

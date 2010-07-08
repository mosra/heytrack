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

#include <QtGui/QPainter>
#include <Plasma/Theme>

#include "HeyTrack.moc"

namespace HeyTrack { namespace Plasmoid {

HeyTrack::HeyTrack(QObject* parent, const QVariantList& args): Applet(parent, args) {
    setBackgroundHints(DefaultBackground);
}

void HeyTrack::init() {

}

void HeyTrack::paintInterface(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& contentsRect) {
    Q_UNUSED(option);

    painter->save();
    painter->setPen(Plasma::Theme::defaultTheme()->color(Plasma::Theme::TextColor));
    painter->drawText(contentsRect, Qt::AlignCenter, tr("Initializing..."));
    painter->restore();
}

K_EXPORT_PLASMA_APPLET(heytrack, HeyTrack);

}}

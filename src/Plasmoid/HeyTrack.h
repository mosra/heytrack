#ifndef HeyTrack_Plasmoid_HeyTrack_h
#define HeyTrack_Plasmoid_HeyTrack_h
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
 * @brief Class Heytrack::Plasmoid::HeyTrack
 */

#include <Plasma/Applet>

namespace HeyTrack { namespace Plasmoid {

class HeyTrack: public Plasma::Applet {
    public:
        HeyTrack(QObject *parent, const QVariantList &args);
        virtual ~HeyTrack() {}

        virtual void init();
        virtual void paintInterface(QPainter* painter, const QStyleOptionGraphicsItem* option, const QRect& contentsRect);
};

}}

#endif

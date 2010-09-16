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

#include "Style.h"

namespace HeyTrack { namespace Core {

Style* Style::object = 0;

Style::Style(const QString& file, QObject* parent): QObject(parent), iconFile(file) {
    object = this;
}

const QIcon& Style::icon(int icon, unsigned int size) {
    /* Load the icon if doesn't exist yet */
    if(!icons.contains(icon))
        icons[icon] = iconFile.copy(((int) icon & 0x0F)*size, ((int) icon & 0xF0)*size/16, size, size);

    return icons[icon];
}

}}

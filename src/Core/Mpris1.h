#ifndef Mpris1_h
#define Mpris1_h
/*
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
 * @brief Mpris 1.0 related structures and methods
 */
#include <QtDBus/QDBusMetaType>

namespace Mpris1 {
    struct Status {
        int play; //Playing = 0, Paused = 1, Stopped = 2
        int random; //Linearly = 0, Randomly = 1
        int repeat; //Go_To_Next = 0, Repeat_Current = 1
        int repeatPlaylist; //Stop_When_Finished = 0, Never_Give_Up_Playing = 1
    };
}

Q_DECLARE_METATYPE(Mpris1::Status)

QDBusArgument& operator<<(QDBusArgument &argument, const Mpris1::Status &status) {
    argument.beginStructure();
    argument << status.play << status.random << status.repeat << status.repeatPlaylist;
    argument.endStructure();
    return argument;
}

const QDBusArgument& operator>>(const QDBusArgument &argument, Mpris1::Status &status) {
    argument.beginStructure();
    argument >> status.play >> status.random >> status.repeat >> status.repeatPlaylist;
    argument.endStructure();
    return argument;
}

#endif

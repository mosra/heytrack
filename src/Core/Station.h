#ifndef HeyTrack_Core_Station_h
#define HeyTrack_Core_Station_h
/*
    Copyright © 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class HeyTrack::Core::Station
 */

#include <QtCore/QString>

namespace HeyTrack { namespace Core {

/** @brief Radio station */
class Station {
    private:
        unsigned int _id;
        QString _nick, _name;

    public:
        /** @brief Station ID */
        inline unsigned int id() const { return _id; }

        /** @brief Station nick */
        inline QString nick() const { return _nick; }

        /** @brief Station name */
        inline QString name() const { return _name; }

        /**
         * @brief Implicit constructor
         *
         * Creates invalid station.
         */
        inline Station(): _id(0) {}

        /**
         * @brief Constructor
         *
         * @param __id      Station ID
         * @param __nick    Station nick
         * @param __name    Station name
         */
        inline Station(unsigned int __id, const QString& __nick, const QString& __name):
            _id(__id), _nick(__nick), _name(__name) {}
};

}}

#endif

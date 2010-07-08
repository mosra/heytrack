#ifndef HeyTrack_Core_Genre_h
#define HeyTrack_Core_Genre_h
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
 * @brief Class HeyTrack::Core::Genre
 */

#include <QtCore/QString>

namespace HeyTrack { namespace Core {

/** @brief Music genre */
class Genre {
    private:
        unsigned int _id;
        QString _name;

    public:
        /** @brief Genre ID */
        inline unsigned int id() const { return _id; }

        /** @brief Genre name */
        inline QString name() const { return _name; }

        /**
         * @brief Implicit constructor
         *
         * Creates invalid genre.
         */
        inline Genre(): _id(0) {}

        /**
         * @brief Constructor
         *
         * @param __id      Genre ID
         * @param __name    Genre name
         */
        inline Genre(unsigned int __id, const QString& __name): _id(__id), _name(__name) {}
};

}}

#endif

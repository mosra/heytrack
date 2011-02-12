#ifndef HeyTrack_Core_Format_h
#define HeyTrack_Core_Format_h
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
 * @brief Class HeyTrack::Core::Format
 */

#include <QtCore/QString>

namespace HeyTrack { namespace Core {

class Format {
    public:
        inline unsigned int id() const { return _id; }
        inline QString nick() const { return _nick; }
        inline QString name() const { return _name; }

        inline Format(): _id(0) {}

        inline Format(unsigned int __id, const QString& __nick, const QString& __name):
            _id(__id), _nick(__nick), _name(__name) {}

    private:
        unsigned int _id;
        QString _nick, _name;
};

}}

#endif

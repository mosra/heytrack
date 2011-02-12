#ifndef HeyTrack_Core_Track_h
#define HeyTrack_Core_Track_h
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
 * @brief Class HeyTrack::Core::Track
 */

#include <QtCore/QString>

namespace HeyTrack { namespace Core {

/**
 * @brief Track
 *
 * Note that some servers doesn't implement all available fields (mainly times
 * and album name).
 */
class Track {
    private:
        QString _artist,
            _album,
            _title;
        unsigned int _time,
            _timeElapsed,
            _timeRemaining;

    public:
        /** @brief Track artist */
        inline QString artist() const { return _artist; }

        /** @brief Track album */
        inline QString album() const { return _album; }

        /** @brief Track title */
        inline QString title() const { return _title; }

        /** @brief Track time */
        inline unsigned int time() const { return _time; }

        /** @brief Track time elapsed */
        inline unsigned int timeElapsed() const { return _timeElapsed; }

        /**
         * @brief Track time remaining
         *
         * Useful for scheduling next track update.
         */
        inline unsigned int timeRemaining() const { return _timeRemaining; }

        /**
         * @brief Implicit constructor
         *
         * Creates invalid track.
         */
        inline Track(): _time(0), _timeElapsed(0), _timeRemaining(0) {}

        /**
         * @brief Constructor
         *
         * @param __artist          Artist
         * @param __title           Title
         * @param __album           Album
         * @param __time            Time
         * @param __timeElapsed     Time elapsed
         * @param __timeRemaining   Time remaining
         */
        inline Track(const QString& __artist, const QString& __title, const QString& __album = QString(), unsigned int __time = 0, unsigned int __timeElapsed = 0, unsigned int __timeRemaining = 0): _artist(__artist), _album(__album), _title(__title), _time(__time), _timeElapsed(__timeElapsed), _timeRemaining(__timeRemaining) {}
};

}}

#endif

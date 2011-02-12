#ifndef HeyTrack_Tray_Style_h
#define HeyTrack_Tray_Style_h
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

#include <QtCore/QHash>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>

namespace HeyTrack { namespace Core {

/**
 * @brief Global access to icons
 */
class Style: public QObject {
    public:
        /**
         * @brief Icons
         *
         * Every icon is 64x64 pixels, OxF0 part means row, 0x0F column.
         */
        enum Icon {
            PlayTray    = 0x03,     /**< @brief Play state in tray icon */
            StopTray    = 0x13,     /**< @brief Stop state in tray icon */
            Exit        = 0x23,     /**< @brief Exit icon in tray context menu */
            Play        = 0x30,     /**< @brief Play icon in tray context menu */
            Stop        = 0x31,     /**< @brief Stop icon in tray context menu */
            Settings    = 0x32,     /**< @brief Settings icon in tray context menu */
        };

        /**
         * @brief Big icons
         *
         * Size: 192x192 px.
         */
        enum BigIcon {
            HeyTrack    = 0x00      /**< @brief HeyTrack logo */
        };

        /**
         * @brief Constructor
         * @param file          Icon file
         * @param parent        Parent object
         */
        Style(const QString& file, QObject* parent = 0);

        /** @brief Signleton object */
        static inline Style* style() { return object; }

        /**
         * @brief Get icon
         * @param _icon         Icon ID
         */
        inline const QIcon& icon(Icon _icon) { return icon(_icon, 64); }

        /**
         * @brief Get big icon
         * @param _icon         Icon ID
         */
        const QIcon& bigIcon(BigIcon _icon) { return icon(_icon, 192); }

    private:
        static Style* object;       /**< @brief Singleton object */
        QHash<int, QIcon> icons;    /**< @brief Icons */
        QPixmap iconFile;           /**< @brief Icons pixmap */

        const QIcon& icon(int icon, unsigned int size);
};

}}

#endif

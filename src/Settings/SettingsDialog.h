#ifndef HeyTrack_Settings_SettingsDialog_h
#define HeyTrack_Settings_SettingsDialog_h
/*
    Copyright © 2010 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class HeyTrack::Settings::SettingsDialog
 */

#include <QtGui/QDialog>

class QCheckBox;
class QSettings;
class QLineEdit;

namespace HeyTrack { namespace Settings {

/** @brief Settings dialog */
class SettingsDialog: public QDialog {
    Q_OBJECT

    public:
        /**
         * @brief Constructor
         *
         * @param _settings   Settings
         */
        SettingsDialog(QSettings* _settings);

    private:
        QSettings* settings;        /**< @brief Settings */

    private slots:
        virtual void accept();      /**< @brief Accept dialog */
};

}}

#endif

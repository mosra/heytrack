#ifndef HeyTrack_Settings_SettingsDialog_h
#define HeyTrack_Settings_SettingsDialog_h
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
 * @brief Class HeyTrack::Settings::SettingsDialog
 */

#include <QtGui/QDialog>

#include "Core/Genre.h"
#include "Core/Station.h"
#include "Core/Format.h"

class QComboBox;
class QCheckBox;
class QSettings;
class QLineEdit;

namespace HeyTrack {

namespace Core {
    class AbstractServer;
}

namespace Settings {

/** @brief Settings dialog */
class SettingsDialog: public QDialog {
    Q_OBJECT

    private:
        Core::AbstractServer** server;

    public:
        /**
         * @brief Constructor
         *
         * @param _settings   Settings
         */
        SettingsDialog(QSettings* _settings, Core::AbstractServer** _server, QWidget* parent = 0);

    private:
        QSettings* settings;        /**< @brief Settings */

        QComboBox *servers,
            *genres,
            *stations,
            *formats;

    private slots:
        virtual void accept();      /**< @brief Accept dialog */

    private slots:
        void setServer(const QString& name);
        void getStations();
        void getFormats();
        void updateGenres(const QList<Core::Genre>& _genres);
        void updateStations(const QList<Core::Station>& _stations);
        void updateFormats(const QList<Core::Format>& _formats);
        void error(const QString& message);
};

}}

#endif

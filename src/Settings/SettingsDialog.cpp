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

#include "SettingsDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QVBoxLayout>

namespace HeyTrack { namespace Settings {

SettingsDialog::SettingsDialog(QSettings* _settings): settings(_settings) {
    setWindowTitle(tr("HeyTrack settings"));

    /* Buttons */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(buttons);
    setLayout(layout);
}

void SettingsDialog::accept() {
    done(QDialog::Accepted);
}

}}

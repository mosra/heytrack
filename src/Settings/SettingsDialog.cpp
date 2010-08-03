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

#include "SettingsDialog.h"

#include <QtCore/QSettings>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>

#include "Core/AbRadioServer.h"
#include "Core/GenreModel.h"
#include "Core/StationModel.h"

namespace HeyTrack { namespace Settings {

using namespace Core;

SettingsDialog::SettingsDialog(QSettings* _settings, QWidget* parent): QDialog(parent), settings(_settings) {
    setWindowTitle(tr("HeyTrack settings"));

    /* Initialize server */
    server = new AbRadioServer;
    connect(server, SIGNAL(genres(QList<Core::Genre>)), SLOT(updateGenres(QList<Core::Genre>)));
    connect(server, SIGNAL(stations(QList<Core::Station>)), SLOT(updateStations(QList<Core::Station>)));
    connect(server, SIGNAL(error(QString)), SLOT(error(QString)));

    /* Initialize comboboxes */
    servers = new QComboBox;
    servers->addItem(server->name());
    genres = new QComboBox;
    connect(genres, SIGNAL(currentIndexChanged(int)), SLOT(getStations()));
    stations = new QComboBox;

    /* Get genre list for the default server */
    server->getGenres();

    /* Buttons */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Server:")), 0, 0);
    layout->addWidget(servers, 0, 1);
    layout->addWidget(new QLabel(tr("Genre:")), 1, 0);
    layout->addWidget(genres, 1, 1);
    layout->addWidget(new QLabel(tr("Station:")), 2, 0);
    layout->addWidget(stations, 2, 1);
    layout->addWidget(buttons, 3, 0, 1, 2);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    setLayout(layout);
}

void SettingsDialog::accept() {
    settings->setValue("genre/id", genres->itemData(genres->currentIndex()));

    settings->setValue("station/id", stations->itemData(stations->currentIndex(), StationModel::Id));
    settings->setValue("station/nick", stations->itemData(stations->currentIndex(), StationModel::Nick));
    settings->setValue("station/name", stations->currentText());

    done(QDialog::Accepted);
}

void SettingsDialog::getStations() {
    server->getStations(qobject_cast<GenreModel*>(genres->model())->genre(genres->currentIndex()));
}

void SettingsDialog::updateGenres(const QList<Genre>& _genres) {
    GenreModel* m = new GenreModel(_genres, this);
    genres->clear();
    genres->setModel(m);

    /* Set genre to user saved */
    if(settings->contains("genre/id"))
        genres->setCurrentIndex(genres->findData(settings->value("genre/id", 0)));
}

void SettingsDialog::updateStations(const QList<Station>& _stations) {
    StationModel* m = new StationModel(_stations, this);
    stations->clear();
    stations->setModel(m);

    /* Set station to user saved */
    if(settings->contains("station/id"))
        stations->setCurrentIndex(stations->findData(settings->value("station/id", 0), StationModel::Id));
}

void SettingsDialog::error(const QString& message) {
    QMessageBox* b = new QMessageBox(this);
    b->setWindowTitle(tr("Server error"));
    b->setText(tr("An error occured during parsing server response:"));
    b->setInformativeText(message);
    b->setStandardButtons(QMessageBox::Ok);
    b->exec();
}

}}

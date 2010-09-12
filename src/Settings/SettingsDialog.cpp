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
#include <QtGui/QApplication>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QGridLayout>
#include <QtGui/QComboBox>
#include <QtGui/QLabel>
#include <QtGui/QMessageBox>

#include "Core/AbRadioServer.h"
#include "Core/GenreModel.h"
#include "Core/StationModel.h"
#include "Core/FormatModel.h"
#include "Core/AbstractPlayer.h"

namespace HeyTrack { namespace Settings {

using namespace Core;

SettingsDialog::SettingsDialog(QSettings* _settings, AbstractServer** _server, AbstractPlayer** _player, QWidget* parent): QDialog(parent), settings(_settings), server(_server), player(_player) {
    setWindowTitle(tr("HeyTrack settings"));

    /* Initialize comboboxes */
    servers = new QComboBox;
    servers->addItems(AbstractServer::servers());
    players = new QComboBox;
    players->addItems(AbstractPlayer::players());
    genres = new QComboBox;
    stations = new QComboBox;
    formats = new QComboBox;

    /* Initialize server */
    if(settings->contains("server"))
        servers->setCurrentIndex(servers->findText(settings->value("server").toString()));
    setServer(settings->value("server", servers->itemText(0)).toString());

    connect(servers, SIGNAL(currentIndexChanged(QString)), SLOT(setServer(QString)));

    /* Get genre list for the default server */
    (*server)->getGenres();

    /* Initialize player */
    if(settings->contains("player"))
        players->setCurrentIndex(players->findText(settings->value("player").toString()));
    setPlayer(settings->value("player", players->itemText(0)).toString());

    /* Buttons */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(new QLabel(tr("Server:")), 0, 0);
    layout->addWidget(servers, 0, 1);
    layout->addWidget(new QLabel(tr("Player:")), 1, 0);
    layout->addWidget(players, 1, 1);
    layout->addWidget(new QLabel(tr("Genre:")), 2, 0);
    layout->addWidget(genres, 2, 1);
    layout->addWidget(new QLabel(tr("Station:")), 3, 0);
    layout->addWidget(stations, 3, 1);
    layout->addWidget(new QLabel(tr("Stream format:")), 4, 0);
    layout->addWidget(formats, 4, 1);
    layout->addWidget(buttons, 5, 0, 1, 2);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 1);
    setLayout(layout);

    setMinimumWidth(320);
}

void SettingsDialog::accept() {
    settings->setValue("server", servers->currentText());
    settings->setValue("player", players->currentText());

    settings->setValue("genre/id", genres->itemData(genres->currentIndex()));

    settings->setValue("station/id", stations->itemData(stations->currentIndex(), StationModel::Id));
    settings->setValue("station/nick", stations->itemData(stations->currentIndex(), StationModel::Nick));
    settings->setValue("station/name", stations->currentText());

    settings->setValue("format/id", formats->itemData(formats->currentIndex(), FormatModel::Id));
    settings->setValue("format/nick", formats->itemData(formats->currentIndex(), FormatModel::Nick));
    settings->setValue("format/name", formats->currentText());

    done(QDialog::Accepted);
}

void SettingsDialog::setServer(const QString& name) {
    if(*server) delete *server;
    (*server) = AbstractServer::instance(name, qApp);

    connect(*server, SIGNAL(genres(QList<Core::Genre>)), SLOT(updateGenres(QList<Core::Genre>)));
    connect(*server, SIGNAL(stations(QList<Core::Station>)), SLOT(updateStations(QList<Core::Station>)));
    connect(*server, SIGNAL(formats(QList<Core::Format>)), SLOT(updateFormats(QList<Core::Format>)));
    connect(*server, SIGNAL(error(QString)), SLOT(error(QString)));

    /* Clear genres, stations and formats for new list */
    genres->clear();
    stations->clear();
    formats->clear();

    (*server)->getGenres();
}

void SettingsDialog::setPlayer(const QString& name) {
    if(*player) delete *player;
    (*player) = AbstractPlayer::instance(name, qApp);
}

void SettingsDialog::getStations() {
    (*server)->getStations(qobject_cast<GenreModel*>(genres->model())->genre(genres->currentIndex()));
}

void SettingsDialog::getFormats() {
    (*server)->getFormats(qobject_cast<StationModel*>(stations->model())->station(stations->currentIndex()));
}

void SettingsDialog::updateGenres(const QList<Genre>& _genres) {
    disconnect(genres, SIGNAL(currentIndexChanged(int)), this, SLOT(getStations()));

    GenreModel* m = new GenreModel(_genres, this);
    genres->clear();
    genres->setModel(m);

    connect(genres, SIGNAL(currentIndexChanged(int)), SLOT(getStations()));

    /* Set genre to user saved */
    if(settings->contains("genre/id"))
        genres->setCurrentIndex(genres->findData(settings->value("genre/id", 0)));
}

void SettingsDialog::updateStations(const QList<Station>& _stations) {
    disconnect(stations, SIGNAL(currentIndexChanged(int)), this, SLOT(getFormats()));

    StationModel* m = new StationModel(_stations, this);
    stations->clear();
    stations->setModel(m);

    connect(stations, SIGNAL(currentIndexChanged(int)), SLOT(getFormats()));

    /* Set station to user saved */
    if(settings->contains("station/id") && settings->value("genre/id") == genres->itemData(genres->currentIndex()))
        stations->setCurrentIndex(stations->findData(settings->value("station/id", 0), StationModel::Id));
}

void SettingsDialog::updateFormats(const QList<Format>& _formats) {
    /** @bug Replies doesn't always come in the same order as requests -
            fill the combobox with only the last requested reply */
    FormatModel* m = new FormatModel(_formats, this);
    formats->clear();
    formats->setModel(m);

    /* Set format to user saved */
    if(settings->contains("format/id") && settings->value("station/id") == stations->itemData(stations->currentIndex()))
        formats->setCurrentIndex(formats->findData(settings->value("format/id", 0), FormatModel::Id));
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

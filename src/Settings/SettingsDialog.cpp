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
#include <QtGui/QPushButton>
#include <QtGui/QLineEdit>

#include "Core/AbRadioServer.h"
#include "Core/GenreModel.h"
#include "Core/StationModel.h"
#include "Core/FormatModel.h"
#include "Core/AbstractPlayer.h"
#include "Core/Style.h"

namespace HeyTrack { namespace Settings {

using namespace Core;

SettingsDialog::SettingsDialog(QSettings* _settings, AbstractServer** _server, AbstractPlayer** _player, QWidget* parent): QDialog(parent), settings(_settings), server(_server), selectedServer(0), player(_player), selectedPlayer(0) {
    setWindowTitle(tr("HeyTrack settings"));
    setWindowIcon(Style::style()->icon(Style::Settings));

    /* Initialize data fields */
    servers = new QComboBox;
    servers->addItems(AbstractServer::servers());
    players = new QComboBox;
    players->addItems(AbstractPlayer::players());
    genres = new QComboBox;
    stations = new QComboBox;
    formats = new QComboBox;
    streamUrl = new QLineEdit;
    streamUrl->setReadOnly(true);

    /* Initialize server (and get genre list) */
    if(settings->contains("server"))
        servers->setCurrentIndex(servers->findText(settings->value("server").toString()));
    setServer(settings->value("server", servers->itemText(0)).toString());

    connect(servers, SIGNAL(currentIndexChanged(QString)), SLOT(setServer(QString)));

    /* Initialize player */
    if(settings->contains("player"))
        players->setCurrentIndex(players->findText(settings->value("player").toString()));
    setPlayer(settings->value("player", players->itemText(0)).toString());

    /* Enable accept button only if required comboboxes have valid indexes */
    connect(servers, SIGNAL(currentIndexChanged(int)), SLOT(checkAcceptable()));
    connect(players, SIGNAL(currentIndexChanged(int)), SLOT(checkAcceptable()));
    connect(genres, SIGNAL(currentIndexChanged(int)), SLOT(checkAcceptable()));
    connect(stations, SIGNAL(currentIndexChanged(int)), SLOT(checkAcceptable()));
    connect(formats, SIGNAL(currentIndexChanged(int)), SLOT(checkAcceptable()));

    /* Label with HeyTrack logo */
    QLabel* logoLabel = new QLabel;
    logoLabel->setPixmap(Style::style()->bigIcon(Style::HeyTrack).pixmap(128, 128));
    logoLabel->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    logoLabel->setMinimumWidth(140);

    /* Label for stream URL */
    QLabel* streamUrlLabel = new QLabel(tr("Stream URL: "
        "<em>If you haven't supported player or no player is working, "
        "you can try to play this URL manually.</em>"));
    streamUrlLabel->setWordWrap(true);
    streamUrlLabel->setAlignment(Qt::AlignJustify|Qt::AlignVCenter);

    /* Buttons */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
    acceptButton = buttons->button(QDialogButtonBox::Save);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(logoLabel, 0, 0, 6, 1);
    layout->addWidget(new QLabel(tr("Player:")), 0, 1);
    layout->addWidget(players, 0, 2);
    layout->addWidget(new QLabel(tr("Server:")), 1, 1);
    layout->addWidget(servers, 1, 2);
    layout->addWidget(new QLabel(tr("Genre:")), 2, 1);
    layout->addWidget(genres, 2, 2);
    layout->addWidget(new QLabel(tr("Station:")), 3, 1);
    layout->addWidget(stations, 3, 2);
    layout->addWidget(new QLabel(tr("Stream format:")), 4, 1);
    layout->addWidget(formats, 4, 2);
    layout->addWidget(streamUrlLabel, 5, 1, 1, 2);
    layout->addWidget(streamUrl, 6, 1, 1, 2);
    layout->addWidget(buttons, 7, 0, 1, 3);
    layout->setColumnStretch(0, 0);
    layout->setColumnStretch(1, 0);
    layout->setColumnStretch(2, 1);
    setLayout(layout);

    setMinimumWidth(420);
}

void SettingsDialog::checkAcceptable() {
    /* Everything must be selected */
    if(servers->currentIndex() == -1 ||
       players->currentIndex() == -1 ||
       genres->currentIndex() == -1 ||
       stations->currentIndex() == -1 ||
       formats->currentIndex() == -1)
        acceptButton->setDisabled(true);

    else acceptButton->setEnabled(true);
}

void SettingsDialog::accept() {
    /* Set global server to selected */
    if(*server) delete *server;
    *server = selectedServer;

    /* Set global player to selected */
    if(*player) delete *player;
    *player = selectedPlayer;

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
    if(selectedServer) delete selectedServer;
    selectedServer = AbstractServer::instance(name, qApp);

    connect(selectedServer, SIGNAL(genres(QList<Core::Genre>)), SLOT(updateGenres(QList<Core::Genre>)));
    connect(selectedServer, SIGNAL(stations(QList<Core::Station>)), SLOT(updateStations(QList<Core::Station>)));
    connect(selectedServer, SIGNAL(formats(QList<Core::Format>)), SLOT(updateFormats(QList<Core::Format>)));
    connect(selectedServer, SIGNAL(error(QString)), SLOT(error(QString)));

    /* Clear subsequent fields for new list */
    genres->clear();
    stations->clear();
    formats->clear();
    streamUrl->clear();

    selectedServer->getGenres();
}

void SettingsDialog::setPlayer(const QString& name) {
    if(selectedPlayer) delete selectedPlayer;
    selectedPlayer = AbstractPlayer::instance(name, qApp);
}

void SettingsDialog::setGenre(int index) {
    stations->clear();
    formats->clear();
    streamUrl->clear();

    if(index != -1)
        selectedServer->getStations(qobject_cast<GenreModel*>(genres->model())->genre(index));
}

void SettingsDialog::setStation(int index) {
    formats->clear();
    streamUrl->clear();

    if(index != -1)
        selectedServer->getFormats(qobject_cast<StationModel*>(stations->model())->station(index));
}

void SettingsDialog::setFormat(int index) {
    if(index == -1) return;

    streamUrl->setText(selectedServer->streamUrl(
        qobject_cast<StationModel*>(stations->model())->station(index),
        qobject_cast<FormatModel*>(formats->model())->format(index)));
}

void SettingsDialog::updateGenres(const QList<Genre>& _genres) {
    disconnect(genres, SIGNAL(currentIndexChanged(int)), this, SLOT(setGenre(int)));

    GenreModel* m = new GenreModel(_genres, genres);
    genres->setModel(m);

    /* Set genre to user saved */
    if(settings->contains("genre/id") && settings->value("server") == servers->currentText())
        genres->setCurrentIndex(genres->findData(settings->value("genre/id", 0)));
    setGenre(genres->currentIndex());

    connect(genres, SIGNAL(currentIndexChanged(int)), SLOT(setGenre(int)));
}

void SettingsDialog::updateStations(const QList<Station>& _stations) {
    disconnect(stations, SIGNAL(currentIndexChanged(int)), this, SLOT(setStation(int)));

    StationModel* m = new StationModel(_stations, stations);
    stations->setModel(m);

    /* Set station to user saved */
    if(settings->contains("station/id") && settings->value("genre/id") == genres->itemData(genres->currentIndex()))
        stations->setCurrentIndex(stations->findData(settings->value("station/id", 0), StationModel::Id));
    setStation(stations->currentIndex());

    connect(stations, SIGNAL(currentIndexChanged(int)), SLOT(setStation(int)));
}

void SettingsDialog::updateFormats(const QList<Format>& _formats) {
    disconnect(formats, SIGNAL(currentIndexChanged(int)), this, SLOT(setFormat(int)));

    /** @bug Replies doesn't always come in the same order as requests -
            fill the combobox with only the last requested reply */
    FormatModel* m = new FormatModel(_formats, formats);
    formats->setModel(m);

    /* Set format to user saved */
    if(settings->contains("format/id") && settings->value("station/id") == stations->itemData(stations->currentIndex()))
        formats->setCurrentIndex(formats->findData(settings->value("format/id", 0), FormatModel::Id));
    setFormat(formats->currentIndex());

    connect(formats, SIGNAL(currentIndexChanged(int)), SLOT(setFormat(int)));
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

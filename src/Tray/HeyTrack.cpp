#include "HeyTrack.h"

#include <QtCore/QTimer>
#include <QtCore/QFile>
#include <QtCore/QProcess>
#include <QtCore/QUrl>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QMenu>
#include <QtGui/QApplication>
#include <QtGui/QCloseEvent>
#include <QtGui/QAction>
#include <QtGui/QStyle>
#include <QtGui/QLabel>
#include <QtGui/QHBoxLayout>

#include "Core/AbRadioServer.h"

#include "HeySettings.h"

namespace HeyTrack { namespace Tray {

using namespace Core;

/* Konstruktor */
HeyTrack::HeyTrack(QWidget* parent): QWidget(parent) {
    /* Titulek a ikona okna */
    setWindowTitle(tr("Radio Hey - právě se hraje"));
    setWindowIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));

    /* Layout */
    nowPlaying = new QLabel(tr("Inicializace..."));
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(nowPlaying, 0, Qt::AlignCenter);
    setLayout(layout);

    /* Initialize server */
    server = new AbRadioServer(this);
    connect(server, SIGNAL(track(Core::Track)), SLOT(updateTrack(Core::Track)));

    /* Inicializace časovače */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getUpdate()));

    /* Tray ikona */
    tray = new QSystemTrayIcon(this);
    tray->setIcon(style()->standardIcon(QStyle::SP_MediaStop).pixmap(16,16));
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(toggleVisibility(QSystemTrayIcon::ActivationReason)));

    /* Kontextové menu tray ikony */
    QMenu* menu = new QMenu(this);
    menu->addAction(
        style()->standardIcon(QStyle::SP_DriveFDIcon).pixmap(16,16),
        tr("Nastavení"), this, SLOT(openSettings()));
    menu->addAction(
        style()->standardIcon(QStyle::SP_DialogCloseButton).pixmap(16,16),
        tr("Ukončit"), qApp, SLOT(quit()));

    tray->setContextMenu(menu);
    tray->show();

    /* První aktualizace */
    timer->start(1000);

    /* Fixní velikost okna */
    setFixedSize(320,50);
}

/* Destruktor */
HeyTrack::~HeyTrack() {
    /* Smazání aktuální přehrávané skladby */
    saveIcesTune("", "");
}

/* Uložení songu do souboru */
bool HeyTrack::saveIcesTune(const QString& artist, const QString& title) {
    QString fileName = settings.value("icesTuneFile", "ices-tune").toString();
    if(fileName.isEmpty())
        return false;

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QString contents = "title=" + title + "\nartist=" + artist + "\nalbum=\nyear=\nlength=";

    file.write(contents.toUtf8());
    file.close();

    QStringList arguments;
    arguments << "-SIGUSR1" << "ices";
    QProcess::execute("/usr/bin/killall", arguments);
}

/* Získání aktuálního songu */
void HeyTrack::getUpdate() {
    /** @todo This is UGLY HACK! */
    server->getTrack(Station(36, "RockRádio Prácheň"));
}

/* Aktualizace textu aktuální písně */
void HeyTrack::updateTrack(Track t) {
    /* Update window label */
    nowPlaying->setText("<strong>" + t.artist() + "</strong> - " + t.title());

    /* Tray icon message */
    tray->showMessage(tr("Rockradio: právě se hraje"), t.artist() + " - " + t.title());
    tray->setIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
    tray->setToolTip("Rockradio: " + t.artist() + " - " + t.title());

    /* If enabled, save Ices Tune */
    if(settings.value("saveIcesTune", false).toBool())
        saveIcesTune(tr("Rockradio: ") + t.artist(), t.title());

    /* Next update after 20 seconds */
    timer->start(20*1000);
}

/* Přepínání viditelnosti okna */
void HeyTrack::toggleVisibility(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger)
        isHidden() ? show() : hide();
}

/* Otevření okna s nastavením */
void HeyTrack::openSettings() {
    HeySettings window(&settings);

    /* Vynucená modálnost (program je pozastaven do doby, než se dialog uzavře) */
    window.exec();
}

}}

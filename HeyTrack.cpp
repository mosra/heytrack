#include "HeyTrack.h"

#include <QStyle>
#include <QLabel>
#include <QHBoxLayout>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QApplication>
#include <QCloseEvent>
#include <QAction>
#include <QFile>
#include <QProcess>
#include <QtCore/QDebug>
#include <QtXmlPatterns/QXmlQuery>

#include <qjson/parser.h>

#include "HeySettings.h"

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

    /* Inicializace časovače */
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(getUpdate()));

    /* Incializace sítě */
    net = new QNetworkAccessManager(this);
    connect(net, SIGNAL(finished(QNetworkReply*)), this, SLOT(updateTrack(QNetworkReply*)));

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
    net->get(QNetworkRequest(QUrl("http://static.abradio.cz/data/ct/36.json")));
}

/* Aktualizace textu aktuální písně */
void HeyTrack::updateTrack(QNetworkReply* reply) {
    QJson::Parser parser;
    QXmlQuery query;
    QString artist;
    QString track;
    bool parsedOk = false;

    /* Parse server result */
    QVariantMap result = parser.parse(reply->readAll().data(), &parsedOk).toMap();
    if (!parsedOk) {
        qFatal("An error occurred during parsing JSON");
        exit(1);
    }

    /* If track wasn't updated, nothing to do */
    quint32 update = result["lastchange"].toInt();
    if(lastUpdate == update && update != 0) return;
    lastUpdate = update;

    /* Root item for getting data*/
    query.setFocus("<root>" + result["html"].toString().replace("&nbsp;","") + "</root>");

    /* Artist */
    query.setQuery("root/li[@class='current']/span[@class='artistname']//text()");
    if (!query.isValid()) {
        qFatal("An error occurred during parsing artist");
        exit(1);
    }
    query.evaluateTo(&artist);
    artist = artist.trimmed();

    /* Track name */
    query.setQuery("root/li[@class='current']/span[@class='trackname']//text()");
    if (!query.isValid()) {
        qFatal("An error occurred during parsing track");
        exit(1);
    }
    query.evaluateTo(&track);
    track = track.trimmed();

    /* Update window label */
    nowPlaying->setText("<strong>" + artist + "</strong> - " + track);

    /* Tray icon message */
    tray->showMessage(tr("Rockradio: právě se hraje"), artist + " - " +track);
    tray->setIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
    tray->setToolTip("Rockradio: " + artist + " - " + track);

    /* If enabled, save Ices Tune */
    if(settings.value("saveIcesTune", false).toBool())
        saveIcesTune(tr("Rockradio: ") + artist, track);

    /* Next update after 20 seconds */
    timer->start(20*1000);

    /* Cleanup server reply */
    reply->deleteLater();
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

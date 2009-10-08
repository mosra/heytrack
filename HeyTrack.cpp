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
    net->get(QNetworkRequest(QUrl("http://www.radiohey.cz/pravehraje-brno/now-read.php")));
}

/* Aktualizace textu aktuální písně */
void HeyTrack::updateTrack(QNetworkReply* reply) {
    /* Překódujeme reply z UTF-8 a rozsekáme podle %% do pole */
    QStringList str = QString::fromUtf8(reply->readAll().data()).split("%%");

    /* Do konce písně zbývá 0 == nic se nehraje */
    if(str[2].toInt() == 0) {
        /* Změna labelu v okně */
        nowPlaying->setText(tr("Právě se nic nehraje"));

        /* Změna tooltipu a ikony traye na pauzu */
        tray->setToolTip(tr("Radio Hey: právě se nic nehraje"));
        tray->setIcon(style()->standardIcon(QStyle::SP_MediaPause).pixmap(16,16));

        /* Pokud je povoleno, uložíme Ices Tune */
        if(settings.value("saveIcesTune", false).toBool())
            saveIcesTune(tr("Radio Hey"), tr("Radio Hey"));

        /* Aktualizace hned za 5 s */
        timer->start(5000);

    /* Přehrává se, příště budeme aktualizovat, až tato píseň skončí */
    } else {
        /* Změna labelu v okně */
        nowPlaying->setText("<strong>" + str[0] + "</strong> - " + str[1]);

        /* Ukázání zprávy, změna tooltipu a ikony traye na play */
        tray->showMessage(tr("Radio Hey: právě se hraje"), str[0] + " - " +str[1]);
        tray->setIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
        tray->setToolTip("Radio Hey: " + str[0] + " - " + str[1]);

        /* Pokud je povoleno, uložíme Ices Tune */
        if(settings.value("saveIcesTune", false).toBool())
            saveIcesTune(tr("Radio Hey: ") + str[0], str[1]);

        /* Aktualizace, až píseň skončí */
        timer->start(str[2].toInt());
    }

    /* Uklidíme po sobě */
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

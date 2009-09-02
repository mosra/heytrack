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
    tray->show();
    connect(tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(toggleVisibility(QSystemTrayIcon::ActivationReason)));

    /* První aktualizace */
    timer->start(1000);

    /* Fixní velikost okna */
    setFixedSize(320,50);
}

/* Získání aktuálního songu */
void HeyTrack::getUpdate() {
    net->get(QNetworkRequest(QUrl("http://www.radiohey.cz/pravehraje-brno/now-read.php")));
}

/* Aktualizace textu aktuální písně */
void HeyTrack::updateTrack(QNetworkReply* reply) {
    /* Překódujeme reply z UTF-8 a rozsekáme podle %% do pole */
    QStringList str = QString::fromUtf8(reply->readAll().data()).split("%%");

    /* Do konce písně zbývá 0 == nic se nehraje, aktualizace hned za 5s */
    if(str[2].toInt() == 0) {
        nowPlaying->setText(tr("Právě se nic nehraje"));
        tray->setToolTip(tr("Radio Hey: právě se nic nehraje"));
        tray->setIcon(style()->standardIcon(QStyle::SP_MediaPause).pixmap(16,16));
        timer->start(5000);

    /* Přehrává se, příště budeme aktualizovat, až tato píseň skončí */
    } else {
        nowPlaying->setText("<strong>" + str[0] + "</strong> - " + str[1]);
        tray->showMessage(tr("Radio Hey: právě se hraje"), str[0] + " - " +str[1]);
        tray->setIcon(style()->standardIcon(QStyle::SP_MediaPlay).pixmap(16,16));
        tray->setToolTip("Radio Hey: " + str[0] + " - " + str[1]);
        timer->start(str[2].toInt());
    }
}

/* Přepínání viditelnosti okna */
void HeyTrack::toggleVisibility(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Trigger)
        isHidden() ? show() : hide();
}

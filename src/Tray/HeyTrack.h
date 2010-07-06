#ifndef HEYTRACK_H
#define HEYTRACK_H

#include <QtCore/QSettings>
#include <QtGui/QWidget>
#include <QtGui/QSystemTrayIcon>
#include <QtGui/QCloseEvent>

#include <Core/Track.h>

class QNetworkAccessManager;
class QLabel;
class QTimer;
class QNetworkReply;

namespace HeyTrack {

namespace Core {
    class AbstractServer;
}

namespace Tray {

/**
 * @brief Widget zobrazující aktuální přehrávanou píseň na rádiu Hey
 *
 * Aktuálně přehrávaná píseň se zobrazí v okně. Tato informace je zobrazena i
 * v tray ikoně. Pokud se právě nic nepřehrává (zprávy, ...), je v trayi
 * zobrazena ikonka pauzy.
 * @todo Separate GUI from logic
 * @todo Globalize for all stations
 */
class HeyTrack: public QWidget {
    Q_OBJECT

    public:
        /**
         * @brief Konstruktor
         *
         * Inicializuje síť, tray ikonu a okno
         * @param   parent      Rodičovský widget
         */
        HeyTrack(QWidget* parent = 0);

        /**
         * @brief Destruktor
         *
         * Smaže aktuální přehrávanou skladbu v IcesTune
         */
        ~HeyTrack();

    private:
        /**
         * @brief Událost zavření okna
         *
         * Okno nezavře, ale schová do traye
         * @param   event       Ukazatel na událost
         */
        inline virtual void closeEvent(QCloseEvent* event)
            { hide(); event->ignore(); }

        /**
         * @brief Zapsání songu do souboru
         *
         * Zapíše song do souboru Ices Tune.
         */
        bool saveIcesTune(const QString& artist, const QString& title);

        QTimer* timer;              /**< @brief Časovač pro zjištění další skladby */
        QLabel* nowPlaying;         /**< @brief Label, co se právě hraje */
        Core::AbstractServer* server; /**< @brief Stream server */
        QSystemTrayIcon* tray;      /**< @brief Tray ikona */

        QSettings settings;         /**< @brief Nastavení programu */
        quint32 lastUpdate;         /**< @brief Poslední aktualizace stavu na serveru */

    private slots:
        /**
         * @brief Odeslání GET požadavku na webovou stránku
         *
         * Poté, co je požadavek splněn, volá se automaticky
         * HeyTrack::updateTrack()
         */
        void getUpdate();

        /**
         * @brief Aktualizace názvu písně
         *
         * Je volán automaticky po získání nových dat z HeyTrack::getUpdate().
         * Aktualizuje název skladby v okně, v trayi a v hintu Traye. Pokud se
         * právě něco přehrává, vyhodí bublinu nad tray ikonou.
         * @param   reply   Ukazatel na objekt s odpovědí ze serveru
         */
        void updateTrack(Core::Track t);

        /**
         * @brief Schová / zobrazí okno
         *
         * Spouštěno po kliknutí na tray ikonu.
         * @param   reason  Důvod aktivace tray ikony (spouštěno jen při kliknutí).
         */
        void toggleVisibility(QSystemTrayIcon::ActivationReason reason);

        /**
         * @brief Otevření menu s nastavením
         *
         * Spuštěno po kliknutí na položku kontextového menu tray ikony
         */
        void openSettings();
};

}}

#endif

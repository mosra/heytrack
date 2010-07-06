#ifndef HEYSETTINGS_H
#define HEYSETTINGS_H

#include <QDialog>

class QCheckBox;
class QSettings;
class QLineEdit;

/**
 * @brief Dialog s nastavením HeyTrack
 *
 * Umožňuje nastavení, zda zobrazit po startu okno aplikace a zda zapisovat
 * aktuální přehrávaný song do Ices Tune.
 */
class HeySettings: public QDialog {
    Q_OBJECT

    public:
        /**
         * @brief Konstruktor
         *
         * @param   _settings   Ukazatel na nastavení
         */
        HeySettings(QSettings* _settings);

    private:
        QSettings* settings;        /** @brief Ukazatel na nastavení */
        QCheckBox *showMainWindow,  /** @brief Zda ukazovat okno aplikace */
            *saveIcesTune;          /** @brief Zda zapisovat soubor IcesTune */
        QLineEdit* icesTuneFile;    /** @brief Cesta k souboru IcesTune */

    private slots:
        virtual void accept();      /** @brief Potvrzení dialogu */
};

#endif

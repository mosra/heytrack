#include "HeySettings.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QSettings>
#include <QLabel>

/* Konstruktor */
HeySettings::HeySettings(QSettings* _settings): settings(_settings) {
    setWindowTitle(tr("Nastavení HeyTrack"));

    /* Zaškrtávací boxy */
    showMainWindow = new QCheckBox(tr("Zobrazit po startu okno aplikace"));
    showMainWindow->setDisabled(true);
    saveIcesTune = new QCheckBox(
        tr("Zapisovat aktuální song do ~/.ices-tune"));

    /* Tlačítka */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(showMainWindow);
    layout->addWidget(saveIcesTune);
    layout->addWidget(new QLabel(
        tr("<em>Tuto volbu zaškrtněte jen pokud opravdu víte, co děláte.</em>")));
    layout->addWidget(buttons);

    /* Zaškrtání podle nastavení */
    if(settings->value("showMainWindow", true).toBool())
        showMainWindow->setChecked(true);
    if(settings->value("saveIcesTune", false).toBool())
        saveIcesTune->setChecked(true);
}

/* Akceptování dialogu */
void HeySettings::accept() {
    settings->setValue("showMainWindow", showMainWindow->isChecked());
    settings->setValue("saveIcesTune", saveIcesTune->isChecked());
    done(QDialog::Accepted);
}

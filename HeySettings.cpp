#include "HeySettings.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QBoxLayout>
#include <QSettings>
#include <QLabel>
#include <QLineEdit>

/* Konstruktor */
HeySettings::HeySettings(QSettings* _settings): settings(_settings) {
    setWindowTitle(tr("Nastavení HeyTrack"));

    /* Zaškrtávací boxy */
    showMainWindow = new QCheckBox(tr("Zobrazit po startu okno aplikace"));
    showMainWindow->setDisabled(true);
    saveIcesTune = new QCheckBox(
        tr("Zapisovat aktuální song do ~/.ices-tune"));
    icesTuneFile = new QLineEdit(
        settings->value("icesTuneFile", "ices-tune").toString());

    /* Tlačítka */
    QDialogButtonBox* buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(showMainWindow);
    layout->addWidget(saveIcesTune);
    QLabel* knowWhatYouDo = new QLabel(
        tr("<em>Tuto volbu zaškrtněte jen pokud opravdu víte, co děláte.</em>"));
    layout->addWidget(knowWhatYouDo);

    QHBoxLayout* lineEditLayout = new QHBoxLayout();
    QLabel* icesTuneFileLabel = new QLabel(tr("Cesta k souboru IcesTune"));
    lineEditLayout->addWidget(icesTuneFileLabel);
    lineEditLayout->addWidget(icesTuneFile);

    layout->addLayout(lineEditLayout);
    layout->addWidget(buttons);

    /* Při odškrtnutí saveIcesTune zašednout volby pod tím */
    connect(saveIcesTune, SIGNAL(toggled(bool)), knowWhatYouDo, SLOT(setEnabled(bool)));
    connect(saveIcesTune, SIGNAL(toggled(bool)), icesTuneFile, SLOT(setEnabled(bool)));
    connect(saveIcesTune, SIGNAL(toggled(bool)), icesTuneFileLabel, SLOT(setEnabled(bool)));

    /* Defaultně se Ices Tune neukládá, zašednout */
    knowWhatYouDo->setDisabled(true);
    icesTuneFileLabel->setDisabled(true);
    knowWhatYouDo->setDisabled(true);

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
    settings->setValue("icesTuneFile", icesTuneFile->text());
    done(QDialog::Accepted);
}

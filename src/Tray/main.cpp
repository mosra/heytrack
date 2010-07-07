#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>

#include "configure.h"
#include "HeyTrack.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("HeyTrack");
    app.setOrganizationName("Mosra");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    /* Localizations */
    QTranslator translator;
    translator.load(HEYTRACK_DATA_DIR + QString("/l10n/") + QLocale::system().name());
    app.installTranslator(&translator);

    HeyTrack::Tray::HeyTrack foo;
    foo.show();

    return app.exec();
}

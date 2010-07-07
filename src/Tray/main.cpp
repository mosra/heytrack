#include <QtGui/QApplication>
#include <QtCore/QTextCodec>

#include "configure.h"
#include "HeyTrack.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("HeyTrack");
    app.setOrganizationName("Mosra");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    HeyTrack::Tray::HeyTrack foo;
    foo.show();

    return app.exec();
}

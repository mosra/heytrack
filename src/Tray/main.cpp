#include <QApplication>
#include <QTextCodec>
#include "HeyTrack.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    app.setApplicationName("HeyTrack");
    app.setOrganizationName("Mosra");

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    HeyTrack::Tray::HeyTrack foo;
    foo.show();

    return app.exec();
}

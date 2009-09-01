#include <QApplication>
#include <QTextCodec>
#include "HeyTrack.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    HeyTrack foo;
    foo.show();
    return app.exec();
}

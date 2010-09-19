/*
    Copyright © 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of HeyTrack.

    HeyTrack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    HeyTrack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include <QtGui/QApplication>
#include <QtCore/QTextCodec>
#include <QtCore/QLocale>
#include <QtCore/QTranslator>
#include <QtCore/QLibraryInfo>

#include "Core/registerEverything.h"
#include "configure.h"
#include "HeyTrack.h"

int main(int argc, char** argv) {
    HeyTrack::Core::registerEverything();

    QApplication app(argc, argv);
    app.setApplicationName("HeyTrack");
    app.setOrganizationName("Mosra");

    /* Don't quit when last window closed */
    app.setQuitOnLastWindowClosed(false);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));

    /* Localizations */
    QTranslator translatorQt, translator;
    translatorQt.load("qt_" + QLocale::system().name(), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    translator.load(QLocale::system().name(), HEYTRACK_TRANSLATION_DIR);

    app.installTranslator(&translatorQt);
    app.installTranslator(&translator);

    HeyTrack::Tray::HeyTrack foo;
    foo.show();

    return app.exec();
}

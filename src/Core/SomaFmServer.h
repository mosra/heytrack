#ifndef HeyTrack_Core_SomaFmServer_h
#define HeyTrack_Core_SomaFmServer_h
/*
    Copyright © 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of HeyTrack.

    HeyTrack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    HeyTrack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "AbstractServer.h"

#include <QtCore/QSignalMapper>

namespace HeyTrack { namespace Core {

/** @brief SomaFM.com server
 *
 * First approach: Parsing from HTML page (DANGEROUS, UNFINISHED, COMMENTED OUT)
 * - Advantages: Windows Media streams, firewall friendly streams
 * - Disadvantages: BIG SHITLOAD OF CRAP
 *
 * Second approach: Parsing well-formed XML
 * - Advantages: everything possible
 * - Disadvantages: no Windows Media, firewall friendly streams
 * @todo Don't download the same data many times
 * @todo On fast (genre) switching errors pop out
 */
class SomaFmServer: public AbstractServer {
    Q_OBJECT

    SERVER_DEFINE(SomaFmServer)

    public:
        SomaFmServer(QObject* parent = 0);

    public slots:
        virtual void getGenres();
        virtual void getStations(const Genre& genre = Genre());
        virtual void getFormats(const Station& station);
        virtual void getTrack(const Station& station);
        inline virtual QString streamUrl(const Station& station, const Format& format) const { return format.nick(); }

    private slots:
        virtual void processGenres();
        virtual void processStations(const QString& genre);
        virtual void processFormats(const QString& station);
        virtual void processTrack(const QString& station);

    private:
        QSignalMapper *stationMapper, *formatMapper, *trackMapper;
        QString lastArtist, lastTitle;
};

}}

#endif

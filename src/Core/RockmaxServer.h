#ifndef HeyTrack_Core_RockmaxServer_h
#define HeyTrack_Core_RockmaxServer_h
/*
    Copyright © 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010, 2011 Jan Dupal <dupal.j@seznam.cz>

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

class QTextCodec;
class QTextDecoder;

namespace HeyTrack { namespace Core {

/**
 * @brief Rockmax.cz server
 *
 * @todo Add Rockmax live stream
 */
class RockmaxServer: public AbstractServer {
    Q_OBJECT

    SERVER_DEFINE(RockmaxServer)

    public:
        RockmaxServer(QObject* parent = 0);
        virtual ~RockmaxServer();

    public slots:
        virtual void getStations(const Genre& genre = Genre());
        virtual void getFormats(const Station& station);
        virtual void getTrack(const Station& station);

        virtual QString streamUrl(const Station& station, const Format& format) const;

    private slots:
        virtual void processTrack();

    private:
        QString lastArtist, lastTitle;
        QTextCodec *codec;
        QTextDecoder *decoder;
};

}}

#endif

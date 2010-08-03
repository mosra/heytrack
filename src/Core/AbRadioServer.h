#ifndef HeyTrack_Core_AbRadioServer_h
#define HeyTrack_Core_AbRadioServer_h
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

#include "AbstractServer.h"

namespace HeyTrack { namespace Core {

/**
 * @brief ABRadio.cz server
 *
 * @todo Station list, genre list
 */
class AbRadioServer: public AbstractServer {
    private:
        unsigned int lastUpdate;

    public:
        inline AbRadioServer(QObject* parent = 0): AbstractServer(parent), lastUpdate(0) {}

        inline virtual QString name() const { return QString::fromUtf8("ABRadio.cz"); }

    public slots:
        virtual void getGenres();
        virtual void getStations(const Genre& genre = Genre());
        virtual void getTrack(const Station& station);

    private slots:
        virtual void processStations();
        virtual void processTrack();
};

}}

#endif

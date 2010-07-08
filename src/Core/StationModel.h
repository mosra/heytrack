#ifndef HeyTrack_Core_StationModel_h
#define HeyTrack_Core_StationModel_h
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

/** @file
 * @brief Class HeyTrack::Core::StationModel
 */

#include <QtCore/QAbstractListModel>

#include "Station.h"

namespace HeyTrack { namespace Core {

/** @brief Station model */
class StationModel: public QAbstractListModel {
    Q_OBJECT

    private:
        QList<Station> stations;

    public:
        inline StationModel(const QList<Station>& _stations, QObject* parent = 0): QAbstractListModel(parent), stations(_stations) {}

        inline virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
            { return stations.count(); }

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        inline const Station& station(int row) { return stations.at(row); }
};

}}

#endif

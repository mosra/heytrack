/*
    Copyright © 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of HeyTrack.

    HeyTrack is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    HeyTrack is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "StationModel.h"

namespace HeyTrack { namespace Core {

QVariant StationModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid() || index.row() >= rowCount() || index.column() != 0)
        return QVariant();

    if(role == Qt::DisplayRole) return stations.at(index.row()).name();
    if(role == Qt::UserRole) return stations.at(index.row()).id();

    return QVariant();
}

}}

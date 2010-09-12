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

#include "StationModel.h"

namespace HeyTrack { namespace Core {

QVariant StationModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid() || index.row() >= rowCount() || index.column() != 0)
        return QVariant();

    if(role == Qt::DisplayRole) return stations.at(index.row()).name();
    if(role == Id) return stations.at(index.row()).id();
    if(role == Nick) return stations.at(index.row()).nick();

    return QVariant();
}

bool StationModel::removeRows(int row, int count, const QModelIndex& parent) {
    if(row < 0 || row+count > stations.size()) return false;

    beginRemoveRows(parent, row, row+count-1);
    for(int i = row+count-1; i >= 0; --i)
        stations.removeAt(i);
    endRemoveRows();

    return true;
}

}}

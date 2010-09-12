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

#include "FormatModel.h"

namespace HeyTrack { namespace Core {

QVariant FormatModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid() || index.row() >= rowCount() || index.column() != 0)
        return QVariant();

    if(role == Qt::DisplayRole) return formats.at(index.row()).name();
    if(role == Id) return formats.at(index.row()).id();
    if(role == Nick) return formats.at(index.row()).nick();

    return QVariant();
}

bool FormatModel::removeRows(int row, int count, const QModelIndex& parent) {
    if(row < 0 || row+count > formats.size()) return false;

    beginRemoveRows(parent, row, row+count-1);
    for(int i = row+count-1; i >= 0; --i)
        formats.removeAt(i);
    endRemoveRows();

    return true;
}

}}

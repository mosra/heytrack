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

#include "GenreModel.h"

namespace HeyTrack { namespace Core {

QVariant GenreModel::data(const QModelIndex& index, int role) const {
    if(!index.isValid() || index.row() >= rowCount() || index.column() != 0)
        return QVariant();

    if(index.row() == 0) {
        if(role == Qt::DisplayRole) return tr("All genres");
        if(role == Qt::UserRole) return 0;
        return QVariant();
    }

    if(role == Qt::DisplayRole) return genres.at(index.row()-1).name();
    if(role == Qt::UserRole) return genres.at(index.row()-1).id();

    return QVariant();
}

Genre GenreModel::genre(int row) const {
    if(row >= rowCount() || row <= 0) return Genre();

    return genres.at(row-1);
}

}}

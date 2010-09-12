#ifndef HeyTrack_Core_GenreModel_h
#define HeyTrack_Core_GenreModel_h
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
 * @brief Class HeyTrack::Core::GenreModel
 */

#include <QtCore/QAbstractListModel>

#include "Genre.h"

namespace HeyTrack { namespace Core {

/** @brief Genre model */
class GenreModel: public QAbstractListModel {
    Q_OBJECT

    private:
        QList<Genre> genres;

    public:
        inline GenreModel(const QList<Genre>& _genres, QObject* parent = 0): QAbstractListModel(parent), genres(_genres) {}

        inline virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
            { return genres.count()+1; }

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

        Genre genre(int row) const;
};

}}

#endif

#ifndef HeyTrack_Core_FormatModel_h
#define HeyTrack_Core_FormatModel_h
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
 * @brief Class HeyTrack::Core::FormatModel
 */

#include <QtCore/QAbstractListModel>

#include "Format.h"

namespace HeyTrack { namespace Core {

/** @brief Format model */
class FormatModel: public QAbstractListModel {
    Q_OBJECT

    private:
        QList<Format> formats;

    public:
        enum DataRole {
            Id = 32,
            Nick = 33
        };

        inline FormatModel(const QList<Format>& _formats, QObject* parent = 0): QAbstractListModel(parent), formats(_formats) {}

        inline virtual int rowCount(const QModelIndex& parent = QModelIndex()) const
            { return formats.count(); }

        virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

        virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex());

        inline const Format& format(int row) { return formats.at(row); }
};

}}

#endif

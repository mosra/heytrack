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

#include "AbRadioServer.h"

namespace HeyTrack { namespace Core {

QHash<QString, AbstractServer* (*)(QObject*)> AbstractServer::_servers;

void AbstractServer::registerServer(const QString& name, AbstractServer* (*instancer)(QObject*)) {
    _servers.insert(name, instancer);
}

QList<QString> AbstractServer::servers() {
    return _servers.keys();
}

AbstractServer* AbstractServer::instance(const QString& name, QObject* parent) {
    if(!_servers.contains(name)) return 0;

    return _servers[name](parent);
}

AbstractServer::AbstractServer(QObject* parent): QObject(parent), manager(new QNetworkAccessManager(this)) {}

}}

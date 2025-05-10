/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "connection.h"
#include "local.h"

#include <QDebug>

// Constructor for a local connection
Connection::Connection(QObject *qObject)
    : QObject(qObject)
{
    this->qObject = qObject;
    this->localConnection = new Local(qObject);
    this->remote = false;

    // Make sure we forward the local process output
    connect(localConnection, SIGNAL(localDataAvailable(QString)), this, SLOT(processData(QString)));
}

// Constructor for a remote SSH connection
Connection::Connection(QObject *qObject, SSHConnectionSettings *sshSettings)
    : QObject(qObject)
{
    this->qObject = qObject;

    this->remoteConnection = new Remote(qObject, sshSettings);
    this->remote = true;

    // Make sure we forward the local process output
    connect(remoteConnection, SIGNAL(remoteDataAvailable(QString)), this, SLOT(processData(QString)));
}

Connection::~Connection()
{
    if(remote){
        delete remoteConnection;
    } else {
        delete localConnection;
    }
}

void Connection::processData(QString data)
{
    emit connectionDataAvailable(data);
}

void Connection::run(QString program, QStringList arguments)
{
    if(remote){
        remoteConnection->run(program, arguments);
    } else {
        localConnection->run(program, arguments);
    }
}

QString Connection::runAndWait(QString program, QStringList arguments)
{
    if(remote){
        return remoteConnection->runAndWait(program, arguments);
    } else {
        return localConnection->runAndWait(program, arguments);
    }
}

bool Connection::isRemote()
{
    return remote;
}

bool Connection::isRunning()
{
    if(remote){
        return remoteConnection->isRunning();
    } else {
        return localConnection->isRunning();
    }
}

void Connection::close()
{
    if(remote){
        remoteConnection->close();
    } else {
        localConnection->close();
    }
}

void Connection::setParam(CONNECTION_PARAM p, QString val)
{
    switch(p){
    case LOCAL_DIRECTORY:
        localConnection->setDirectory(val);
    default:
        break;
    }
}

/**
 * qjournalctl: A Qt-based GUI for systemd's journalctl command
 *
 * Copyright (c) 2016-2021 by Patrick Eigensatz <patrick.eigensatz@gmail.com>
 * Some rights reserved. See LICENSE.
 */


#include "local.h"

#include <QProcess>
#include <QDebug>

Local::Local(QObject *qObject)
    : QObject(qObject)
{
    journalProcess = new QProcess(qObject);
    connect(journalProcess, SIGNAL(readyRead()), this, SLOT(processHasData()));
}


Local::~Local()
{
    delete journalProcess;
}


void Local::run(QString program, QStringList arguments)
{
    // Make sure to terminate an already running process first
    if(isRunning()){
        journalProcess->close();
    }

    if(directory != "") {
        arguments << "--directory" << directory;
    }

    journalProcess->start(program, arguments);
}

QString Local::runAndWait(QString program, QStringList arguments)
{
    if(directory != "") {
        arguments << "--directory" << directory;
    }

    QProcess p;
    p.start(program, arguments);
    p.waitForFinished(-1);

    return QString(p.readAllStandardOutput());
}

void Local::close()
{
    journalProcess->close();
}


bool Local::isRunning()
{
    return (journalProcess->state() == QProcess::Running
            || journalProcess->state() == QProcess::Starting);
}

void Local::setDirectory(QString dir)
{
    directory = dir;
}

void Local::processHasData()
{
    // Collect the process output, then forward this
    // event through the abstraction layers
    QByteArray read = journalProcess->read(8192000);

    QString readString = QString(read);
    readString = readString.left(readString.size()-1);

    emit localDataAvailable(readString);
}

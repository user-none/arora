/*
 * Copyright 2009 Jonas Gehring <jonas.gehring@boolsoft.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#ifndef FILEACCESSHANDLER_H
#define FILEACCESSHANDLER_H

#include "schemeaccesshandler.h"

#include <qbuffer.h>
#include <qnetworkreply.h>

class FileAccessHandler : public SchemeAccessHandler
{
public:
    FileAccessHandler(QObject *parent = 0);

    virtual QNetworkReply *createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData = 0);
};

class FileAccessReply : public QNetworkReply
{
    Q_OBJECT

public:
    FileAccessReply(const QNetworkRequest &request, QObject *parent = 0);
    ~FileAccessReply();

    virtual qint64 bytesAvailable() const;
    virtual void abort() { };
    virtual void close();

protected:
    virtual qint64 readData(char *data, qint64 maxSize);

private slots:
    void listDirectory();

private:
    QBuffer buffer;
};

#endif // FILEACCESSHANDLER_H

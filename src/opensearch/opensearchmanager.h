/*
 * Copyright 2009 Jakub Wieczorek <faw217@gmail.com>
 * Copyright 2009 Christian Franke <cfchris6@ts2server.com>
 * Copyright 2009 Christopher Eby <kreed@kreed.org>
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

#ifndef OPENSEARCHMANAGER_H
#define OPENSEARCHMANAGER_H

#include <qobject.h>

#include <qhash.h>
#include <qpixmap.h>
#include <qurl.h>

class QNetworkReply;
class QNetworkRequest;

class AutoSaver;
class OpenSearchEngine;
class OpenSearchEngineModel;

class OpenSearchManager : public QObject
{
    Q_OBJECT

signals:
    void changed();
    void currentEngineChanged();

public:
    OpenSearchManager(QObject *parent = 0);
    ~OpenSearchManager();

    QStringList allEnginesNames() const;
    int enginesCount() const;

    QString currentEngineName() const;
    void setCurrentEngineName(const QString &currentName);

    OpenSearchEngine *currentEngine() const;
    void setCurrentEngine(OpenSearchEngine *current);

    OpenSearchEngine *engine(const QString &name);

    bool engineExists(const QString &name);

    QUrl convertKeywordSearchToUrl(const QString &string);
    OpenSearchEngine *engineForKeyword(const QString &keyword) const;
    void setEngineForKeyword(const QString &keyword, OpenSearchEngine *engine);

    QStringList keywordsForEngine(OpenSearchEngine *engine) const;
    void setKeywordsForEngine(OpenSearchEngine *engine, const QStringList &keywords);

    void addEngine(const QUrl &url);
    bool addEngine(const QString &fileName);
    bool addEngine(OpenSearchEngine *engine);
    void removeEngine(const QString &name);
    void restoreDefaults();

public slots:
    void save();

protected:
    void load();
    bool loadDirectory(const QString &dirName);
    void saveDirectory(const QString &dirName);
    QString enginesDirectory() const;
    QString generateEngineFileName(const QString &engineName) const;

private:
    bool confirmAddition(OpenSearchEngine *engine);

protected slots:
    void engineFromUrlAvailable();

private:
    AutoSaver *m_autoSaver;

    QHash<QString, OpenSearchEngine*> m_engines;
    QHash<QString, OpenSearchEngine*> m_keywords;
    QString m_current;
};

#endif //OPENSEARCHMANAGER_H


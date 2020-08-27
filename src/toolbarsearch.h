/*
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
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

/****************************************************************************
**
** Copyright (C) 2007-2008 Trolltech ASA. All rights reserved.
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the files LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file.  Alternatively you may (at
** your option) use any later version of the GNU General Public
** License if such license has been publicly approved by Trolltech ASA
** (or its successors, if any) and the KDE Free Qt Foundation. In
** addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.2, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/. If
** you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech, as the sole
** copyright holder for Qt Designer, grants users of the Qt/Eclipse
** Integration plug-in the right for the Qt/Eclipse Integration to
** link to functionality provided by Qt Designer and its related
** libraries.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not expressly
** granted herein.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef TOOLBARSEARCH_H
#define TOOLBARSEARCH_H

#include "searchlineedit.h"

#include "tabwidget.h"

class AutoSaver;
class OpenSearchManager;
class QCompleter;
class QModelIndex;
class QStandardItem;
class QStandardItemModel;
class QTimer;
class QUrl;
class ToolbarSearch : public SearchLineEdit
{
    Q_OBJECT

signals:
    void search(const QUrl &url, TabWidget::OpenUrlIn tab);

public:
    ToolbarSearch(QWidget *parent = 0);
    ~ToolbarSearch();
    static OpenSearchManager *openSearchManager();

public slots:
    void clear();
    void searchNow();

private slots:
    void currentEngineChanged();
    void save();
    void textEdited(const QString &);
    void newSuggestions(const QStringList &suggestions);
    void completerActivated(const QModelIndex &index);
    bool completerHighlighted(const QModelIndex &index);
    void getSuggestions();
    void showEnginesMenu();
    void changeCurrentEngine();
    void addEngineFromUrl();

protected:
    void changeEvent(QEvent *event);
    void focusInEvent(QFocusEvent *event);

private:
    void load();
    void setupList();
    void retranslate();

    static OpenSearchManager *s_openSearchManager;
    QString m_currentEngine;
    bool m_suggestionsEnabled;

    AutoSaver *m_autosaver;
    int m_maxSavedSearches;
    QStringList m_recentSearches;
    QStringList m_suggestions;
    QStandardItemModel *m_model;

    QStandardItem *m_suggestionsItem;
    QStandardItem *m_recentSearchesItem;
    QTimer *m_suggestTimer;

    QCompleter *m_completer;
};

#endif // TOOLBARSEARCH_H


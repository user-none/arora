/*
 * Copyright 2008-2009 Benjamin C. Meyer <ben@meyerhome.net>
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

#ifndef BOOKMARKSMANAGER_H
#define BOOKMARKSMANAGER_H

#include <qobject.h>

#include <qundostack.h>

#include "tabwidget.h"

class AutoSaver;
class BookmarkNode;
class BookmarksModel;
class BookmarksManager : public QObject
{
    Q_OBJECT

signals:
    void entryAdded(BookmarkNode *item);
    void entryRemoved(BookmarkNode *parent, int row, BookmarkNode *item);
    void entryChanged(BookmarkNode *item);

public:
    BookmarksManager(QObject *parent = 0);
    ~BookmarksManager();

    void addBookmark(BookmarkNode *parent, BookmarkNode *node, int row = -1);
    void removeBookmark(BookmarkNode *node);
    void setTitle(BookmarkNode *node, const QString &newTitle);
    void setUrl(BookmarkNode *node, const QString &newUrl);
    void changeExpanded();
    void retranslate() const;

    BookmarkNode *bookmarks();
    BookmarkNode *menu();
    BookmarkNode *toolbar();

    BookmarksModel *bookmarksModel();
    QUndoStack *undoRedoStack() {
        return &m_commands;
    }

public slots:
    void importBookmarks();
    void exportBookmarks();

private slots:
    void save() const;

private:
    void load();

    bool m_loaded;
    AutoSaver *m_saveTimer;
    BookmarkNode *m_bookmarkRootNode;
    BookmarkNode *m_toolbar;
    BookmarkNode *m_menu;
    BookmarksModel *m_bookmarkModel;
    QUndoStack m_commands;

    friend class RemoveBookmarksCommand;
    friend class ChangeBookmarkCommand;
};

class RemoveBookmarksCommand : public QUndoCommand
{

public:
    RemoveBookmarksCommand(BookmarksManager *m_bookmarkManagaer, BookmarkNode *parent, int row);
    ~RemoveBookmarksCommand();
    void undo();
    void redo();

protected:
    int m_row;
    BookmarksManager *m_bookmarkManagaer;
    BookmarkNode *m_node;
    BookmarkNode *m_parent;
    bool m_done;
};

class InsertBookmarksCommand : public RemoveBookmarksCommand
{

public:
    InsertBookmarksCommand(BookmarksManager *m_bookmarkManagaer,
                           BookmarkNode *parent, BookmarkNode *node, int row);
    void undo() {
        RemoveBookmarksCommand::redo();
    }
    void redo() {
        RemoveBookmarksCommand::undo();
    }

};

class ChangeBookmarkCommand : public QUndoCommand
{

public:
    ChangeBookmarkCommand(BookmarksManager *m_bookmarkManagaer,
                          BookmarkNode *node, const QString &newValue, bool title);
    void undo();
    void redo();

private:
    BookmarksManager *m_bookmarkManagaer;
    bool m_title;
    QString m_oldValue;
    QString m_newValue;
    BookmarkNode *m_node;
};

#endif // BOOKMARKSMANAGER_H

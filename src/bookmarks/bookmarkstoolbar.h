/*
 * Copyright 2008-2009 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright 2009 Jakub Wieczorek <faw217@gmail.com>
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

#ifndef BOOKMARKSTOOLBAR_H
#define BOOKMARKSTOOLBAR_H

#include "modeltoolbar.h"

#include "tabwidget.h"

#include <qabstractitemmodel.h>
#include <qpoint.h>
#include <qurl.h>

class BookmarksModel;
class BookmarksToolBar : public ModelToolBar
{
    Q_OBJECT

signals:
    void openUrl(const QUrl &url, const QString &title);
    void openUrl(const QUrl &url, TabWidget::OpenUrlIn tab, const QString &title);

public:
    BookmarksToolBar(BookmarksModel *model, QWidget *parent = 0);

protected:
    virtual ModelMenu *createMenu();

private slots:
    void contextMenuRequested(const QPoint &position);

protected slots:
    void openBookmark();
    void openBookmarkInCurrentTab();
    void openBookmarkInNewTab();
    void removeBookmark();
    void newBookmark();
    void newFolder();
    void bookmarkActivated(const QModelIndex &);

private:
    BookmarksModel *m_bookmarksModel;

    QPoint m_dragStartPosition;
};

#endif // BOOKMARKSTOOLBAR_H

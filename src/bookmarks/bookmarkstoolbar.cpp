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

#include "bookmarkstoolbar.h"

#include "addbookmarkdialog.h"
#include "bookmarknode.h"
#include "bookmarksmanager.h"
#include "bookmarksmenu.h"
#include "bookmarksmodel.h"
#include "browserapplication.h"
#include "modelmenu.h"

#include <qevent.h>

BookmarksToolBar::BookmarksToolBar(BookmarksModel *model, QWidget *parent)
    : ModelToolBar(parent)
    , m_bookmarksModel(model)
{
    setModel(model);
    setRootIndex(model->index(BrowserApplication::bookmarksManager()->toolbar()));

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
            this, SLOT(contextMenuRequested(const QPoint &)));
    connect(this, SIGNAL(activated(const QModelIndex &)),
            this, SLOT(bookmarkActivated(const QModelIndex &)));

    setHidden(true);
    setToolButtonStyle(Qt::ToolButtonTextOnly);
}

void BookmarksToolBar::contextMenuRequested(const QPoint &position)
{
    QAction *action = actionAt(position);

    QMenu menu;

    if (action) {
        QVariant variant = action->data();
        Q_ASSERT(variant.canConvert<QModelIndex>());

        QAction *menuAction = 0;

        if (!action->menu()) {
            menuAction = menu.addAction(tr("Open"), this, SLOT(openBookmarkInCurrentTab()));
            menuAction->setData(variant);

            menuAction = menu.addAction(tr("Open in New &Tab"), this, SLOT(openBookmarkInNewTab()));
            menuAction->setData(variant);

            menu.addSeparator();
        }

        menuAction = menu.addAction(tr("Remove"), this, SLOT(removeBookmark()));
        menuAction->setData(variant);

        menu.addSeparator();
    }

    menu.addAction(tr("Add Bookmark..."), this, SLOT(newBookmark()));
    menu.addAction(tr("Add Folder..."), this, SLOT(newFolder()));

    menu.exec(QCursor::pos());
}

void BookmarksToolBar::bookmarkActivated(const QModelIndex &index)
{
    Q_ASSERT(index.isValid());

    emit openUrl(
        index.data(BookmarksModel::UrlRole).toUrl(),
        index.data(Qt::DisplayRole).toString());
}

void BookmarksToolBar::openBookmark()
{
    QModelIndex index = ModelToolBar::index(qobject_cast<QAction*>(sender()));

    emit openUrl(
        index.data(BookmarksModel::UrlRole).toUrl(),
        index.data(Qt::DisplayRole).toString());
}

void BookmarksToolBar::openBookmarkInCurrentTab()
{
    QModelIndex index = ModelToolBar::index(qobject_cast<QAction*>(sender()));

    emit openUrl(
        index.data(BookmarksModel::UrlRole).toUrl(),
        TabWidget::CurrentTab,
        index.data(Qt::DisplayRole).toString());
}

void BookmarksToolBar::openBookmarkInNewTab()
{
    QModelIndex index = ModelToolBar::index(qobject_cast<QAction*>(sender()));

    emit openUrl(
        index.data(BookmarksModel::UrlRole).toUrl(),
        TabWidget::NewTab,
        index.data(Qt::DisplayRole).toString());
}

void BookmarksToolBar::removeBookmark()
{
    QModelIndex index = ModelToolBar::index(qobject_cast<QAction*>(sender()));

    m_bookmarksModel->removeRow(index.row(), rootIndex());
}

void BookmarksToolBar::newBookmark()
{
    AddBookmarkDialog dialog;
    dialog.setCurrentIndex(rootIndex());
    dialog.exec();
}

void BookmarksToolBar::newFolder()
{
    AddBookmarkDialog dialog;
    dialog.setCurrentIndex(rootIndex());
    dialog.setFolder(true);
    dialog.exec();
}

ModelMenu *BookmarksToolBar::createMenu()
{
    BookmarksMenu *menu = new BookmarksMenu(this);
    connect(menu, SIGNAL(openUrl(const QUrl&, const QString&)),
            this, SIGNAL(openUrl(const QUrl&, const QString&)));
    connect(menu, SIGNAL(openUrl(const QUrl&, TabWidget::OpenUrlIn, const QString &)),
            this, SIGNAL(openUrl(const QUrl&, TabWidget::OpenUrlIn, const QString &)));
    return menu;
}


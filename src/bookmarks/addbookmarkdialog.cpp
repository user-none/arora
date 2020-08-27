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

/****************************************************************************
**
** Copyright (C) 2008-2008 Trolltech ASA. All rights reserved.
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

#include "addbookmarkdialog.h"

#include "bookmarknode.h"
#include "bookmarksmanager.h"
#include "bookmarksmodel.h"
#include "browserapplication.h"

#include <qheaderview.h>
#include <qtreeview.h>

AddBookmarkProxyModel::AddBookmarkProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

int AddBookmarkProxyModel::columnCount(const QModelIndex &parent) const
{
    return qMin(1, QSortFilterProxyModel::columnCount(parent));
}

bool AddBookmarkProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    return sourceModel()->hasChildren(idx);
}

AddBookmarkDialog::AddBookmarkDialog(QWidget *parent, BookmarksManager *bookmarksManager)
    : QDialog(parent)
    , m_bookmarksManager(bookmarksManager)
    , m_addedNode(0)
    , m_proxyModel(0)
    , m_addFolder(false)
{
    setWindowFlags(Qt::Sheet);
    setupUi(this);

    if (!m_bookmarksManager)
        m_bookmarksManager = BrowserApplication::bookmarksManager();

    m_proxyModel = new AddBookmarkProxyModel(this);
    BookmarksModel *model = m_bookmarksManager->bookmarksModel();
    m_proxyModel->setSourceModel(model);

    m_treeView = new QTreeView(this);
    m_treeView->setModel(m_proxyModel);
    m_treeView->expandAll();
    m_treeView->header()->setStretchLastSection(true);
    m_treeView->header()->hide();
    m_treeView->setItemsExpandable(false);
    m_treeView->setRootIsDecorated(false);
    m_treeView->setIndentation(10);
    m_treeView->show();

    location->setModel(m_proxyModel);
    location->setView(m_treeView);

    address->setInactiveText(tr("Url"));
    name->setInactiveText(tr("Title"));

    resize(sizeHint());
}

void AddBookmarkDialog::setUrl(const QString &url)
{
    address->setText(url);
    address->setVisible(url.isEmpty());
    resize(sizeHint());
}

QString AddBookmarkDialog::url() const
{
    return address->text();
}

void AddBookmarkDialog::setTitle(const QString &title)
{
    name->setText(title);
}

QString AddBookmarkDialog::title() const
{
    return name->text();
}

void AddBookmarkDialog::setCurrentIndex(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    QModelIndex proxyIndex = m_proxyModel->mapFromSource(index);
    m_treeView->setCurrentIndex(proxyIndex);
    location->setCurrentIndex(proxyIndex.row());
}

QModelIndex AddBookmarkDialog::currentIndex() const
{
    QModelIndex index = location->view()->currentIndex();
    index = m_proxyModel->mapToSource(index);
    return index;
}

void AddBookmarkDialog::setFolder(bool addFolder)
{
    m_addFolder = addFolder;

    if (addFolder) {
        setWindowTitle(tr("Add Folder"));
        address->setVisible(false);
    } else {
        setWindowTitle(tr("Add Bookmark"));
        address->setVisible(true);
    }

    resize(sizeHint());
}

bool AddBookmarkDialog::isFolder() const
{
    return m_addFolder;
}

BookmarkNode *AddBookmarkDialog::addedNode() const
{
    return m_addedNode;
}

void AddBookmarkDialog::accept()
{
    if ((!m_addFolder && address->text().isEmpty()) || name->text().isEmpty()) {
        QDialog::accept();
        return;
    }

    QModelIndex index = currentIndex();
    if (!index.isValid())
        index = m_bookmarksManager->bookmarksModel()->index(0, 0);
    BookmarkNode *parent = m_bookmarksManager->bookmarksModel()->node(index);

    BookmarkNode::Type type = (m_addFolder) ? BookmarkNode::Folder : BookmarkNode::Bookmark;
    BookmarkNode *bookmark = new BookmarkNode(type);
    bookmark->title = name->text();
    if (!m_addFolder)
        bookmark->url = address->text();

    m_bookmarksManager->addBookmark(parent, bookmark);
    m_addedNode = bookmark;

    QDialog::accept();
}


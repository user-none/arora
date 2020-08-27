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

#include "treesortfilterproxymodel.h"
#include "ui_treesortfilterproxymodeldialog.h"

#include <qapplication.h>
#include <qdirmodel.h>
#include <qtreeview.h>

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    QDialog dialog;
    Ui_TreeSortFilterProxyModelDialog ui;
    ui.setupUi(&dialog);

    TreeSortFilterProxyModel *proxy = new TreeSortFilterProxyModel(&dialog);
    dialog.connect(ui.search, SIGNAL(textChanged(const QString &)),
            proxy, SLOT(setFilterRegExp(const QString &)));
    QDirModel *dirModel = new QDirModel(&dialog);
    proxy->setSourceModel(dirModel);
    ui.treeView->setModel(proxy);
    ui.treeView->setRootIndex(proxy->mapFromSource(dirModel->index(QDir::homePath())));
    dialog.show();

    return application.exec();
}


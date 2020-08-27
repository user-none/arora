/**
 * Copyright (c) 2009, Benjamin C. Meyer
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <qapplication.h>
#include <qstringlistmodel.h>
#include <qstandarditemmodel.h>

#include "ui_editview.h"

class Window : public QDialog, public Ui_Dialog
{
    Q_OBJECT

public:
    Window();
};

Window::Window()
    : QDialog()
{
    setupUi(this);
    QStringList list;
    list << "a" << "b" << "c";
    listView->setModel(new QStringListModel(list));
    tableView->setModel(new QStringListModel(list));
    connect(listViewRemoveAllButton, SIGNAL(clicked()), listView, SLOT(removeAll()));
    connect(tableViewRemoveAllButton, SIGNAL(clicked()), tableView, SLOT(removeAll()));
    connect(treeViewRemoveAllButton, SIGNAL(clicked()), treeView, SLOT(removeAll()));

    QStandardItemModel *model = new QStandardItemModel(this);
    QStandardItem *parentItem = model->invisibleRootItem();
    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("item %0").arg(i));
        parentItem->appendRow(item);
        item = new QStandardItem(QString("item %0").arg(i * 2));
        parentItem->appendRow(item);
        parentItem = item;
    }
    treeView->setModel(model);
    treeView->expandAll();
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Window window;
    window.show();
    return app.exec();
}

#include "main_editview.moc"


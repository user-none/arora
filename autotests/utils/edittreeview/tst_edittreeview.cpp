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

#include <qtest.h>

#include <edittreeview.h>
#include <qstandarditemmodel.h>

class tst_EditTreeView : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void edittreeview_data();
    void edittreeview();

    void keyPressEvent_data();
    void keyPressEvent();

    void removeAll();

    void removeSelected_data();
    void removeSelected();
};

// Subclass that exposes the protected functions.
class SubEditTreeView : public EditTreeView
{

public:
    void addModel() {
        QStandardItemModel *model = new QStandardItemModel(this);
        QStandardItem *parentItem = model->invisibleRootItem();
        for (int i = 0; i < 4; ++i) {
            QStandardItem *item = 0;
            for (int j = 0; j < 4; ++j) {
                item = new QStandardItem(QString("item %0").arg(j));
                parentItem->appendRow(item);
            }
            parentItem = item;
        }
        setModel(model);
    }

};

// This will be called before the first test function is executed.
// It is only called once.
void tst_EditTreeView::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_EditTreeView::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_EditTreeView::init()
{
}

// This will be called after every test function.
void tst_EditTreeView::cleanup()
{
}

void tst_EditTreeView::edittreeview_data()
{
}

void tst_EditTreeView::edittreeview()
{
    SubEditTreeView view;
    QTest::keyClick(&view, Qt::Key_Space);
    QTest::keyClick(&view, Qt::Key_Delete);
    view.removeAll();
    view.removeSelected();
}

Q_DECLARE_METATYPE(Qt::Key)
void tst_EditTreeView::keyPressEvent_data()
{
    QTest::addColumn<Qt::Key>("key");
    QTest::newRow("space") << Qt::Key_Space;
    QTest::newRow("delete") << Qt::Key_Delete;
}

// public void keyPressEvent(QKeyEvent *event)
void tst_EditTreeView::keyPressEvent()
{
    QFETCH(Qt::Key, key);

    SubEditTreeView view;
    view.addModel();

    QModelIndex idx = view.model()->index(0, 0);
    view.selectionModel()->select(idx, QItemSelectionModel::SelectCurrent);
    int oldCount = view.model()->rowCount();
    QTest::keyClick(&view, key);
    if (key == Qt::Key_Delete)
        QCOMPARE(oldCount - 1, view.model()->rowCount());
    else
        QCOMPARE(oldCount, view.model()->rowCount());
}

// public void removeAll()
void tst_EditTreeView::removeAll()
{
    SubEditTreeView view;
    view.addModel();
    view.removeAll();
    QCOMPARE(view.model()->rowCount(), 0);
}

typedef QList<int> IntList;
Q_DECLARE_METATYPE(IntList)
void tst_EditTreeView::removeSelected_data()
{
    QTest::addColumn<int>("selectParent");
    QTest::addColumn<IntList>("select");
    QTest::addColumn<bool>("upParent");
    QTest::addColumn<int>("isSelected");

    QTest::newRow("first in list") << 3 << (IntList() << 0) << false << 0;
    QTest::newRow("last in list")  << 3 << (IntList() << 3) << false << 2;
    QTest::newRow("all in list")  << 3 << (IntList() << 0 << 1 << 2 << 3) << true << 3;
}

// public void removeSelected()
void tst_EditTreeView::removeSelected()
{
    QFETCH(int, selectParent);
    QFETCH(IntList, select);
    QFETCH(bool, upParent);
    QFETCH(int, isSelected);

    SubEditTreeView view;
    view.show();
    view.addModel();
    QAbstractItemModel *model = view.model();
    view.expandAll();
    QModelIndex parent = model->index(selectParent, 0);
    while (!select.isEmpty()) {
        QModelIndex idx = model->index(select.takeLast(), 0, parent);
        view.selectionModel()->select(idx, QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);
        view.setCurrentIndex(idx);
        view.removeSelected();
    }

    if (upParent)
        parent = parent.parent();

    QModelIndex idx = model->index(isSelected, 0, parent);

    QCOMPARE(view.currentIndex(), idx);
    QCOMPARE(view.selectionModel()->selectedRows().count(), 1);
    QCOMPARE(view.selectionModel()->selectedRows().first(), idx);
}

QTEST_MAIN(tst_EditTreeView)
#include "tst_edittreeview.moc"


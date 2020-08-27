/*
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
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
#include <qstringlistmodel.h>

#include <editlistview.h>

class tst_EditListView : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void editlistview_data();
    void editlistview();

    void keyPressEvent_data();
    void keyPressEvent();
    void removeAll_data();
    void removeAll();
    void removeSelected_data();
    void removeSelected();
};

// Subclass that exposes the protected functions.
class SubEditListView : public EditListView
{
public:

};

// This will be called before the first test function is executed.
// It is only called once.
void tst_EditListView::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_EditListView::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_EditListView::init()
{
}

// This will be called after every test function.
void tst_EditListView::cleanup()
{
}

void tst_EditListView::editlistview_data()
{
}

Q_DECLARE_METATYPE(Qt::Key)
void tst_EditListView::editlistview()
{
    SubEditListView view;
    QTest::keyClick(&view, Qt::Key_Delete);
    QTest::keyClick(&view, Qt::Key_R);
    view.removeAll();
    view.removeSelected();
}

void tst_EditListView::keyPressEvent_data()
{
    QTest::addColumn<Qt::Key>("key");
    QTest::addColumn<QStringList>("predata");
    QTest::addColumn<QStringList>("postdata");
    QStringList predata;
    predata << "a";
    QTest::newRow("R") << Qt::Key_R << predata << predata;
    QTest::newRow("delete") << Qt::Key_Delete << predata << QStringList();
}

// public void keyPressEvent(QKeyEvent *event)
void tst_EditListView::keyPressEvent()
{
    QFETCH(Qt::Key, key);
    QFETCH(QStringList, predata);
    QFETCH(QStringList, postdata);

    SubEditListView view;
    QStringListModel *model = new QStringListModel(predata);
    view.setModel(model);
    view.setCurrentIndex(model->index(0, 0));
    QTest::keyClick(&view, key);
    QCOMPARE(model->stringList(), postdata);
}

void tst_EditListView::removeAll_data()
{
    QTest::addColumn<QStringList>("data");
    QTest::newRow("0") << QStringList();
    QTest::newRow("3") << (QStringList() << "x" << "y" << "z");
}

// public void removeAll()
void tst_EditListView::removeAll()
{
    QFETCH(QStringList, data);

    SubEditListView view;
    view.setModel(new QStringListModel(data));

    view.removeAll();
    QCOMPARE(view.model()->rowCount(), 0);
}

void tst_EditListView::removeSelected_data()
{
    QTest::addColumn<Qt::Key>("key");
    QTest::addColumn<QStringList>("predata");
    QTest::addColumn<QStringList>("postdata");
    QStringList predata;
    predata << "a";
    QTest::newRow("R") << Qt::Key_R << predata << predata;
    QTest::newRow("delete") << Qt::Key_Delete << predata << QStringList();
}

// public void removeSelected()
void tst_EditListView::removeSelected()
{
    QFETCH(Qt::Key, key);
    QFETCH(QStringList, predata);
    QFETCH(QStringList, postdata);

    SubEditListView view;
    QStringListModel *model = new QStringListModel(predata);
    view.setModel(model);
    view.setCurrentIndex(model->index(0, 0));
    QTest::keyClick(&view, key);
    QCOMPARE(model->stringList(), postdata);
}

QTEST_MAIN(tst_EditListView)
#include "tst_editlistview.moc"


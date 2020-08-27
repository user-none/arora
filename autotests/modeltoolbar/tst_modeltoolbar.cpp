/*
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

#include <QtTest/QtTest>
#include <modeltoolbar.h>
#include "qtry.h"

#include "browserapplication.h"
#include "modelmenu.h"

#include <qaction.h>
#include <qcolor.h>
#include <qstandarditemmodel.h>
#include <qstringlistmodel.h>

class tst_ModelToolBar : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void modeltoolbar();
    void index();
    void model();
    void rootIndex();
    void activated_data();
    void activated();
    void build_data();
    void build();
    void showHide();
};

// Subclass that exposes the protected functions.
class SubModelToolBar : public ModelToolBar
{
public:
    void call_activated(QModelIndex const &index)
        { return SubModelToolBar::activated(index); }

    void call_build()
        { SubModelToolBar::build(); resize(sizeHint()); }

    ModelMenu *call_createMenu()
        { return SubModelToolBar::createMenu(); }
};

class ColorModel : public QAbstractItemModel
{
public:
    ColorModel(QObject *parent = 0)
        : QAbstractItemModel(parent)
    {
        m_colours = QColor::colorNames();
    }

    bool hasChildren(const QModelIndex &parent = QModelIndex()) const
    {
        Q_UNUSED(parent)
        return false;
    }

    int rowCount(const QModelIndex &parent = QModelIndex()) const
    {
        if (parent.isValid())
            return 0;
        return m_colours.count();
    }

    int columnCount(const QModelIndex &parent = QModelIndex()) const
    {
        if (parent.isValid())
            return 0;
        return 1;
    }

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const
    {
        if (index.parent().isValid() || index.column() > 0)
            return QVariant();

        switch (role) {
        case Qt::DisplayRole:
            return m_colours.at(index.row());
        break;
        default:
            return QVariant();
        break;
        }
    }

    QModelIndex parent(const QModelIndex &index) const
    {
        Q_UNUSED(index);
        return QModelIndex();
    }

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const
    {
        if (parent.isValid())
            return QModelIndex();
        return createIndex(row, column);
    }

private:
    QStringList m_colours;
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_ModelToolBar::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_ModelToolBar::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_ModelToolBar::init()
{
}

// This will be called after every test function.
void tst_ModelToolBar::cleanup()
{
}

void tst_ModelToolBar::modeltoolbar()
{
    SubModelToolBar bar;
    QCOMPARE(bar.model(), (QAbstractItemModel*)0);
    QCOMPARE(bar.index(0), QModelIndex());
    QCOMPARE(bar.rootIndex(), QModelIndex());
    bar.setModel(new QStandardItemModel(&bar));
    QVERIFY(bar.model());
    bar.show();
    bar.hide();
    bar.call_build();
}

// public static QModelIndex index(QAction *action)
void tst_ModelToolBar::index()
{
    SubModelToolBar bar;

    ColorModel model;
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);

    bar.show();

    QList<QAction*> actions = bar.actions();
    QVERIFY(!actions.isEmpty());

    for (int i = 0; i < model.rowCount(); ++i) {
        QModelIndex index = model.index(i, 0);
        QAction *action = actions.at(i);
        QVERIFY(action);
        QVERIFY(bar.index(action).isValid());
        QCOMPARE(index.data(Qt::DisplayRole).toString(), action->text());
        QCOMPARE(index.data(Qt::DecorationRole).value<QIcon>(), action->icon());
        QCOMPARE(model.hasChildren(index), (bool)action->menu());

        if (action->menu()) {
            ModelMenu *menu = static_cast<ModelMenu*>(action->menu());
            QVERIFY(menu);
            QCOMPARE(bar.index(action), menu->rootIndex());
            QCOMPARE(bar.model(), menu->model());
            QCOMPARE(model.rowCount(bar.index(action)), menu->actions().count());
        }
    }
}

// public QAbstractItemModel *model() const
void tst_ModelToolBar::model()
{
    SubModelToolBar bar;
    QCOMPARE(bar.model(), (QAbstractItemModel*)0);
    bar.setModel(0);
    QCOMPARE(bar.model(), (QAbstractItemModel*)0);

    ColorModel model;
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);
}

// public QModelIndex rootIndex() const
void tst_ModelToolBar::rootIndex()
{
    SubModelToolBar bar;
    QCOMPARE(bar.rootIndex(), QModelIndex());
    bar.setRootIndex(QModelIndex());
    QCOMPARE(bar.rootIndex(), QModelIndex());

    ColorModel model;
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);
    QVERIFY(model.rowCount(QModelIndex()) > 0);
    bar.setRootIndex(model.index(0, 0, QModelIndex()));
    QCOMPARE(bar.rootIndex(), model.index(0, 0, QModelIndex()));
}

void tst_ModelToolBar::activated_data()
{
    QTest::addColumn<QStringList>("items");

    QTest::newRow("null") << QStringList();
    QTest::newRow("foobarbaz") << (QStringList() << "foo" << "bar" << "baz" << "bar" << "foo");
    QTest::newRow("colors") << QColor::colorNames();
    QTest::newRow("environment") << QProcess::systemEnvironment();
}

// protected void activated(QModelIndex const &index)
void tst_ModelToolBar::activated()
{
    QFETCH(QStringList, items);

    SubModelToolBar bar;

    QStringListModel model(items);
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);

    bar.show();
    bar.call_build();
    QCOMPARE(bar.actions().isEmpty(), items.isEmpty());

    QRect rect = bar.childrenRect();
    QPoint point = rect.topLeft();

    QSignalSpy spy(&bar, SIGNAL(activated(QModelIndex const&)));

    while (rect.contains(point)) {
        QAction *action = bar.actionAt(point);
        QWidget *widget = bar.childAt(point);
        if (action) {
            QVERIFY(widget);
            QVERIFY(bar.index(action).isValid());
        } else {
            QVERIFY(!widget);
            QVERIFY(!bar.index(action).isValid());
        }

        if (!widget)
            widget = &bar;

        QTest::mouseClick(widget, Qt::LeftButton, Qt::NoModifier, bar.mapToGlobal(point));
        QTest::mouseClick(widget, Qt::MidButton, Qt::ShiftModifier, bar.mapToGlobal(point));
        QTRY_COMPARE(spy.count(), (bool)action * 2);

        if (spy.count())
            QCOMPARE(spy.at(0).at(0).value<QModelIndex>(), bar.index(action));

        if (action) {
            QCOMPARE(BrowserApplication::instance()->eventMouseButtons(), Qt::MidButton);
            QCOMPARE(BrowserApplication::instance()->eventKeyboardModifiers(), Qt::ShiftModifier);
        }

        spy.clear();

        point.setX(point.x() + 50);
    }
}

void tst_ModelToolBar::build_data()
{
    QTest::addColumn<QStringList>("items");

    QTest::newRow("null") << QStringList();
    QTest::newRow("foobarbaz") << (QStringList() << "foo" << "bar" << "baz" << "bar" << "foo");
    QTest::newRow("colors") << QColor::colorNames();
    QTest::newRow("environment") << QProcess::systemEnvironment();
}

// protected void build()
void tst_ModelToolBar::build()
{
    QFETCH(QStringList, items);

    SubModelToolBar bar;

    QStringListModel model(items);
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);
    QCOMPARE(items.count(), model.rowCount());

    QVERIFY(bar.actions().isEmpty());
    bar.call_build();
    QCOMPARE(bar.actions().count(), items.count());
    bar.call_build();
    QCOMPARE(bar.actions().count(), model.rowCount());
}

void tst_ModelToolBar::showHide()
{
    SubModelToolBar bar;

    ColorModel model;
    bar.setModel(&model);
    QCOMPARE(bar.model(), &model);

    QVERIFY(bar.actions().isEmpty());

    bar.show();
    QCOMPARE(bar.actions().count(), model.rowCount());
    bar.hide();
    QVERIFY(bar.actions().isEmpty());
    bar.show();
    QCOMPARE(bar.actions().count(), model.rowCount());
}

QTEST_MAIN(tst_ModelToolBar)
#include "tst_modeltoolbar.moc"


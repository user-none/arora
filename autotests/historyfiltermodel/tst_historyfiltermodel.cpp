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

#include <QtTest/QtTest>
#include <history.h>
#include <historymanager.h>

class tst_HistoryFilterModel : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void historyfiltermodel_data();
    void historyfiltermodel();

    void historyContains_data();
    void historyContains();

    void setSourceModel();

    void historyLocation_data();
    void historyLocation();

    void addRow_data();
    void addRow();

    void removeRows_data();
    void removeRows();
};

// Subclass that exposes the protected functions.
class SubHistoryFilterModel : public HistoryFilterModel
{
public:
    SubHistoryFilterModel(QObject *parent = 0)
        : HistoryFilterModel(0, parent)
    {
        history = new HistoryManager(this);
        historyModel = new HistoryModel(history, this);
        setSourceModel(historyModel);
        history->setDaysToExpire(-1);
    }

    HistoryModel *historyModel;
    HistoryManager *history;
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_HistoryFilterModel::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_HistoryFilterModel::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_HistoryFilterModel::init()
{
}

// This will be called after every test function.
void tst_HistoryFilterModel::cleanup()
{
}

void tst_HistoryFilterModel::historyfiltermodel_data()
{
}

void tst_HistoryFilterModel::historyfiltermodel()
{
    SubHistoryFilterModel model;
    QCOMPARE(model.historyContains(QString()), false);
    model.setSourceModel(0);
    model.historyLocation(QString());
}

typedef QList<HistoryEntry> HistoryList;
Q_DECLARE_METATYPE(HistoryList)
Q_DECLARE_METATYPE(HistoryEntry)

HistoryList makeHistoryList(int count)
{
    HistoryList list;
    QDateTime dateTime = QDateTime::currentDateTime();
    for (int i = 0; i < count; ++i) {
        HistoryEntry item;
        QString url = QString("http://%1host-%2.com/")
            .arg(qrand() % 2 ? "www." : "")
            .arg(QString::number(i));
        item.url = url;
        item.title = QString("title %1").arg(i);
        item.dateTime = dateTime;
        list.append(item);
        dateTime = dateTime.addSecs(-1 * qrand() % (60 * 60));
    }
    return list;
}

void tst_HistoryFilterModel::historyContains_data()
{
    QTest::addColumn<HistoryList>("list");
    QTest::addColumn<QString>("url");
    QTest::addColumn<bool>("historyContains");

    QTest::newRow("null") << HistoryList() << QString("foo") << false;

    HistoryList list1 = makeHistoryList(1);
    QTest::newRow("one") << list1 << list1.at(0).url << true;

    HistoryList list2 = makeHistoryList(5);
    QTest::newRow("many-0") << list2 << list2.at(0).url << true;
    QTest::newRow("many-1") << list2 << list2.at(1).url << true;
    QTest::newRow("many-2") << list2 << list2.at(4).url << true;
    QTest::newRow("many-3") << list2 << QString("foo") << false;
}

// public bool historyContains(QString const &url) const
void tst_HistoryFilterModel::historyContains()
{
    QFETCH(HistoryList, list);
    QFETCH(QString, url);
    QFETCH(bool, historyContains);

    SubHistoryFilterModel model;
    model.history->setHistory(list);

    QCOMPARE(model.historyContains(url), historyContains);
}

// public void setSourceModel(QAbstractItemModel *sourceModel)
void tst_HistoryFilterModel::setSourceModel()
{
    SubHistoryFilterModel model;

    HistoryManager manager;
    HistoryModel *m = new HistoryModel(&manager, &model);
    model.setSourceModel(m);
    QCOMPARE(model.sourceModel(), m);
}

Q_DECLARE_METATYPE(QModelIndex)
void tst_HistoryFilterModel::historyLocation_data()
{
    QTest::addColumn<HistoryList>("history");
    QTest::addColumn<int>("source_row");
    QTest::addColumn<int>("historyLocation");

    QTest::newRow("null") << HistoryList() << 0 << 0;

    HistoryList list1 = makeHistoryList(1);
    QTest::newRow("one-0") << list1 << 0 << 0;
    QTest::newRow("one-1") << list1 << 1 << 0;

    HistoryList list2 = makeHistoryList(5);
    list2.insert(1, list2.at(1));
    list2[1].dateTime = list2[1].dateTime.addSecs(1);
    QTest::newRow("many-0") << list2 << 0 << 0;
    QTest::newRow("many-1") << list2 << 1 << 1;
    QTest::newRow("many-2") << list2 << 2 << 1;
    QTest::newRow("many-3") << list2 << 3 << 3;
}

// protected int historyLocation(int source_row) const
void tst_HistoryFilterModel::historyLocation()
{
    QFETCH(HistoryList, history);
    QFETCH(int, source_row);
    QFETCH(int, historyLocation);

    SubHistoryFilterModel model;
    model.history->setHistory(history);
    QCOMPARE(model.history->history(), history);

    QCOMPARE(model.historyLocation(history.value(source_row).url), historyLocation);
}

typedef QList<int> EnabledList;
Q_DECLARE_METATYPE(EnabledList)

void tst_HistoryFilterModel::addRow_data()
{
    QTest::addColumn<HistoryList>("history");
    QTest::addColumn<QString>("item");
    QTest::addColumn<EnabledList>("enabledList");

    QTest::newRow("null") << HistoryList() << "http://foo.com" << (EnabledList() << 0);

    HistoryList list1 = makeHistoryList(1);
    QTest::newRow("one-0") << list1 << "http://foo.com" << (EnabledList() << 0 << 1);
    QTest::newRow("one-1") << list1 << list1[0].url << (EnabledList() << 0 << 0);

    HistoryList list2 = makeHistoryList(2);
    QTest::newRow("two-0") << list2 << "http://foo.com" << (EnabledList() << 0 << 1 << 2);
    QTest::newRow("two-1") << list2 << list2[0].url << (EnabledList() << 0 << 0 << 2);
    QTest::newRow("two-2") << list2 << list2[1].url << (EnabledList() << 0 << 1 << 0);

    HistoryList list3 = makeHistoryList(3);
    QTest::newRow("three-0") << list3 << list3[0].url << (EnabledList() << 0 << 0 << 2 << 3);
    QTest::newRow("three-1") << list3 << list3[1].url << (EnabledList() << 0 << 1 << 0 << 3);
    QTest::newRow("three-2") << list3 << list3[2].url << (EnabledList() << 0 << 1 << 2 << 0);

    HistoryList list4 = makeHistoryList(5);
    list4.insert(1, list4.at(1));
    list4[1].dateTime = list4[1].dateTime.addSecs(1);
    QTest::newRow("many-0") << list4
        << list4[0].url
        << (EnabledList() << 0 << 0 << 2 << 2 << 4 << 5 << 6);

    HistoryList list5 = makeHistoryList(3);
    // 0, 1, 2
    list5.insert(1, list5.at(1));
    list5[1].dateTime = list5[1].dateTime.addSecs(1);
    // 0, 1, 1, 2
    list5.insert(4, list5.at(1));
    list5[4].dateTime = list5[3].dateTime.addSecs(-81);
    // 0, 1, 1, 2, 1

    // ?, 1, 2, 2, 4, 2
    QTest::newRow("many-1") << list5
        << list5[1].url
        << (EnabledList() << 0 << 1 << 0 << 0 << 4 << 0);
}

void tst_HistoryFilterModel::addRow()
{
    QFETCH(HistoryList, history);
    QFETCH(QString, item);
    QFETCH(EnabledList, enabledList);

    SubHistoryFilterModel model;
    model.history->setHistory(history);
    QCOMPARE(model.history->history(), history);
    QCOMPARE(model.history->history().count(), history.count());
    model.history->addHistoryEntry(item);

    int currentRow = 0;
    QCOMPARE(model.history->history().count(), enabledList.count());
    for (int i = 0; i < enabledList.count(); ++i) {
        QVERIFY(i < model.history->history().count());
        QCOMPARE(model.historyLocation(model.history->history().value(i).url), enabledList[i]);

        if (i > 0
            && enabledList[i] != 0
            && (enabledList[i-1] < enabledList[i]
            || enabledList[i-1] == 0))
            ++currentRow;
        if (currentRow >= model.rowCount() || enabledList[i] == 0)
            continue;
        QModelIndex idx = model.index(currentRow, 1);
        QCOMPARE(idx.data(HistoryModel::UrlStringRole).toString(), model.history->history().value(i).url);

        QModelIndex historyIndex = model.historyModel->index(i, 1);
        QModelIndex filterIndex = model.mapFromSource(historyIndex);
        if (filterIndex.isValid()) {
            QCOMPARE(filterIndex.data().toString(), historyIndex.data().toString());
        }
    }

    if (history.count() > 0)
        for (int i = 0; i < qrand() % 15; ++i)
            model.history->addHistoryEntry(history[qrand() % history.count()].url);
    QStringList urls;
    for (int i = 0; i < model.rowCount(); ++i) {
        QModelIndex idx = model.index(i, 0);
        QString url = idx.data(HistoryModel::UrlStringRole).toString();
        QVERIFY(!urls.contains(url));
        urls.append(url);
    }
}

void tst_HistoryFilterModel::removeRows_data()
{
    QTest::addColumn<HistoryList>("history");
    QTest::addColumn<int>("start");
    QTest::addColumn<int>("end");
    QTest::addColumn<int>("count");

    QTest::newRow("null") << HistoryList() << 0 << 0 << 0;

    QTest::newRow("first-0") << makeHistoryList(5) << 0 << 1 << 4;
    QTest::newRow("first-1") << makeHistoryList(5) << 1 << 1 << 4;
    QTest::newRow("first-2") << makeHistoryList(5) << 0 << 2 << 3;
    QTest::newRow("first-3") << makeHistoryList(5) << 0 << 5 << 0;

    HistoryList list = makeHistoryList(5);
    list[1].url = list[0].url;
    QTest::newRow("dupe-0") << list << 0 << 0 << 4;
    QTest::newRow("dupe-1") << list << 0 << 1 << 4;
    QTest::newRow("dupe-2") << list << 1 << 1 << 3;
    QTest::newRow("dupe-3") << list << 0 << 4 << 0;
}

void tst_HistoryFilterModel::removeRows()
{
    QFETCH(HistoryList, history);
    QFETCH(int, start);
    QFETCH(int, end);
    QFETCH(int, count);

    SubHistoryFilterModel model;
    model.history->setHistory(history);
    model.removeRows(start, end);
    QCOMPARE(model.rowCount(), count);
}

QTEST_MAIN(tst_HistoryFilterModel)
#include "tst_historyfiltermodel.moc"


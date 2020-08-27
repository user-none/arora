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

#include "opensearchreader.h"
#include "opensearchengine.h"

class tst_OpenSearchReader : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void read_data();
    void read();
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_OpenSearchReader::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_OpenSearchReader::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_OpenSearchReader::init()
{
}

// This will be called after every test function.
void tst_OpenSearchReader::cleanup()
{
}

Q_DECLARE_METATYPE(OpenSearchEngine::Parameters)
void tst_OpenSearchReader::read_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<bool>("valid");
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("description");
    QTest::addColumn<QString>("searchUrlTemplate");
    QTest::addColumn<QString>("suggestionsUrlTemplate");
    QTest::addColumn<QString>("imageUrl");
    QTest::addColumn<OpenSearchEngine::Parameters>("searchParameters");
    QTest::addColumn<OpenSearchEngine::Parameters>("suggestionsParameters");
    QTest::addColumn<QString>("searchMethod");
    QTest::addColumn<QString>("suggestionsMethod");

    QTest::newRow("null") << QString(":/doesNotExist") << false << QString() << QString() << QString() << QString()
            << QString() << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters() << QString("get") << QString("get");

    QTest::newRow("testfile1") << QString(":/testfile1.xml") << true << QString("Wikipedia (en)")
            << QString("Full text search in the English Wikipedia") << QString("http://en.wikipedia.org/bar")
            << QString("http://en.wikipedia.org/foo") << QString("http://en.wikipedia.org/favicon.ico")
            << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters() << QString("post") << QString("get");

    QTest::newRow("testfile2") << QString(":/testfile2.xml") << false << QString("Wikipedia (en)")
            << QString() << QString() << QString("http://en.wikipedia.org/foo") << QString("http://en.wikipedia.org/favicon.ico")
            << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters() << QString("get") << QString("get");

    QTest::newRow("testfile3") << QString(":/testfile3.xml") << true << QString("GitHub") << QString("Search GitHub")
            << QString("http://github.com/search") << QString("http://github.com/suggestions") << QString()
            << (OpenSearchEngine::Parameters() << OpenSearchEngine::Parameter(QString("q"), QString("{searchTerms}"))
                                               << OpenSearchEngine::Parameter(QString("b"), QString("foo")))
            << (OpenSearchEngine::Parameters() << OpenSearchEngine::Parameter(QString("bar"), QString("baz")))
            << QString("get") << QString("post");

    QTest::newRow("testfile4") << QString(":/testfile4.xml") << true << QString("Google") << QString("Google Web Search")
            << QString("http://www.google.com/search?bar") << QString("http://suggestqueries.google.com/complete/foo")
            << QString("http://www.google.com/favicon.ico") << OpenSearchEngine::Parameters()
            << OpenSearchEngine::Parameters() << QString("get") << QString("get");

    QTest::newRow("testfile5") << QString(":/testfile5.xml") << false << QString() << QString() << QString() << QString()
            << QString() << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters() << QString("get") << QString("get");

    QTest::newRow("testfile6") << QString(":/testfile6.xml") << false << QString() << QString() << QString() << QString()
            << QString() << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters()  << QString("get") << QString("get");

    QTest::newRow("testfile7") << QString(":/testfile7.xml") << false << QString() << QString() << QString() << QString()
            << QString() << OpenSearchEngine::Parameters() << OpenSearchEngine::Parameters()  << QString("get") << QString("get");
}

void tst_OpenSearchReader::read()
{
    QFETCH(QString, fileName);
    QFETCH(bool, valid);
    QFETCH(QString, name);
    QFETCH(QString, description);
    QFETCH(QString, searchUrlTemplate);
    QFETCH(QString, suggestionsUrlTemplate);
    QFETCH(QString, imageUrl);
    QFETCH(OpenSearchEngine::Parameters, searchParameters);
    QFETCH(OpenSearchEngine::Parameters, suggestionsParameters);
    QFETCH(QString, searchMethod);
    QFETCH(QString, suggestionsMethod);

    QFile file(fileName);
    file.open(QIODevice::ReadOnly);
    OpenSearchReader reader;
    OpenSearchEngine *engine = reader.read(&file);

    QCOMPARE(engine->isValid(), valid);
    QCOMPARE(engine->name(), name);
    QCOMPARE(engine->description(), description);
    QCOMPARE(engine->searchUrlTemplate(), searchUrlTemplate);
    QCOMPARE(engine->suggestionsUrlTemplate(), suggestionsUrlTemplate);
    QCOMPARE(engine->searchParameters(), searchParameters);
    QCOMPARE(engine->suggestionsParameters(), suggestionsParameters);
    QCOMPARE(engine->imageUrl(), imageUrl);
    QCOMPARE(engine->searchMethod(), searchMethod);
    QCOMPARE(engine->suggestionsMethod(), suggestionsMethod);

    delete engine;
}

QTEST_MAIN(tst_OpenSearchReader)

#include "tst_opensearchreader.moc"


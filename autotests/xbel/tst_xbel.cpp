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

#include "bookmarknode.h"
#include "xbelreader.h"
#include "xbelwriter.h"

class tst_Xbel : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void xbelreader_data();
    void xbelreader();

    void read_data();
    void read();

    void readProperly();

    void xbelwriter_data();
    void xbelwriter();

    void write_data();
    void write();

};

// Subclass that exposes the protected functions.
class SubXbelReader : public XbelReader
{

public:
};

// Subclass that exposes the protected functions.
class SubXbelWriter : public XbelWriter
{
public:

};

// This will be called before the first test function is executed.
// It is only called once.
void tst_Xbel::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_Xbel::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_Xbel::init()
{
}

// This will be called after every test function.
void tst_Xbel::cleanup()
{
}

void tst_Xbel::xbelreader_data()
{
}

void tst_Xbel::xbelreader()
{
    SubXbelReader reader;
    BookmarkNode *root = reader.read(QString());
    QVERIFY(root);
    QVERIFY(reader.error() == QXmlStreamReader::NoError);
    delete root;
}

Q_DECLARE_METATYPE(QXmlStreamReader::Error)
void tst_Xbel::read_data()
{
    QTest::addColumn<QString>("fileName");
    QTest::addColumn<QXmlStreamReader::Error>("error");
    QTest::newRow("null") << QString() << QXmlStreamReader::NoError;
    QTest::newRow("frank") << QString("frank.xbel") << QXmlStreamReader::NoError;
    QTest::newRow("frank") << QString("all.xbel") << QXmlStreamReader::NoError;
    QTest::newRow("bad") << QString("bad.xbel") << QXmlStreamReader::CustomError;
}

// public BookmarkNode *read(QString const &fileName)
void tst_Xbel::read()
{
    QFETCH(QString, fileName);
    QFETCH(QXmlStreamReader::Error, error);

    SubXbelReader reader;

    BookmarkNode *root = reader.read(fileName);
    QVERIFY(root);
    delete root;
    QCOMPARE(reader.error(), error);
}

void tst_Xbel::readProperly()
{
    SubXbelReader reader;
    BookmarkNode *root = reader.read("all.xbel");
    QCOMPARE(reader.error(), QXmlStreamReader::NoError);
    QList<BookmarkNode*>children = root->children();
    QCOMPARE(children.count(), 4);
    // null folder
    QCOMPARE(children[0]->children().count(), 0);
    QCOMPARE(children[0]->title, QString());
    QCOMPARE(children[0]->url, QString());
    QCOMPARE(children[0]->expanded, false);

    // folded = no
    QCOMPARE(children[1]->children().count(), 0);
    QCOMPARE(children[1]->title, QString());
    QCOMPARE(children[1]->url, QString());
    QCOMPARE(children[1]->expanded, true);
    QCOMPARE(children[1]->type(), BookmarkNode::Folder);

    QCOMPARE(children[2]->children().count(), 3);
    QCOMPARE(children[2]->title, QString("Folder Title"));
    QCOMPARE(children[2]->url, QString());
    QCOMPARE(children[2]->expanded, false);
    QCOMPARE(children[2]->type(), BookmarkNode::Folder);
    {
        children = children[2]->children();
        QCOMPARE(children[0]->children().count(), 0);
        QCOMPARE(children[0]->title, QString("Title"));
        QCOMPARE(children[0]->url, QString("http://www.foo.com/"));
        QCOMPARE(children[0]->expanded, false);
        QCOMPARE(children[0]->type(), BookmarkNode::Bookmark);

        QCOMPARE(children[1]->children().count(), 0);
        QCOMPARE(children[1]->title, QString("Title 2"));
        QCOMPARE(children[1]->url, QString("http://www.bar.com/"));
        QCOMPARE(children[1]->expanded, false);
        QCOMPARE(children[1]->type(), BookmarkNode::Bookmark);

        QCOMPARE(children[2]->title, QString("Title 3"));
    }

    children = root->children();
    QCOMPARE(children[3]->children().count(), 3);
    QCOMPARE(children[3]->title, QString("Has SubFolder"));
    QCOMPARE(children[3]->url, QString());
    QCOMPARE(children[3]->expanded, true);
    QCOMPARE(children[3]->type(), BookmarkNode::Folder);
    {
        children = children[3]->children();
        QCOMPARE(children[0]->children().count(), 0);
        QCOMPARE(children[0]->title, QString());
        QCOMPARE(children[0]->url, QString());
        QCOMPARE(children[0]->expanded, false);
        QCOMPARE(children[0]->type(), BookmarkNode::Separator);

        QCOMPARE(children[1]->children().count(), 3);
        QCOMPARE(children[1]->title, QString("SubFolder"));
        QCOMPARE(children[1]->url, QString());
        QCOMPARE(children[1]->expanded, true);
        QCOMPARE(children[1]->type(), BookmarkNode::Folder);
        // children not checked

        QCOMPARE(children[2]->children().count(), 0);
        QCOMPARE(children[2]->title, QString());
        QCOMPARE(children[2]->url, QString());
        QCOMPARE(children[2]->expanded, false);
        QCOMPARE(children[2]->type(), BookmarkNode::Separator);
    }
    delete root;
}


void tst_Xbel::xbelwriter_data()
{
}

void tst_Xbel::xbelwriter()
{
    SubXbelWriter writer;
    QCOMPARE(writer.write(QString(), 0), false);
    QCOMPARE(writer.write(QString(), 0), false);

}

void tst_Xbel::write_data()
{
    QTest::addColumn<QString>("readFileName");
    QTest::newRow("frank") << QString("frank.xbel");
    QTest::newRow("all") << QString("all.xbel");
}

// public BookmarkNode *read(QString const &fileName)
void tst_Xbel::write()
{
    QFETCH(QString, readFileName);

    SubXbelReader reader;
    BookmarkNode *root = reader.read(readFileName);
    QVERIFY(root);
    QVERIFY(reader.error() == QXmlStreamReader::NoError);

    SubXbelWriter writer;
    QTemporaryFile file;
    QVERIFY (file.open());
    QVERIFY(writer.write(file.fileName(), root));
    BookmarkNode *writtenRoot = reader.read(file.fileName());
    QVERIFY(*writtenRoot == *root);
    delete root;
    delete writtenRoot;
}

QTEST_MAIN(tst_Xbel)
#include "tst_xbel.moc"


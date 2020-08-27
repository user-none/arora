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

#include "addbookmarkdialog.h"
#include "bookmarksmanager.h"
#include "bookmarknode.h"
#include "browserapplication.h"

#include <qpushbutton.h>
#include <qabstractitemview.h>

class tst_AddBookmarkDialog : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void addbookmarkdialog_data();
    void addbookmarkdialog();
};

// Subclass that exposes the protected functions.
class SubAddBookmarkDialog : public AddBookmarkDialog
{

public:
    SubAddBookmarkDialog(QWidget *parent, BookmarksManager *manager)
        : AddBookmarkDialog(parent, manager) {}

};

// This will be called before the first test function is executed.
// It is only called once.
void tst_AddBookmarkDialog::initTestCase()
{
    QCoreApplication::setApplicationName("addbookmarkdialogtest");
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_AddBookmarkDialog::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_AddBookmarkDialog::init()
{
    BookmarksManager *manager = BrowserApplication::bookmarksManager();
    BookmarkNode *root = manager->bookmarks();
    QList<BookmarkNode*> nodes = root->children();
    BookmarkNode *menu = nodes[0];
    BookmarkNode *toolbar = nodes[1];
    while (!menu->children().isEmpty())
        manager->removeBookmark(menu->children().first());
    while (!toolbar->children().isEmpty())
        manager->removeBookmark(toolbar->children().first());
}

// This will be called after every test function.
void tst_AddBookmarkDialog::cleanup()
{
}

Q_DECLARE_METATYPE(QDialogButtonBox::StandardButton)
void tst_AddBookmarkDialog::addbookmarkdialog_data()
{
    QTest::addColumn<QString>("url");
    QTest::addColumn<QString>("title");
    QTest::addColumn<QDialogButtonBox::StandardButton>("button");
    QTest::addColumn<int>("menuCount");
    QTest::addColumn<int>("toolbarCount");
    QTest::addColumn<int>("select");

    // select invalid, menu, toolbar, submenu
    QTest::newRow("cancel")     << "url" << "title" << QDialogButtonBox::Cancel << 0 << 0 << -1;
    QTest::newRow("ok default") << "url" << "title" << QDialogButtonBox::Ok     << 0 << 1 << -1;
    QTest::newRow("ok toolbar") << "url" << "title" << QDialogButtonBox::Ok     << 0 << 1 << 0;
    QTest::newRow("ok menu")    << "url" << "title" << QDialogButtonBox::Ok     << 1 << 0 << 1;
}

void tst_AddBookmarkDialog::addbookmarkdialog()
{
    QFETCH(QString, url);
    QFETCH(QString, title);
    QFETCH(QDialogButtonBox::StandardButton, button);
    QFETCH(int, menuCount);
    QFETCH(int, toolbarCount);
    QFETCH(int, select);

    BookmarksManager *manager = BrowserApplication::bookmarksManager();
    qRegisterMetaType<BookmarkNode*>("BookmarkNode *");
    QSignalSpy spy(manager, SIGNAL(entryAdded(BookmarkNode *)));
    BookmarkNode *menu = manager->menu();
    BookmarkNode *toolbar = manager->toolbar();
    QCOMPARE(menu->children().count(), 0);
    QCOMPARE(toolbar->children().count(), 0);

    SubAddBookmarkDialog dialog(0, manager);
    dialog.setUrl(url);
    dialog.setTitle(title);
    QComboBox *combobox = dialog.findChild<QComboBox*>();
    QVERIFY(combobox);
    if (select != -1) {
        combobox->setCurrentIndex(select);
        combobox->view()->setCurrentIndex(combobox->model()->index(select, 0));
    }
    QDialogButtonBox *buttonBox = dialog.findChild<QDialogButtonBox*>();
    QVERIFY(buttonBox);
    QPushButton *pushButton = buttonBox->button(button);
    pushButton->click();

    QCOMPARE(spy.count(), menuCount + toolbarCount);

    QCOMPARE(menu->children().count(), menuCount);
    QCOMPARE(toolbar->children().count(), toolbarCount);
    BookmarkNode *node = 0;
    if (menuCount == 1) node = menu->children()[0];
    if (toolbarCount == 1) node = toolbar->children()[0];
    if (node) {
        QCOMPARE(node->title, title);
        QCOMPARE(node->url, url);
        QVERIFY(dialog.addedNode());
        QVERIFY(*node == *dialog.addedNode());
    }
}

QTEST_MAIN(tst_AddBookmarkDialog)
#include "tst_addbookmarkdialog.moc"


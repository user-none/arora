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

#include <QtTest/QtTest>
#include <searchlineedit.h>

class tst_SearchLineEdit : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void searchlineedit_data();
    void searchlineedit();
};

// Subclass that exposes the protected functions.
class SubSearchLineEdit : public SearchLineEdit
{
public:};

// This will be called before the first test function is executed.
// It is only called once.
void tst_SearchLineEdit::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_SearchLineEdit::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_SearchLineEdit::init()
{
}

// This will be called after every test function.
void tst_SearchLineEdit::cleanup()
{
}

void tst_SearchLineEdit::searchlineedit_data()
{
}

void tst_SearchLineEdit::searchlineedit()
{
    SubSearchLineEdit edit;
    QVERIFY(edit.clearButton() != 0);
    QVERIFY(edit.searchButton() != 0);
}

QTEST_MAIN(tst_SearchLineEdit)
#include "tst_searchlineedit.moc"


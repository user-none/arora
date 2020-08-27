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

#include <lineedit.h>
#include <qtoolbutton.h>

class tst_LineEdit : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void lineedit_data();
    void lineedit();

    void addWidget_data();
    void addWidget();
    void removeWidget();
    void setWidgetSpacing_data();
    void setWidgetSpacing();
    void textMargin_data();
    void textMargin();
    void inactiveText_data();
    void inactiveText();
};

// Subclass that exposes the protected functions.
class SubLineEdit : public LineEdit
{
public:
    void call_resizeEvent(QResizeEvent *event)
        { return SubLineEdit::resizeEvent(event); }
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_LineEdit::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_LineEdit::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_LineEdit::init()
{
}

// This will be called after every test function.
void tst_LineEdit::cleanup()
{
}

void tst_LineEdit::lineedit_data()
{
}

void tst_LineEdit::lineedit()
{
    SubLineEdit edit;
    edit.addWidget((QWidget*)0, LineEdit::LeftSide);
    edit.addWidget((QWidget*)0, LineEdit::RightSide);
    edit.removeWidget((QWidget*)0);
    QCOMPARE(edit.widgetSpacing(), 3);
    edit.setWidgetSpacing(-1);
    QCOMPARE(edit.textMargin(LineEdit::LeftSide), 0);
    QCOMPARE(edit.textMargin(LineEdit::RightSide), 0);
}

Q_DECLARE_METATYPE(LineEdit::WidgetPosition)
void tst_LineEdit::addWidget_data()
{
    QTest::addColumn<LineEdit::WidgetPosition>("position");
    QTest::addColumn<int>("spacing");
    for (int i = 0; i < 4; ++i) {
        QTest::newRow("left") << LineEdit::LeftSide << i;
        QTest::newRow("right") << LineEdit::RightSide << i;
    }
}

// public void addWidget(QWidget *widget, LineEdit::WidgetPosition position)
void tst_LineEdit::addWidget()
{
    QFETCH(LineEdit::WidgetPosition, position);
    QFETCH(int, spacing);
    LineEdit::WidgetPosition other = position == LineEdit::LeftSide ? LineEdit::RightSide : LineEdit::LeftSide;

    SubLineEdit edit;
    edit.setWidgetSpacing(spacing);

    QToolButton *button = new QToolButton;
    button->setMinimumSize(10, 10);
    button->setMaximumSize(10, 10);
    edit.addWidget(button, position);
    edit.show();

    spacing = edit.widgetSpacing();
    QCOMPARE(edit.textMargin(position), 10 + spacing * 2);
    QCOMPARE(edit.textMargin(other), 0);

    edit.removeWidget(button);
    qApp->processEvents();
    QCOMPARE(edit.textMargin(position), 0);
}

// public void removeWidget(QWidget *widget)
void tst_LineEdit::removeWidget()
{
    SubLineEdit edit;
    QToolButton *button = new QToolButton;
    edit.addWidget(button, LineEdit::LeftSide);
    edit.show();
    QVERIFY(edit.textMargin(LineEdit::LeftSide) != 0);
    edit.removeWidget(button);
    qApp->processEvents();
    QVERIFY(edit.textMargin(LineEdit::LeftSide) == 0);
}

void tst_LineEdit::setWidgetSpacing_data()
{
    QTest::addColumn<int>("spacing");
    QTest::newRow("0") << 0;
    QTest::newRow("1") << 1;
    QTest::newRow("2") << 2;
}

// public void setWidgetSpacing(int spacing)
void tst_LineEdit::setWidgetSpacing()
{
    QFETCH(int, spacing);

    SubLineEdit edit;

    edit.setWidgetSpacing(spacing);
    QCOMPARE(edit.widgetSpacing(), spacing);
}

void tst_LineEdit::textMargin_data()
{
    QTest::addColumn<LineEdit::WidgetPosition>("position");
    QTest::addColumn<int>("textMargin");
    QTest::newRow("null") << LineEdit::LeftSide << 0;
}

// public int textMargin(LineEdit::WidgetPosition position) const
void tst_LineEdit::textMargin()
{
    QFETCH(LineEdit::WidgetPosition, position);
    QFETCH(int, textMargin);

    SubLineEdit edit;

    QCOMPARE(edit.textMargin(position), textMargin);
}

void tst_LineEdit::inactiveText_data()
{
    QTest::addColumn<QString>("inactiveText");
    QTest::newRow("foo") << QString("foo");
}

// public QString inactiveText() const
void tst_LineEdit::inactiveText()
{
    QFETCH(QString, inactiveText);

    SubLineEdit edit;
    edit.setInactiveText(inactiveText);
    QCOMPARE(edit.inactiveText(), inactiveText);
}

QTEST_MAIN(tst_LineEdit)
#include "tst_lineedit.moc"


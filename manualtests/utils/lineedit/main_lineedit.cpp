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

#include <qtoolbutton.h>
#include <qdebug.h>

#include <lineedit.h>
#include "ui_dialog.h"

class Dialog : public QDialog, public Ui_Dialog
{
    Q_OBJECT
public:
    Dialog(QWidget *parent = 0);

    QLabel *rssButton;
    QLabel *bookmarkButton;
    QLabel *siteIconButton;

private slots:
    void setWidgetSpacing(int s);
    void add();
    void remove();
    void showW();
    void addToolButton();

    QWidget *getWidget();
};

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    rssButton = new QLabel(this);
    rssButton->setPixmap(QPixmap("rss.png"));
    rssButton->hide();
    bookmarkButton = new QLabel(this);
    bookmarkButton->setPixmap(QPixmap("bookmark.png"));
    bookmarkButton->hide();
    siteIconButton = new QLabel(this);
    siteIconButton->setPixmap(QPixmap("siteicon.png"));
    siteIconButton->hide();

    {
        QToolButton *b = new QToolButton(this);
        b->resize(16, 16);
        leftLineEdit->addWidget(b, LineEdit::LeftSide);
    }
    {
        QToolButton *b = new QToolButton(this);
        b->resize(16, 16);
        rightLineEdit->addWidget(b, LineEdit::RightSide);
    }
    connect(addButton, SIGNAL(clicked()), this, SLOT(add()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(remove()));
    connect(showButton, SIGNAL(clicked()), this, SLOT(showW()));
    connect(addToolButtonButton, SIGNAL(clicked()), this, SLOT(addToolButton()));
    connect(spacing, SIGNAL(valueChanged(int)), this, SLOT(setWidgetSpacing(int)));
}

QWidget *Dialog::getWidget()
{
    if (rssWidget->isChecked()) {
        bookmarkWidget->setChecked(true);
        return rssButton;
    }
    if (bookmarkWidget->isChecked()) {
        siteIconWidget->setChecked(true);
        return bookmarkButton;
    }
    if (siteIconWidget->isChecked()) {
        rssWidget->setChecked(true);
        return siteIconButton;
    }
    return 0;
}

void Dialog::setWidgetSpacing(int s)
{
    lineEdit->setWidgetSpacing(s);
}

void Dialog::add()
{
    LineEdit::WidgetPosition position = leftSide->isChecked() ? LineEdit::LeftSide : LineEdit::RightSide;
    lineEdit->addWidget(getWidget(), position);
}

void Dialog::remove()
{
    lineEdit->removeWidget(getWidget());
}

void Dialog::showW()
{
    QWidget *w = getWidget();
    w->setVisible(!w->isVisible());
}

void Dialog::addToolButton()
{
    LineEdit::WidgetPosition position = leftSide->isChecked() ? LineEdit::LeftSide : LineEdit::RightSide;
    QToolButton *button = new QToolButton;
    lineEdit->addWidget(button, position);
}

int main(int argc,char ** argv)
{
    QApplication app(argc,argv);

    if (app.arguments().count() == 1) {
        Dialog dialog;
        dialog.show();
        return app.exec();
    } else {
        LineEdit lineEdit;
        QPushButton *rightButton = new QPushButton("Dialog bar");
        qDebug() << rightButton->sizeHint();
        lineEdit.addWidget(rightButton, LineEdit::RightSide);
        lineEdit.show();
        return app.exec();
    }
}

#include "main_lineedit.moc"


/*
 * Copyright 2008-2009 Benjamin C. Meyer <ben@meyerhome.net>
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

#include <qapplication.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcompleter.h>
#include <qstringlistmodel.h>
#include <qdialog.h>

#include "searchlineedit.h"
#include "searchbutton.h"

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    QDialog window;
    QPushButton *button1 = new QPushButton("One");
    SearchLineEdit *s1 = new SearchLineEdit(&window);
    SearchLineEdit *s2 = new SearchLineEdit(&window);
    QCompleter *completer = new QCompleter(&window);
    s2->setCompleter(completer);
    s2->searchButton()->setShowMenuTriangle(true);
    QObject::connect(s2->searchButton(), SIGNAL(clicked()),
            completer, SLOT(complete()));
    QStringList list;
    list << "a" << "b" << "c";
    s2->completer()->setModel(new QStringListModel(list));

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(s1);
    layout->addWidget(s2);
    layout->addWidget(button1);

    window.setLayout(layout);
    window.show();
    return application.exec();
}


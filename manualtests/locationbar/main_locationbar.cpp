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

#include <qmainwindow.h>
#include <qsplitter.h>
#include <qcombobox.h>
#include <qlayout.h>
#include <qtoolbar.h>

#include "browserapplication.h"
#include "locationbar.h"
#include "webview.h"

int main(int argc, char **argv)
{
    BrowserApplication application(argc, argv);
    QCoreApplication::setApplicationName(QLatin1String("urllineeditexample"));
    QMainWindow w;

    QWidget *window = new QWidget;
    QComboBox *comboBox = new QComboBox(window);
    comboBox->setEditable(true);
    QLineEdit *lineEdit = new QLineEdit(window);
    LocationBar *s1 = new LocationBar(window);
    LocationBar *s2 = new LocationBar(window);
    WebView *view = new WebView(window);
    view->setUrl(QUrl("http://www.google.com"));
    s2->setWebView(view);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(comboBox);
    layout->addWidget(lineEdit);
    layout->addWidget(s1);
    layout->addWidget(s2);
    layout->addWidget(view);
    window->setLayout(layout);
    w.show();
    w.setCentralWidget(window);

    QToolBar *bar = w.addToolBar("foo");
    QSplitter *splitter = new QSplitter(window);
    splitter->addWidget(new LocationBar);
    splitter->addWidget(new QLineEdit);
    bar->addWidget(splitter);
    return application.exec();
}


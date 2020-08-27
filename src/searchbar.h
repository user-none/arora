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

#ifndef SEARCHBAR_H
#define SEARCHBAR_H

#include <qwidget.h>

#include "ui_searchbanner.h"

QT_BEGIN_NAMESPACE
class QTimeLine;
QT_END_NAMESPACE

class SearchBar : public QWidget
{
    Q_OBJECT

public:
    SearchBar(QWidget *parent = 0);
    void setSearchObject(QObject *object);
    QObject *searchObject() const;

public slots:
    void animateHide();
    void clear();
    void showFind();
    virtual void findNext() = 0;
    virtual void findPrevious() = 0;

protected:
    void resizeEvent(QResizeEvent *event);
    Ui_SearchBanner ui;

private slots:
    void frameChanged(int frame);

private:
    void initializeSearchWidget();
    QObject *m_object;
    QWidget *m_widget;
    QTimeLine *m_timeLine;
};

#endif


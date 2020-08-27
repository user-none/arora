/*
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
 * Copyright 2009 Benjamin K. Stuhl <bks24@cornell.edu>
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

#ifndef HISTORYCOMPLETER_H
#define HISTORYCOMPLETER_H

#include "history.h"

#include <qcompleter.h>
#include <qregexp.h>
#include <qsortfilterproxymodel.h>
#include <qtableview.h>
#include <qtimer.h>

class QResizeEvent;
class HistoryCompletionView : public QTableView
{
public:
    HistoryCompletionView(QWidget *parent = 0);
    int sizeHintForRow(int row) const;

protected:
    void resizeEvent(QResizeEvent *event);
};

// These two classes constitute a dirty hack around QCompleter's inflexibility:
// QCompleter does not allow changing the matching algorithm; it is fixed
// at a simple QString::startsWith() comparison against the model's completionRole()
// data. (QCompleter also does not allow post-facto sorting of the completion results,
// the source model must already be completed.) To work around these limitations,
// we create a custom subclass of QCompleter which abuses the QCompleter::pathFromIndex()
// virtual override to tell the HistoryCompletionModel which string to look for,
// _before_ the QCompleter tries to do matching. Since the HistoryCompletionModel does
// its own filtering, we just lie to the QCompleter and tell it everything matches. We then
// abuse QCompleter::pathFromIndex() to return a url that does not start with what
// the user typed -- but is what they were looking for.

class HistoryCompletionModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QString searchString READ searchString WRITE setSearchString)

public:
    HistoryCompletionModel(QObject *parent = 0);

    enum Roles { HistoryCompletionRole = HistoryFilterModel::MaxRole + 1 };

    QString searchString() const;
    void setSearchString(const QString &str);

    bool isValid() const;
    void setValid(bool b);

    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString m_searchString;
    QRegExp m_searchMatcher;
    QRegExp m_wordMatcher;
    bool m_isValid;
};

class HistoryCompleter : public QCompleter
{
    Q_OBJECT

public:
    HistoryCompleter(QObject *parent = 0);
    HistoryCompleter(QAbstractItemModel *model, QObject *parent = 0);

    virtual QString pathFromIndex(const QModelIndex &index) const;
    virtual QStringList splitPath(const QString &path) const;

protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void updateFilter();

private:
    void init();
    mutable QString m_searchString;
    mutable QTimer m_filterTimer;
};

#endif

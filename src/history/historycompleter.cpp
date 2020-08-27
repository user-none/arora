/*
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

#include "historycompleter.h"

#include <qevent.h>
#include <qfontmetrics.h>
#include <qheaderview.h>

HistoryCompletionView::HistoryCompletionView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->hide();
    verticalHeader()->hide();

    setShowGrid(false);

    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setTextElideMode(Qt::ElideRight);

    QFontMetrics metrics = fontMetrics();
    verticalHeader()->setDefaultSectionSize(metrics.height());

    // As URLs are always LRT, this should be LRT as well
    setLayoutDirection(Qt::LeftToRight);
}

void HistoryCompletionView::resizeEvent(QResizeEvent *event)
{
    horizontalHeader()->resizeSection(0, 0.65 * width());
    horizontalHeader()->setStretchLastSection(true);

    QTableView::resizeEvent(event);
}

int HistoryCompletionView::sizeHintForRow(int row) const
{
    Q_UNUSED(row)
    QFontMetrics metrics = fontMetrics();
    return metrics.height();
}

HistoryCompletionModel::HistoryCompletionModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_searchMatcher(QString(), Qt::CaseInsensitive, QRegExp::FixedString)
    , m_wordMatcher(QString(), Qt::CaseInsensitive)
    , m_isValid(false)
{
    setDynamicSortFilter(true);
}

QVariant HistoryCompletionModel::data(const QModelIndex &index, int role) const
{
    // if we are valid, tell QCompleter that everything we have filtered matches
    // what the user typed; if not, nothing matches
    if (role == HistoryCompletionRole && index.isValid()) {
        if (isValid())
            return QLatin1String("a");
        else
            return QLatin1String("b");
    }

    if (role == Qt::FontRole && index.column() == 1) {
        QFont font = qvariant_cast<QFont>(QSortFilterProxyModel::data(index, role));
        font.setWeight(QFont::Light);
        return font;
    }

    if (role == Qt::DisplayRole)
        role = (index.column() == 0) ? HistoryModel::UrlStringRole : HistoryModel::TitleRole;

    return QSortFilterProxyModel::data(index, role);
}

QString HistoryCompletionModel::searchString() const
{
    return m_searchString;
}

void HistoryCompletionModel::setSearchString(const QString &str)
{
    if (str == m_searchString)
        return;

    m_searchString = str;
    m_searchMatcher.setPattern(str);
    m_wordMatcher.setPattern(QLatin1String("\\b") + QRegExp::escape(str));
    invalidateFilter();
}

bool HistoryCompletionModel::isValid() const
{
    return m_isValid;
}

void HistoryCompletionModel::setValid(bool b)
{
    if (b == m_isValid)
        return;

    m_isValid = b;

    // tell the HistoryCompleter that we've changed
    emit dataChanged(index(0, 0), index(0, rowCount() - 1));
}

bool HistoryCompletionModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    // do a case-insensitive substring match against both the url and title;
    // we have also made sure that the user doesn't accidentally use regexp
    // metacharacters
    QModelIndex idx = sourceModel()->index(source_row, 0, source_parent);
    QString url = sourceModel()->data(idx, HistoryModel::UrlStringRole).toString();

    if (m_searchMatcher.indexIn(url) != -1)
        return true;

    QString title = sourceModel()->data(idx, HistoryModel::TitleRole).toString();

    if (m_searchMatcher.indexIn(title) != -1)
        return true;

    return false;
}

bool HistoryCompletionModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // We give a bonus to hits that match on a word boundary so that e.g. "dot.kde.org"
    // is a better result for typing "dot" than "slashdot.org". However, we only look
    // for the string in the host name, not the entire url, since while it makes sense
    // to e.g. give "www.phoronix.com" a bonus for "ph", it does _not_ make sense to
    // give "www.yadda.com/foo.php" the bonus.
    int frecency_l = sourceModel()->data(left, HistoryFilterModel::FrecencyRole).toInt();
    QString url_l = sourceModel()->data(left, HistoryModel::UrlRole).toUrl().host();
    QString title_l = sourceModel()->data(left, HistoryModel::TitleRole).toString();

    if (m_wordMatcher.indexIn(url_l) != -1 || m_wordMatcher.indexIn(title_l) != -1)
        frecency_l *= 2;

    int frecency_r = sourceModel()->data(right, HistoryFilterModel::FrecencyRole).toInt();
    QString url_r = sourceModel()->data(right, HistoryModel::UrlRole).toUrl().host();
    QString title_r = sourceModel()->data(right, HistoryModel::TitleRole).toString();
    if (m_wordMatcher.indexIn(url_r) != -1 || m_wordMatcher.indexIn(title_r) != -1)
        frecency_r *= 2;

    // sort results in descending frecency-derived score
    return (frecency_r < frecency_l);
}

HistoryCompleter::HistoryCompleter(QObject *parent)
    : QCompleter(parent)
{
    init();
}

HistoryCompleter::HistoryCompleter(QAbstractItemModel *m, QObject *parent)
    : QCompleter(m, parent)
{
    init();
}

void HistoryCompleter::init()
{
    setPopup(new HistoryCompletionView());

    // we want to complete against our own faked role
    setCompletionRole(HistoryCompletionModel::HistoryCompletionRole);

    // and since we fake our completion role, we can take
    // advantage of the sorted-model optimizations in QCompleter
    setCaseSensitivity(Qt::CaseSensitive);
    setModelSorting(QCompleter::CaseSensitivelySortedModel);

    m_filterTimer.setSingleShot(true);
    connect(&m_filterTimer, SIGNAL(timeout()), this, SLOT(updateFilter()));
}

QString HistoryCompleter::pathFromIndex(const QModelIndex &index) const
{
    // we want to return the actual url from the history for the
    // data the QCompleter finally returns
    return model()->data(index, HistoryModel::UrlStringRole).toString();
}

QStringList HistoryCompleter::splitPath(const QString &path) const
{
    if (path == m_searchString)
        return QStringList() << QLatin1String("a");

    // queue an update to our search string
    // We will wait a bit so that if the user is quickly typing,
    // we don't try to complete until they pause.
    if (m_filterTimer.isActive())
        m_filterTimer.stop();
    m_filterTimer.start(150);

    // if the previous search results are not a superset of
    // the current search results, tell the model that it is not valid yet
    if (!path.startsWith(m_searchString)) {
        HistoryCompletionModel *completionModel = qobject_cast<HistoryCompletionModel*>(model());
        Q_ASSERT(completionModel);
        completionModel->setValid(false);
    }

    m_searchString = path;

    // the actual filtering is done by the HistoryCompletionModel; we just
    // return a short dummy here so that QCompleter thinks we match everything
    return QStringList() << QLatin1String("a");
}

bool HistoryCompleter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && popup()->isVisible()) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        if (keyEvent->key() == Qt::Key_Tab) {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress,
                                                Qt::Key_Down,
                                                keyEvent->modifiers(),
                                                QString());

            if (!QCompleter::eventFilter(obj, newEvent))
                obj->event(newEvent);
            return true;
        } else if (keyEvent->key() == Qt::Key_Backtab) {
            QKeyEvent *newEvent = new QKeyEvent(QEvent::KeyPress,
                                                Qt::Key_Up,
                                                keyEvent->modifiers(),
                                                keyEvent->text(),
                                                keyEvent->isAutoRepeat(),
                                                keyEvent->count());

            if (!QCompleter::eventFilter(obj, newEvent))
                obj->event(newEvent);
            return true;
        } else if (keyEvent->key() == Qt::Key_Escape) {
            popup()->hide();
        }
    }
    return QCompleter::eventFilter(obj, event);
}

void HistoryCompleter::updateFilter()
{
    HistoryCompletionModel *completionModel = qobject_cast<HistoryCompletionModel*>(model());
    Q_ASSERT(completionModel);

    // tell the HistoryCompletionModel about the new search string
    completionModel->setSearchString(m_searchString);

    // sort the model
    completionModel->sort(0);

    // mark it valid
    completionModel->setValid(true);

    // and now update the QCompleter widget, but only if the user is still
    // typing a url
    if (widget() && widget()->hasFocus())
        complete();
}

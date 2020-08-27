/*
 * Copyright 2008 ChriChristian Franke <cfchris6@ts2server.com>>
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

#ifndef PLAINTEXTEDITSEARCH_H
#define PLAINTEXTEDITSEARCH_H

#include "searchbar.h"

#include <qtextdocument.h>

class QPlainTextEdit;
class PlainTextEditSearch : public SearchBar
{
    Q_OBJECT

public:
    PlainTextEditSearch(QPlainTextEdit *plainTextEdit, QWidget *parent = 0);

public slots:
    void findNext();
    void findPrevious();

private:
    void find(QTextDocument::FindFlags flags);
    QPlainTextEdit *m_edit;
    QString m_lastSearchTerm;
};

#endif // PLAINTEXTEDITSEARCH_H


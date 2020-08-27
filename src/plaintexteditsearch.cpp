/*
 * Copyright 2008 Christian Franke <cfchris6@ts2server.com>
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

#include "plaintexteditsearch.h"

#include <qplaintextedit.h>
#include <qtextcursor.h>

PlainTextEditSearch::PlainTextEditSearch(QPlainTextEdit *plainTextEdit, QWidget *parent)
    : SearchBar(parent)
    , m_edit(plainTextEdit)
{
    setSearchObject(plainTextEdit);
}

void PlainTextEditSearch::findNext()
{
    find(0);
}

void PlainTextEditSearch::findPrevious()
{
    find(QTextDocument::FindBackward);
}

void PlainTextEditSearch::find(QTextDocument::FindFlags flags)
{
    QString searchString = ui.searchLineEdit->text();
    if (!m_edit || searchString.isEmpty())
        return;
    if (searchString != m_lastSearchTerm) {
        QTextCursor cursor = m_edit->textCursor();
        cursor.setPosition(cursor.selectionStart());
        cursor.clearSelection();
        m_edit->setTextCursor(cursor);
        m_lastSearchTerm = searchString;
    }
    QString infoString;
    if (!m_edit->find(searchString, flags)) {
        /* no support for wrapping so we set the cursor to start */
        QTextCursor cursor = m_edit->textCursor();
        m_edit->moveCursor(QTextCursor::Start);
        /* ...search again from the beginning */
        if (!m_edit->find(searchString,flags)) {
            /* and if there's still nothing, we revert the cursor */
            infoString = tr("Not Found");
            cursor.clearSelection();
            m_edit->setTextCursor(cursor);
        }
    }
    ui.searchInfo->setText(infoString);
}


/*
 * Copyright 2009 Christian Franke <cfchris6@ts2server.com>
 * Copyright 2009 Jakub Wieczorek <faw217@gmail.com>
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

#include "opensearchdialog.h"

#include "browserapplication.h"
#include "opensearchenginemodel.h"
#include "opensearchmanager.h"
#include "toolbarsearch.h"

#include <qfiledialog.h>
#include <qmessagebox.h>

OpenSearchDialog::OpenSearchDialog(QWidget *parent)
    : QDialog(parent)
    , m_model(0)
{
    setModal(true);
    setupUi(this);

    m_model = new OpenSearchEngineModel(ToolbarSearch::openSearchManager(), this);
    m_tableView->setModel(m_model);
    m_tableView->horizontalHeader()->resizeSection(0, 200);
    m_tableView->horizontalHeader()->setStretchLastSection(true);
    m_tableView->verticalHeader()->hide();
    m_tableView->verticalHeader()->setDefaultSectionSize(1.2 * fontMetrics().height());
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setShowGrid(false);
    m_tableView->setAlternatingRowColors(true);

    connect(m_closeButton, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(m_addButton, SIGNAL(clicked()),
            this, SLOT(addButtonClicked()));
    connect(m_deleteButton, SIGNAL(clicked()),
            this, SLOT(deleteButtonClicked()));
    connect(m_restoreButton, SIGNAL(clicked()),
            this, SLOT(restoreButtonClicked()));
}

void OpenSearchDialog::addButtonClicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,
                                                          tr("Open File"),
                                                          QString(),
                                                          tr("OpenSearch") + QLatin1String(" (*.xml)"));

    foreach (const QString &fileName, fileNames) {
        if (!ToolbarSearch::openSearchManager()->addEngine(fileName)) {
            QMessageBox::critical(this, tr("Error"),
                    tr("%1 is not a valid OpenSearch 1.1 description or is already on your list.").arg(fileName));
        }
    }
}

void OpenSearchDialog::deleteButtonClicked()
{
    if (m_tableView->model()->rowCount() == 1) {
        QMessageBox::critical(this, tr("Error"),
                tr("You must have at least one search engine in here."));
        return;
    }

    m_tableView->removeSelected();
}

void OpenSearchDialog::restoreButtonClicked()
{
    ToolbarSearch::openSearchManager()->restoreDefaults();
}


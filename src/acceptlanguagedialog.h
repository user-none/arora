/*
 * Copyright 2009 Benjamin C. Meyer <ben@meyerhome.net>
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

#ifndef ACCEPTLANGUAGEDIALOG_H
#define ACCEPTLANGUAGEDIALOG_H

#include <qdialog.h>
#include "ui_acceptlanguagedialog.h"

#include <qstringlistmodel.h>

class AcceptLanguageDialog : public QDialog, public Ui_AcceptLanguage
{
    Q_OBJECT

public:
    AcceptLanguageDialog(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    void accept();
    static QByteArray httpString(const QStringList &list);
    static QStringList defaultAcceptList();

private slots:
    void load();
    void save();

    void currentChanged(const QModelIndex &current, const QModelIndex &previous);
    void moveLanguageUp();
    void moveLanguageDown();
    void removeLanguage();
    void addLanguage();

private:
    static QStringList expand(const QLocale::Language language);
    QStringListModel m_allLanguagesModel;
    QStringListModel m_model;

};

#endif // ACCEPTLANGUAGEDIALOG_H


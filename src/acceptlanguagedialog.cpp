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

#include "acceptlanguagedialog.h"

#include "browserapplication.h"
#include "languagemanager.h"

#include <qlistview.h>
#include <qsettings.h>

AcceptLanguageDialog::AcceptLanguageDialog(QWidget *parent, Qt::WindowFlags flags)
    : QDialog(parent, flags)
{
    setupUi(this);
    connect(addButton, SIGNAL(clicked()), this, SLOT(addLanguage()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(removeLanguage()));
    connect(moveUpButton, SIGNAL(clicked()), this, SLOT(moveLanguageUp()));
    connect(moveDownButton, SIGNAL(clicked()), this, SLOT(moveLanguageDown()));
    listView->setModel(&m_model);
    connect(listView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
            this, SLOT(currentChanged(const QModelIndex &, const QModelIndex &)));
    load();

    QStringList allLanguages;
    for (int i = 1 + (int)QLocale::C; i <= (int)QLocale::LastLanguage; ++i)
        allLanguages += expand(QLocale::Language(i));
    m_allLanguagesModel.setStringList(allLanguages);
    addComboBox->setModel(&m_allLanguagesModel);
}

QStringList AcceptLanguageDialog::expand(const QLocale::Language language)
{
    QStringList allLanguages;
    QList<QLocale::Country> countries = QLocale::countriesForLanguage(language);
    for (int j = 0; j < countries.size(); ++j) {
        QString languageString;
        if (countries.count() == 1) {
            languageString = QString(QLatin1String("%1 [%2]"))
                .arg(QLocale::languageToString(language))
                .arg(QLocale(language).name().split(QLatin1Char('_')).at(0));
        } else {
            languageString = QString(QLatin1String("%1/%2 [%3]"))
                .arg(QLocale::languageToString(language))
                .arg(QLocale::countryToString(countries.at(j)))
                .arg(QLocale(language, countries.at(j)).name().split(QLatin1Char('_')).join(QLatin1String("-")).toLower());

        }
        if (!allLanguages.contains(languageString))
            allLanguages.append(languageString);
    }
    return allLanguages;
}

void AcceptLanguageDialog::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    removeButton->setEnabled(current.isValid());
    int row = current.row();
    moveUpButton->setEnabled(row > 0);
    moveDownButton->setEnabled(row != -1 && row < m_model.rowCount() - 1);
}

QStringList AcceptLanguageDialog::defaultAcceptList()
{
    QString currentLanguage = BrowserApplication::instance()->languageManager()->currentLanguage();
    if (currentLanguage.isEmpty())
        return QStringList();
    return expand(QLocale(currentLanguage).language());
}

void AcceptLanguageDialog::accept()
{
    save();
    QDialog::accept();
}

void AcceptLanguageDialog::load()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("network"));
    m_model.setStringList(settings.value(QLatin1String("acceptLanguages"), defaultAcceptList()).toStringList());
}

void AcceptLanguageDialog::save()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("network"));
    QStringList result = m_model.stringList();
    if (result == defaultAcceptList() || result.isEmpty())
        settings.remove(QLatin1String("acceptLanguages"));
    else
        settings.setValue(QLatin1String("acceptLanguages"), result);
}

/*
    Return a ByteArray that can be sent along with the Accept-Language http header

    See RFC 2616 section 14.4
 */
QByteArray AcceptLanguageDialog::httpString(const QStringList &list)
{
    QStringList processed;
    qreal qvalue = 1.0;
    foreach (const QString &string, list) {
        int leftBracket = string.indexOf(QLatin1Char('['));
        int rightBracket = string.indexOf(QLatin1Char(']'));
        QString tag = string.mid(leftBracket + 1, rightBracket - leftBracket - 1);
        if (processed.isEmpty()) {
            processed << tag;
        } else {
            processed << QString(QLatin1String("%1; %2")).arg(tag).arg(QString::number(qvalue, 'f', 1));
        }
        if (qvalue > .1)
            qvalue -= .1;
    }
    return processed.join(QLatin1String(", ")).toLatin1();
}

void AcceptLanguageDialog::moveLanguageUp()
{
    int currentRow = listView->currentIndex().row();
    QString item = listView->currentIndex().data().toString();
    m_model.removeRow(currentRow);
    m_model.insertRows(currentRow - 1, 1);
    m_model.setData(m_model.index(currentRow - 1), item);
    listView->setCurrentIndex(m_model.index(currentRow + 1));
}

void AcceptLanguageDialog::moveLanguageDown()
{
    int currentRow = listView->currentIndex().row();
    QString item = listView->currentIndex().data().toString();
    m_model.removeRow(currentRow);
    m_model.insertRows(currentRow + 1, 1);
    m_model.setData(m_model.index(currentRow + 1), item);
    listView->setCurrentIndex(m_model.index(currentRow + 1));
}

void AcceptLanguageDialog::removeLanguage()
{
    int currentRow = listView->currentIndex().row();
    m_model.removeRow(currentRow);
}

void AcceptLanguageDialog::addLanguage()
{
    QString text = addComboBox->currentText();
    if (m_model.stringList().contains(text))
        return;
    m_model.insertRow(m_model.rowCount());
    m_model.setData(m_model.index(m_model.rowCount() - 1), text);
}


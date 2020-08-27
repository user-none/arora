/*
 * Copyright 2008-2009 Jason A. Donenfeld <Jason@zx2c4.com>
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

#include "clearprivatedata.h"

#include "browserapplication.h"
#include "browsermainwindow.h"
#include "cookiejar.h"
#include "downloadmanager.h"
#include "historymanager.h"
#include "networkaccessmanager.h"
#include "toolbarsearch.h"

#include <qabstractnetworkcache.h>
#include <qcheckbox.h>
#include <qdialogbuttonbox.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlist.h>
#include <qpushbutton.h>
#include <qsettings.h>
#include <qwebsettings.h>

ClearPrivateData::ClearPrivateData(QWidget *parent)
    : QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint)
{
    setWindowTitle(tr("Clear Private Data"));

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(new QLabel(tr("Clear the following items:")));

    QSettings settings;
    settings.beginGroup(QLatin1String("clearprivatedata"));

    m_browsingHistory = new QCheckBox(tr("&Browsing History"));
    m_browsingHistory->setChecked(settings.value(QLatin1String("browsingHistory"), true).toBool());
    layout->addWidget(m_browsingHistory);

    m_downloadHistory = new QCheckBox(tr("&Download History"));
    m_downloadHistory->setChecked(settings.value(QLatin1String("downloadHistory"), true).toBool());
    layout->addWidget(m_downloadHistory);

    m_searchHistory = new QCheckBox(tr("&Search History"));
    m_searchHistory->setChecked(settings.value(QLatin1String("searchHistory"), true).toBool());
    layout->addWidget(m_searchHistory);

    m_cookies = new QCheckBox(tr("&Cookies"));
    m_cookies->setChecked(settings.value(QLatin1String("cookies"), true).toBool());
    layout->addWidget(m_cookies);

    m_cache = new QCheckBox(tr("C&ached Web Pages"));
    if (BrowserApplication::networkAccessManager()->cache()) {
        m_cache->setChecked(settings.value(QLatin1String("cache"), true).toBool());
    } else {
        m_cache->setEnabled(false);
    }
    layout->addWidget(m_cache);

    m_favIcons = new QCheckBox(tr("Website &Icons"));
    m_favIcons->setChecked(settings.value(QLatin1String("favIcons"), true).toBool());
    layout->addWidget(m_favIcons);

    settings.endGroup();

    QPushButton *acceptButton = new QPushButton(tr("Clear &Private Data"));
    acceptButton->setDefault(true);
    QPushButton *rejectButton = new QPushButton(tr("&Cancel"));
    QDialogButtonBox *buttonBox = new QDialogButtonBox;
    buttonBox->addButton(acceptButton, QDialogButtonBox::AcceptRole);
    buttonBox->addButton(rejectButton, QDialogButtonBox::RejectRole);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    layout->addWidget(buttonBox);

    setLayout(layout);
    acceptButton->setFocus(Qt::OtherFocusReason);
}

void ClearPrivateData::accept()
{
    QSettings settings;
    settings.beginGroup(QLatin1String("clearprivatedata"));

    settings.setValue(QLatin1String("browsingHistory"), m_browsingHistory->isChecked());
    settings.setValue(QLatin1String("downloadHistory"), m_downloadHistory->isChecked());
    settings.setValue(QLatin1String("searchHistory"), m_searchHistory->isChecked());
    settings.setValue(QLatin1String("cookies"), m_cookies->isChecked());
    settings.setValue(QLatin1String("cache"), m_cache->isChecked());
    settings.setValue(QLatin1String("favIcons"), m_favIcons->isChecked());

    settings.endGroup();

    if (m_browsingHistory->isChecked()) {
        BrowserApplication::historyManager()->clear();
    }

    if (m_downloadHistory->isChecked()) {
        BrowserApplication::downloadManager()->cleanup();
        BrowserApplication::downloadManager()->hide();
    }

    if (m_searchHistory->isChecked()) {
        QList<BrowserMainWindow*> mainWindows = BrowserApplication::instance()->mainWindows();
        for (int i = 0; i < mainWindows.count(); ++i) {
            mainWindows.at(i)->toolbarSearch()->setText(QString());
        }
    }

    if (m_cookies->isChecked()) {
        BrowserApplication::cookieJar()->clear();
    }

    if (m_cache->isChecked() && BrowserApplication::networkAccessManager()->cache()) {
        BrowserApplication::networkAccessManager()->cache()->clear();
    }

    if (m_favIcons->isChecked()) {
        QWebSettings::clearIconDatabase();
    }
    QDialog::accept();
}

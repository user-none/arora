/*
 * Copyright 2009-2010 Benjamin C. Meyer <ben@meyerhome.net>
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

#include "privacyindicator.h"

#include "browserapplication.h"

PrivacyIndicator::PrivacyIndicator(QWidget *parent)
    : QLabel(parent)
{
    setPixmap(QPixmap(QLatin1String(":graphics/private.png")));
    connect(BrowserApplication::instance(), SIGNAL(privacyChanged(bool)),
            this, SLOT(setVisible(bool)));
    setCursor(Qt::ArrowCursor);
    setVisible(BrowserApplication::isPrivate());
}

void PrivacyIndicator::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    BrowserApplication::instance()->setPrivate(false);
}

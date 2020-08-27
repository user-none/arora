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

#include "opensearchengineaction.h"

#include "browserapplication.h"
#include "networkaccessmanager.h"
#include "opensearchengine.h"

OpenSearchEngineAction::OpenSearchEngineAction(OpenSearchEngine *engine, QObject *parent)
    : QAction(parent)
    , m_engine(engine)
{
    if (!engine->networkAccessManager())
        engine->setNetworkAccessManager(BrowserApplication::networkAccessManager());

    setText(engine->name());
    imageChanged();
    connect(engine, SIGNAL(imageChanged()), this, SLOT(imageChanged()));
}

void OpenSearchEngineAction::imageChanged()
{
    QImage image = m_engine->image();
    if (image.isNull())
        setIcon(BrowserApplication::icon(m_engine->imageUrl()));
    else
        setIcon(QIcon(QPixmap::fromImage(image)));
}


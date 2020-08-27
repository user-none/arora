/*
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

#include "opensearchwriter.h"

#include "opensearchengine.h"

#include <qdebug.h>
#include <qiodevice.h>

/*!
    \class OpenSearchWriter
    \brief A class writing a search engine description to an external source

    OpenSearchReader is a class that can be used to write search engine descriptions
    using the OpenSearch format.

    For more information see:
    http://www.opensearch.org/Specifications/OpenSearch/1.1/Draft_4#OpenSearch_description_document

    \sa OpenSearchEngine, OpenSearchReader
*/

/*!
    Constructs a new writer.

    \note One instance can be used to write multiple search engine descriptions, one by one.
*/
OpenSearchWriter::OpenSearchWriter()
    : QXmlStreamWriter()
{
    setAutoFormatting(true);
}

/*!
    Writes an OpenSearch engine description to the \a device, filling the output document
    with all the necessary data.

    If the \a device is closed, it will be opened.

    \return true on success and false on failure.
*/
bool OpenSearchWriter::write(QIODevice *device, OpenSearchEngine *engine)
{
    if (!engine)
        return false;

    if (!device->isOpen()) {
        if (!device->open(QIODevice::WriteOnly))
            return false;
    }

    setDevice(device);
    write(engine);
    return true;
}

void OpenSearchWriter::write(OpenSearchEngine *engine)
{
    writeStartDocument();
    writeStartElement(QLatin1String("OpenSearchDescription"));
    writeDefaultNamespace(QLatin1String("http://a9.com/-/spec/opensearch/1.1/"));

    if (!engine->name().isEmpty())
        writeTextElement(QLatin1String("ShortName"), engine->name());

    if (!engine->description().isEmpty())
        writeTextElement(QLatin1String("Description"), engine->description());

    if (!engine->searchUrlTemplate().isEmpty()) {
        writeStartElement(QLatin1String("Url"));
        writeAttribute(QLatin1String("method"), engine->searchMethod());
        writeAttribute(QLatin1String("type"), QLatin1String("text/html"));
        writeAttribute(QLatin1String("template"), engine->searchUrlTemplate());

        if (!engine->searchParameters().empty()) {
            writeNamespace(QLatin1String("http://a9.com/-/spec/opensearch/extensions/parameters/1.0/"), QLatin1String("p"));

            QList<OpenSearchEngine::Parameter>::const_iterator end = engine->searchParameters().constEnd();
            QList<OpenSearchEngine::Parameter>::const_iterator i = engine->searchParameters().constBegin();
            for (; i != end; ++i) {
                writeStartElement(QLatin1String("p:Parameter"));
                writeAttribute(QLatin1String("name"), i->first);
                writeAttribute(QLatin1String("value"), i->second);
                writeEndElement();
            }
        }

        writeEndElement();
    }

    if (!engine->suggestionsUrlTemplate().isEmpty()) {
        writeStartElement(QLatin1String("Url"));
        writeAttribute(QLatin1String("method"), engine->suggestionsMethod());
        writeAttribute(QLatin1String("type"), QLatin1String("application/x-suggestions+json"));
        writeAttribute(QLatin1String("template"), engine->suggestionsUrlTemplate());

        if (!engine->suggestionsParameters().empty()) {
            writeNamespace(QLatin1String("http://a9.com/-/spec/opensearch/extensions/parameters/1.0/"), QLatin1String("p"));

            QList<OpenSearchEngine::Parameter>::const_iterator end = engine->suggestionsParameters().constEnd();
            QList<OpenSearchEngine::Parameter>::const_iterator i = engine->suggestionsParameters().constBegin();
            for (; i != end; ++i) {
                writeStartElement(QLatin1String("p:Parameter"));
                writeAttribute(QLatin1String("name"), i->first);
                writeAttribute(QLatin1String("value"), i->second);
                writeEndElement();
            }
        }

        writeEndElement();
    }

    if (!engine->imageUrl().isEmpty())
        writeTextElement(QLatin1String("Image"), engine->imageUrl());

    writeEndElement();
    writeEndDocument();
}


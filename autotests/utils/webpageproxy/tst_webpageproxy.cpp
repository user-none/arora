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

#include <qtest.h>

#include <webpageproxy.h>
#include <qnetworkrequest.h>

class tst_WebPageProxy : public QObject
{
    Q_OBJECT

public slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

private slots:
    void webpageproxy_data();
    void webpageproxy();

    void populateNetworkRequest();
};

// Subclass that exposes the protected functions.
class SubWebPageProxy : public WebPageProxy
{
public:
    void call_populateNetworkRequest(QNetworkRequest &request)
        { return SubWebPageProxy::populateNetworkRequest(request); }
};

// This will be called before the first test function is executed.
// It is only called once.
void tst_WebPageProxy::initTestCase()
{
}

// This will be called after the last test function is executed.
// It is only called once.
void tst_WebPageProxy::cleanupTestCase()
{
}

// This will be called before each test function is executed.
void tst_WebPageProxy::init()
{
}

// This will be called after every test function.
void tst_WebPageProxy::cleanup()
{
}

void tst_WebPageProxy::webpageproxy_data()
{
}

void tst_WebPageProxy::webpageproxy()
{
    SubWebPageProxy proxy;
    QCOMPARE(proxy.pageAttributeId(), 1100);
    QNetworkRequest request;
    proxy.call_populateNetworkRequest(request);
}

// protected void populateNetworkRequest(QNetworkRequest &request)
void tst_WebPageProxy::populateNetworkRequest()
{
    SubWebPageProxy proxy;

    QNetworkRequest emptyRequest;
    QNetworkRequest request = emptyRequest;
    proxy.call_populateNetworkRequest(request);

    QVERIFY(request != emptyRequest);
    QVariant v = request.attribute((QNetworkRequest::Attribute)(proxy.pageAttributeId()));
    QVERIFY(v.isValid());
    QWebPage *webPage = (QWebPage*)(v.value<void*>());
    QVERIFY(webPage);
    QCOMPARE(webPage, &proxy);
}

QTEST_MAIN(tst_WebPageProxy)
#include "tst_webpageproxy.moc"


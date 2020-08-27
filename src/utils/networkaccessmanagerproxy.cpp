/**
 * Copyright (c) 2009, Benjamin C. Meyer  <ben@meyerhome.net>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Benjamin Meyer nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "networkaccessmanagerproxy.h"
#include "networkaccessmanagerproxy_p.h"

#include "webpageproxy.h"

#include <qnetworkcookie.h>
#include <qnetworkrequest.h>

NetworkAccessManagerProxy *NetworkAccessManagerProxy::m_primaryManager = 0;

/*
    NetworkAccessManagerProxy inserts a pointer to the QWebPage in the
    QNetworkRequest before passing it on.
 */
NetworkAccessManagerProxy::NetworkAccessManagerProxy(QObject *parent)
    : QNetworkAccessManager(parent)
    , m_webPage(0)
{
}

void NetworkAccessManagerProxy::setWebPage(WebPageProxy *page)
{
    Q_ASSERT(page);
    m_webPage = page;
}

void NetworkAccessManagerProxy::setPrimaryNetworkAccessManager(NetworkAccessManagerProxy *manager)
{
    Q_ASSERT(manager);
    m_primaryManager = manager;
    setCookieJar(new NetworkCookieJarProxy(this));

    connect(this, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)),
            m_primaryManager, SIGNAL(authenticationRequired(QNetworkReply*, QAuthenticator*)));
    connect(this, SIGNAL(finished(QNetworkReply *)),
            m_primaryManager, SIGNAL(finished(QNetworkReply *)));
    connect(this, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)),
            m_primaryManager, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)));
#ifndef QT_NO_OPENSSL
    connect(this, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)),
            m_primaryManager, SIGNAL(sslErrors(QNetworkReply*, const QList<QSslError>&)));
#endif
}

QNetworkReply *NetworkAccessManagerProxy::createRequest(QNetworkAccessManager::Operation op, const QNetworkRequest &request, QIODevice *outgoingData)
{
    if (m_primaryManager && m_webPage) {
        QNetworkRequest pageRequest = request;
        m_webPage->populateNetworkRequest(pageRequest);
        return m_primaryManager->createRequest(op, pageRequest, outgoingData);
    }
    return QNetworkAccessManager::createRequest(op, request, outgoingData);
}


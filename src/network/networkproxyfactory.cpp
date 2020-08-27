/*
 * Copyright 2009 Benjamin K. Stuhl <bks24@cornell.edu>
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

#include "networkproxyfactory.h"

NetworkProxyFactory::NetworkProxyFactory()
    : QNetworkProxyFactory()
{
}

void NetworkProxyFactory::setHttpProxy(const QNetworkProxy &proxy)
{
    m_httpProxy = proxy;
}

void NetworkProxyFactory::setGlobalProxy(const QNetworkProxy &proxy)
{
    m_globalProxy = proxy;
}

QList<QNetworkProxy> NetworkProxyFactory::queryProxy(const QNetworkProxyQuery &query)
{
    QList<QNetworkProxy> ret;

    if (query.protocolTag() == QLatin1String("http") && m_httpProxy.type() != QNetworkProxy::DefaultProxy)
        ret << m_httpProxy;
    ret << m_globalProxy;

    return ret;
}


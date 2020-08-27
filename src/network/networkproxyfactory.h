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

#ifndef NETWORKPROXYFACTORY_H
#define NETWORKPROXYFACTORY_H

#include <qnetworkproxy.h>

class NetworkProxyFactory : public QNetworkProxyFactory
{
public:
    NetworkProxyFactory();

    void setHttpProxy(const QNetworkProxy &proxy);
    void setGlobalProxy(const QNetworkProxy &proxy);

    virtual QList<QNetworkProxy> queryProxy(const QNetworkProxyQuery &query = QNetworkProxyQuery());

private:
    QNetworkProxy m_httpProxy;
    QNetworkProxy m_globalProxy;
};

#endif // NETWORKPROXYFACTORY_H

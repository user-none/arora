/*
 * Copyright 2008 Benjamin C. Meyer <ben@meyerhome.net>
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

#ifndef LOCATIONBARSITEICON_H
#define LOCATIONBARSITEICON_H

#include <qlabel.h>

class WebView;
class LocationBarSiteIcon : public QLabel
{
    Q_OBJECT

public:
    LocationBarSiteIcon(QWidget *parent = 0);
    void setWebView(WebView *webView);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void webViewSiteIconChanged();

private:
    WebView *m_webView;
    QPoint m_dragStartPos;

};

#endif // LOCATIONBARSITEICON_H


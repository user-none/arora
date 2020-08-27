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

#include <qapplication.h>

#include "squeezelabel.h"

int main(int argc, char **argv)
{
    QApplication application(argc, argv);

    SqueezeLabel label;
    if (application.arguments().count() > 1)
        label.setText(application.arguments().value(1));
    else
        label.setText("This is one long sentence that I will have to ponder the meaning of");
    label.show();
    label.resize(100, label.height());

    return application.exec();
}


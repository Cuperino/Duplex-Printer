/*  Duplex Printer
* A virtual printer emulates various ecological features on physical printers.
* Copyright (C) 2014  Javier Oscar Cordero Pérez <javier.cordero@upr.edu>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef UNITS_H
#define UNITS_H

#include <QString>
#include <QMap>

class units
{
public:
    units();
    float factor( QString );
    QList<QString> list();
private:
    QMap<QString,float> _units;
};

#endif // UNITS_H

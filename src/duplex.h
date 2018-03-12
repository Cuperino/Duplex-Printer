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
#ifndef DUPLEX_H
#define DUPLEX_H

#include <QString>
#include <QSettings>

namespace DUPLEX
{
    enum Flip{ NO_FLIP, FLIP_LONG, FLIP_SHORT };
    enum Rotate{ _none, _180deg, _90degClockwise, _90degCounter };
}

class Duplex
{
public:
    Duplex();
    Duplex( QString );
    bool isSetup( );
    bool isSetup( QString );
    int setup( DUPLEX::Flip, DUPLEX::Rotate );
    int setup( QString, DUPLEX::Flip, DUPLEX::Rotate );
    QString getName();
    int getFlip();
    int getFlip( QString );
    int getRotation();
    int getRotation( QString );

private:
    QString _PrinterName;
    QSettings settings;
};

#endif // DUPLEX_H

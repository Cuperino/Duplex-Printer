/*  Duplex Printer
* Virtual printer adds ecological features to physical printers.
* Copyright (C) 2014  Javier Oscar Cordero Pérez <javier@imaginary.tech>
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
#include "units.h"

units::units()
{
    // UNITS MAP The user may specify margins & borders in their unit of choice.
    // This will garantee a good render, since every OS aproximates differently.
    _units.insert( "in", 1.0 );
    _units.insert( "ft", 12.0 );
    _units.insert( "pt", 1.0 / 72.0 );
    _units.insert( "m", 100.0 / 2.54 );
    _units.insert( "dm", 10.0 / 2.54 );
    _units.insert( "cm", 1.0 / 2.54 );
    _units.insert( "mm", 0.1 / 2.54 );
}

float units::factor( QString unit )
{
    QMapIterator<QString,float> iter(_units);
    do
    {
        iter.next();
        if ( !static_cast<bool>( QString::compare(
                                     iter.key(), unit, Qt::CaseInsensitive) ) )
            return iter.value();
    } while ( iter.hasNext() );
    return 0;
}

QList<QString> units::list()
{
    return _units.keys();
}

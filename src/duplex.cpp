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
#include "duplex.h"

Duplex::Duplex()
{

}

Duplex::Duplex( QString PrinterName )
{
    _PrinterName = PrinterName;
}

bool Duplex::isSetup( )
{
    return isSetup( _PrinterName );
}
bool Duplex::isSetup( QString PrinterName )
{
    settings.beginGroup("Printer");
    QStringList groups = settings.childGroups();
    settings.endGroup();
    settings.sync();

    foreach( QString group, groups )
        if ( PrinterName==group )
            return true;
    return false;
}

int Duplex::setup(DUPLEX::Flip flip, DUPLEX::Rotate rotate)
{
    if ( !_PrinterName.isEmpty() )
        return setup(_PrinterName, flip, rotate);
    return -1;
}

int Duplex::setup(QString PrinterName, DUPLEX::Flip flip, DUPLEX::Rotate rotate)
{
    settings.beginGroup("Printer/"+PrinterName+"/Duplex");
    settings.setValue("Flip", flip);
    settings.setValue("Rot", rotate);
    settings.endGroup();
    settings.sync();
    return 0;
}

QString Duplex::getName()
{
    return _PrinterName;
}

int Duplex::getFlip()
{
    return getFlip( _PrinterName );
}
int Duplex::getFlip( QString PrinterName )
{
    return settings.value("Printer/"+PrinterName+"/Duplex/Flip").toInt();
}

int Duplex::getRotation()
{
    return getRotation( _PrinterName );
}
int Duplex::getRotation( QString PrinterName )
{
    return settings.value("Printer/"+PrinterName+"/Duplex/Rot").toInt();
}

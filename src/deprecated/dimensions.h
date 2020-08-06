/*  This is a fork of the dimen.c lib, part of the debian libpaper package.
 *  Copyright (C) Javier Cordero <javier.cordero@upr.edu>, 2014.
 *  Copyright (C) Yves Arrouye <Yves.Arrouye@marin.fdn.fr>, 1996.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIMENSIONS_H
#define DIMENSIONS_H

#include <QString>
#include <QtPrintSupport/QPrinterInfo>

//using namespace QPrinter;
//template <class T_PAPER>
class dimensions
{
public:
    // CONSTRUCTOR
    dimensions();
    // FUNCTIONS
    //

private:
    // OBJECTS
    // Array of units, contains unit names and conversion factors
    /*struct _Units {
        QString name;
        float factor;
    }_Units units[7];*/
    // Array of units, contains unit names and conversion factors
    /*struct _Paper {
        QString name;
        QPrinter::PaperSize Qsize;
        int width;
        int height;
    } paper[];*/
    //enum dimension{ Width, Height };
    // FUNCTIONS
    //float unitfactor( QString unit );
};

#endif // DIMENSIONS_H

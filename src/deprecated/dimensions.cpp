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

#include "dimensions.h"

dimensions::dimensions()
{
/*    units[7] = {
        { "in", 1.0 },
        { "ft", 12.0 },
        { "pt", 1.0 / 72.0 },
        { "m",  100.0 / 2.54 },
        { "dm", 10.0 / 2.54 },
        { "cm", 1.0 / 2.54 },
        { "mm", 0.1 / 2.54 }
        //{ NULL, 0, 0 }
    };
*/
/*
    paper[] = {
        // Most common sizes go first
        // Custom QPaperSize correspond to page formats
        //  NameStr / QPaperSize / Width / Height //
        { "A5",             A4,   421,   595 }, // 14.85cm * 21cm
        { "B5",             B5,   499,   709 }, // 17.6cm * 25cm
        { "Letter",     Letter,   612,   792 }, // 8.5in * 11in
        { "Legal",       Legal,   612,  1008 }, // 8.5in * 14in
        { "Executive", Executive, 540,   720 }, // 7.6in * 10in
        { "A0",             A0,  2382,  3369 }, // 84cm * 118.8cm
        { "A1",             A1,  1684,  2382 }, // 59.4cm * 84cm
        { "A2",             A2,  1191,  1684 }, // 42cm * 59.4cm
        { "A3",             A3,   842,  1191 }, // 29.7cm * 42cm
        { "A4",             A4,   595,   842 }, // 21cm * 29.7cm
        { "A6",             A6,   297,   420 }, // 10.5cm * 14.85 cm
        { "A7",             A7,   210,   297 }, // 7.4cm * 10.5cm
        { "A8",             A8,   148,   210 }, // 5.2cm * 7.4cm
        { "A9",             A9,   105,   148 }, // 3.7cm * 5.2cm
        { "A10",           A10,    73,   105 }, // 2.6cm * 3.7cm
        { "B0",             B0,  2835,  4008 }, // 100cm * 141.4cm
        { "B1",             B1,  2004,  2835 }, // 70.7cm * 100cm
        { "B2",             B2,  1417,  2004 }, // 50cm * 70.7cm
        { "B3",             B3,  1001,  1417 }, // 35.3cm * 50cm
        { "B4",             B4,   709,  1001 }, // 25cm * 35.3cm
        { "B6",             B6,   354,   499 }, // 12.5cm * 17.6cm
        { "B7",             B7,   0,0        }, // 88 x 125 mm
        { "B8",             B8,   0,0        }, // 62 x 88 mm
        { "B9",             B9,   0,0        }, // 44 x 62 mm
        { "B10",           B10,   0,0        }, // 31 x 44 mm
        { "JISB0",       JisB0,  2920,  4127 }, // 103.0cm * 145.6cm
        { "JISB1",       JisB1,  2064,  2920 }, // 72.8cm * 103.0cm
        { "JISB2",       JisB2,  1460,  2064 }, // 51.5cm * 72.8cm
        { "JISB3",       JisB3,  1032,  1460 }, // 36.4cm * 51.5cm
        { "JISB4",       JisB4,   729,  1032 }, // 25.7cm * 36.4cm
        { "JISB5",       JisB5,   516,   729 }, // 18.2cm * 25.7cm
        { "JISB6",       JisB6,   363,   516 }, // 12.8cm * 18.2cm
        { "C0",     EnvelopeC0,  2599,  3677 }, // 91.7cm * 129.7cm
        { "C1",     EnvelopeC1,  1837,  2599 }, // 64.8cm * 91.7cm
        { "C2",     EnvelopeC2,  1298,  1837 }, // 45.8cm * 64.8cm
        { "C3",     EnvelopeC3,   918,  1298 }, // 32.4cm * 45.8cm
        { "C4",     EnvelopeC4,   649,   918 }, // 22.9cm * 32.4cm
        { "C5",            C5E,   459,   649 }, // 16.3cm * 22.9cm
        { "C6",     EnvelopeC6,   323,   459 }, // 11.4cm * 16.2cm
        { "Ledger",     Ledger,  1224,   792 }, // 17in * 11in
        { "Tabloid",   Tabloid,   792,  1224 }, // 11in * 17in
        { "Atatement",  Custom,   396,   612 }, // 5.5in * 8.5in
        { "Folio",       Folio,   612,   936 }, // 8.5in * 13in
        { "Quarto",     Quarto,   610,   780 }, // 8.5in * 10.83in
        { "10x14", Imperial10x14, 720,  1008 }, // 10in * 14in
        { "archE",       ArchE,  2592,  3456 },	// 34in * 44in
        { "archD",       ArchD,  1728,  2592 },	// 22in * 34in
        { "archC",       ArchC,  1296,  1728 },	// 17in * 22in
        { "archB",       ArchB,   864,  1296 },	// 11in * 17in
        { "archA",       ArchA,   648,   864 },	// 8.5in * 11in
        { "flsa",       Custom,   612,   936 }, // U.S. foolscap: 8.5in * 13in
        { "flse",       Custom,   612,   936 }, // European foolscap
        { "halfletter", Custom,   396,   612 }, // 5.5in * 8.5in
        { NULL, 0, 0 }
    };
*/
}

/*
// Get unit conversion factor
float dimensions::ufactor( QString unit )
{
    for (int i=0; units[i].name; ++i)
        if ( !QString::compare( units[i].name, unit, Qt::CaseInsensitive ) )
            return units[i].factor;
    return 0;
}
*/

/*
// Find measurement value for given paper
float dimensions<T_PAPER>::findPMeasure( T_PAPER paper, dimension d )
{
    // Column that contains width or height... Which dimension should we get?
    int col = d==dimension::Width ? 2 : 3;

    // Searching for a QPaper o a string of the paper's name?
    //if ( sizeOf(T_PAPER)==sizeOf(PaperSize) )
    //    for (int i=0; paper[i].name; ++i)
    //        if ( !QString::compare( units[i].name, unit, Qt::CaseInsensitive ) )
    //else

    return 0;
}
*/

// PUBLIC

/*
// Convert a measures to PostScript pica units
int dimensions::psMeasurement( double base, QString unit )
{
    return base * ufactor(unit) * 72;
}
*/

/*
int dimensions<T_PAPER>::width ( T_PAPER paper )
{
    return findPMeasure( paper, dimension::Width );
}

int dimensions<T_PAPER>::height ( T_PAPER paper )
{
    return findPMeasure( paper, dimension::Height );
}
*/

// QT5.3
//QPrinterInfo::supportsCustomPageSizes()

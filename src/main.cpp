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
 * b2ut WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QApplication>
#include <QCommandLineParser>
#include "printerdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Program details.
    QCoreApplication::setApplicationName("Duplex Printer");
    QCoreApplication::setApplicationVersion("0.20");
    QCoreApplication::setOrganizationName("dualPrint");
    QCoreApplication::setOrganizationDomain("dualPrint.org");

    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::translate("A virtual printer that emulates various ecological features on physical printers.","Program description."));
    parser.addPositionalArgument(QCoreApplication::translate("source", "Source file that will be passed."),QCoreApplication::translate("Source PostScript file or parse", "Describes the user could parse a PostScript source on the command line, either as a file location or the PostScript source itself."));
    parser.addVersionOption();
    parser.addHelpOption();
    parser.process( a );

    // Get list of positional arguments.
    QStringList positionalArguments = parser.positionalArguments();
    PrinterDialog w;
    w.runArguments(positionalArguments);
    w.show();
    return a.exec();
}

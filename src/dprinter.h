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
#ifndef DPRINTER_H
#define DPRINTER_H
#include <QApplication>
#include <QObject>
#include <QThread>
#include <QString>
#include "units.h"
// File management
#include <QTextStream>
#include <QFile>
#include <QDir>
// Multiplatform access to printer information
#include <QtPrintSupport/QPrinterInfo>
// Load external and parallel processes
#include <QProcess>
#include <QDebug>

class dPrinter : public QObject
{
    Q_OBJECT
public:
    // Constructors &~
    explicit dPrinter(QObject *parent = 0);
    ~dPrinter();
    // Multithreading setup
    void setupThread_Process( QThread * );
    void setupThread_FirstPrint( QThread * );
    void setupThread_SecondPrint( QThread * );
    // FILE MANAGEMENT
    void consoleStreamParse( );
    // SETTERS AND GETTEES
    void set(QPrinterInfo);
    QPrinterInfo getPrinter();
    int setFileLoc( const QString & );
    void setPipeEmulation( bool );
    bool getPipeEmulation( );
    // INSTRUCTIONS FROM UI
    void setSourceHeight( qreal );
    void setSourceWidth( qreal );
    void setDestinationHeight( qreal );
    void setDestinationWidth( qreal );
    void setMargin( int );
    void setMargin( float, QString );
    void setBorder( int );
    void setBorder( float, QString );
    void setSelection( QString selection );
    int validateRange(QString &);
    int setPagesPerSide( int );
    void setLayout( bool, bool, bool );
    void setDuplex( int );
    void setReverse( bool );
    void setToast( bool );
    void setColorProfile( int );
    void setCopies( int );
    void setCollate( bool );
    void print( QString );
    QString state();
    int waitStateChange();

signals:
    void processProgress( int progress );
    void printComplete( int printNumber );
    void error( int id, QString message );

public slots:
    void firstPrint();
    void secondPrint();
    void process();

private:
    // Objects
    QPrinterInfo _printer;
    // Values
    QString _selection, _inFileName;
    enum Orientation{portrait,landscape} _orientation;
    enum Duplex{duplex,single,odd,even} _duplex;
    enum ColorProfile{monochrome,grayscale,color} _colorProfile;
    int _margin, _border, _pagesPerSide, _copies;
    qreal _sourceWidth, _destinationWidth, _sourceHeight, _destinationHeight;
    bool _emulatePipes, _toast, _horizontal_Layout, _leftToRight_Layout,
         _topToButton_Layout, _reverse, _collate;
    //> File Processing
    QFile _File_A, _File_B;
    QTextStream /*QFile*/ Stream_A, Stream_B, *in, *out;

    // Functions
    float unitFactor( QString );
    double nextdiv(int, int);
    void setOrientation( Orientation );
    int total();
};

#endif // DPRINTER_H

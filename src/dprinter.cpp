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
#include "dprinter.h"

// CONSTRUCTORS AND DECONSTRUCTOR
dPrinter::dPrinter(QObject *parent) :
    QObject(parent)
{
    // Default values
    _toast=_horizontal_Layout=_leftToRight_Layout=_topToButton_Layout=_collate
        =true;
    _emulatePipes=_reverse=false;
    _copies=_pagesPerSide=1;
    _sourceWidth=_destinationWidth=590;    // Letter size
    _sourceHeight=_destinationHeight=792;
    _margin=36;  // .50 in
    _border=21;  // .3 in
    _selection="all";
    _inFileName="";
    _orientation=portrait;
    _duplex=duplex;
    _colorProfile=monochrome;
}
dPrinter::~dPrinter()
{
    // Delete files from hard drive, if any.
    // Reminder: files are saved on disk when emulating pipes. Windows needs...
    //for ( int i=0; i<6; i++ ) //6 >= PROCESSAMNT, bellow.
        //if ( "p"+QString::number(i)+".ps" exists )
            //QFile::remove( "p"+QString::number(i)+".ps" );
}
// THREAD SETUPS
void dPrinter::setupThread_Process( QThread *cThread )
{
    connect(cThread, SIGNAL(started()),
            this, SLOT(process()), Qt::UniqueConnection);
}
/*
void dPrinter::setupThread_FirstPrint( QThread *cThread )
{
    connect(cThread, SIGNAL(started()), this, SLOT(firstPrint()));
}
*/
void dPrinter::setupThread_SecondPrint( QThread *cThread )
{
    connect(cThread, SIGNAL(started()),
            this, SLOT(secondPrint()), Qt::UniqueConnection);
}

// GET FILES
int dPrinter::setFileLoc(const QString & fileLoc )
{
    // Save file location
    _inFileName = fileLoc;
    // Load file, return 1 if fails.
    //_File_A.setFileName(fileLoc);
    //if (!_File_A.open(QIODevice::ReadWrite | QIODevice::Text))
    //    return 1;
    //Stream_A.setDevice(&_File_A);
    // Ready output.
    //_File_B.setFileName(QDir::tempPath()+"/tmpB.ps");
    //if (!_File_B.open(QIODevice::ReadWrite | QIODevice::Text))
    //    return 2;
    //Stream_B.setDevice(&_File_B);
    // Point into Stream_A
    //in=&Stream_A;
    //out=&Stream_B;
    return 0;
}
void dPrinter::consoleStreamParse( )
{
    // QFile object handles stdin, Stream_A points to the QFile handler.
    QFile inFile;
        inFile.open(stdin, QIODevice::ReadOnly, QFileDevice::DontCloseHandle);
    Stream_A.setDevice(&inFile);
    in=&Stream_A;
}
void dPrinter::setPipeEmulation( bool emulate )
{
    _emulatePipes = emulate;
}
bool dPrinter::getPipeEmulation( )
{
    return _emulatePipes;
}

// SET OPERATIONS
//> SET PRINTER
void dPrinter::set(const QPrinterInfo printer)
{
    _printer = printer;
}
QPrinterInfo dPrinter::getPrinter()
{
    return _printer;
}
//> SET MARGIN AND BORDER
void dPrinter::setMargin( int margin )
{
    _margin = margin;
}
void dPrinter::setMargin( float base, QString factor )
{
    units unit;
    setMargin( base*72*unit.factor( factor ) );
}
void dPrinter::setBorder( int border )
{
    _border = border;
}
void dPrinter::setBorder( float base, QString  factor )
{
    units unit;
    setBorder( base*72*unit.factor( factor ) );
}
//> SET SOURCE PAGE SIZE
void dPrinter::setSourceHeight( qreal value )
{
    _sourceHeight = value;
}
void dPrinter::setSourceWidth( qreal value )
{
    _sourceWidth = value;
}
//> SET DESTINATION PAGE SIZE
void dPrinter::setDestinationHeight( qreal value )
{
    _destinationHeight = value;
}
void dPrinter::setDestinationWidth( qreal value )
{
    _destinationWidth = value;
}
//> SET SELECTION
void dPrinter::setSelection( QString selection )
{
    _selection = selection;
}
//*> SET PAGES PER SIDE
int dPrinter::setPagesPerSide( int nup )
{
    // PSnUp Layout Optimization
    const int tolerance = 100000;  // PSnUp default.

    // PAGE SIZE
    const int width = _destinationWidth,
              height = _destinationHeight;

    // INNER PAGE SIZE
    // Subtract paper margins from height & width.
    const int ppwid = width - _margin*2,
                 pphgt = height - _margin*2;

    // VALIDATION
    if (ppwid <= 0 || pphgt <= 0)
        return -1;

    // Finding the best layout is an optimization problem. We try all of the
    // combinations of width*height in both normal and rotated form, and
    // minimise the wasted space.
    double best = tolerance;
    for (double horizontal=1; horizontal!=0; horizontal=nextdiv(horizontal,nup))
    {
        double vertical = nup/horizontal;
        // Try normal orientation first.
        double scl = std::min(pphgt/(height*vertical),ppwid/(width*horizontal));
        double optim = (ppwid-scl*width*horizontal)*(ppwid-scl*width*horizontal)
                       +(pphgt-scl*height*vertical)*(pphgt-scl*height*vertical);
        if (optim < best)
        {
            best = optim;
            setOrientation(portrait);
        }
        // Try rotated orientation.
        scl = std::min(pphgt/(width*horizontal), ppwid/(height*vertical));
        optim = (pphgt-scl*width*horizontal)*(pphgt-scl*width*horizontal)
                +(ppwid-scl*height*vertical)*(ppwid-scl*height*vertical);
        if (optim < best)
        {
            best = optim;
            setOrientation(landscape);
        }
    }

    // Fail if nothing better than worst tolerance was found.
    if (best == tolerance)
    {
        if ( _pagesPerSide < nup )
            return setPagesPerSide(++nup);
        else if ( _pagesPerSide > nup )
            return setPagesPerSide(--nup);
    }
    else
        _pagesPerSide = nup;
    return nup;
}
//>> Return next larger exact divisor of number, or 0 if none.
double dPrinter::nextdiv(int n, int m)
{
    while (n++ <= m)
        if (m%n == 0)
            return n;
    return 0;
}
//> SET ORIENTATION
void dPrinter::setOrientation( Orientation value )
{
    _orientation = value;
}
//> SET LAYOUT ORDER
void dPrinter::setLayout( bool horizontal_priority, bool leftToRight_reading,
                bool topToButton_reading )
{
    _horizontal_Layout = horizontal_priority;
    _leftToRight_Layout = leftToRight_reading;
    _topToButton_Layout = topToButton_reading;
}
//*> SET DUPLEX
void dPrinter::setDuplex( int index )
{
    _duplex = static_cast<Duplex>(index);
}
void dPrinter::setReverse( bool checked )
{
    _reverse = checked;
}
//> SET TOAST
void dPrinter::setToast( bool value )
{
    _toast = value;
}
//> SET COLOR PROFILE
void dPrinter::setColorProfile( int index )
{
    _colorProfile = static_cast<ColorProfile>(index);
}
//*> SET COPIES
void dPrinter::setCopies( int value )
{
    _copies = value;
}
//*> SET COLLATE
void dPrinter::setCollate( bool checked )
{
    _collate = checked;
}
// INPUT VALIDATION
int dPrinter::validateRange( QString &str )
{
    // Declare int, dash and position counters.
    int i=0, d=0, p, fullNum=0;
    // Fist pass. Remove undesired characters.
    for ( p=0; p<str.length(); p++ )
        if ( !( str.at(p).isDigit() || str.at(p)==',' || str.at(p)=='-' ) )
        {
            str.remove(p,1);
            p--;
        }
    // Seccond pass. Detect logical errors.
    for ( p=0; p<str.length(); p++ )
        if ( str.at(p)==',' )
        {
            // Remove immediate ',' duplicates plus those at start and end pos.
            if ( i==0 || p+1==str.length() )
            {
                str.remove(p,1);
                p--;
            }
            else if ( fullNum==0 )
                break;
            // For each comma, reset integer and dash subrange counters.
            i=d=fullNum=0;
        }
        else if ( str.at(p)=='-' )
        {
            // Make sure there's no more than 1 dash per sub range.
            d++;
            // Remove immediate '-' duplicates plus those at start and end pos.
            if (p+1==str.length() || str.at(p+1)=='-' )
            {
                str.remove(p,1);
                d--;
                p--;
            }
            // If there is more than a dash per subrange, break and notify user.
            else if ( d>1 || i==0 || /*str.at(p+1)==',' ||*/ fullNum==0 )
                break;
            fullNum=0;
        }
        // Increment the count of integers per subrange.
        else
        {
            i++;
            fullNum = fullNum*10 + str.at(p).digitValue();
        }
    // end for
    // If there's an error the user must fix, return its position,
    if ( p!=str.length() )
        return p;
    // Otherwise return -1.
    return -1;
}

// TOTAL
int dPrinter::total( )
{
    // Variables
    int total=0,  // selection's total
    pageCount=0, docTotal=0;  // number of pages and total value in document

    // Count file's pages and save Total of pages value.
    if ( _inFileName!="" )
    {
        QFile file( _inFileName );
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return 1;
        QTextStream in(&file);
        while ( !in.atEnd() )
        {
            QString line = in.readLine();
            if ( line.startsWith("%%Pages:") )
            {
                QStringList lineElements = line.split(" ");
                if ( lineElements.length()>0 &&
                     lineElements.at(1) != "(atend)" )
                {
                    line = lineElements.at(1);
                    docTotal = line.toInt();
                }
            }
            else if ( line.startsWith("%%Page:") )
                pageCount++;
        }
        file.close();

        // If total and page count don't match, assume largest value.
        if ( pageCount!=docTotal )
            docTotal = pageCount>docTotal? pageCount : docTotal;
    }
    // WHOLE DOCUMENT
    if ( !static_cast<bool>( QString::compare(
                                _selection, tr("all"), Qt::CaseInsensitive ) ) )
        total = docTotal;
    // SELECTION OF PAGES
    else
    {
        // Splits _selection string into sub ranges.
        QStringList range = _selection.split(",");
        foreach ( QString subrange, range )
        {
            // Split sub ranges into first and last pages
            QStringList page = subrange.split("-");
            // Add the amount of pages in sub range to total acumulator.
            if ( page.length()== 1 && page.at(0).toInt()<=docTotal )
                // ,#,
                total++;
            else if ( page.length()==2 )
            {
                // ,#-#,
                int n0 = page.at(0).toInt(),
                    n1 = page.at(1).toInt();

                // Count reverse ranges as normal ranges.
                if ( n0 > n1 )
                {
                    // Swap
                    int tmp=n0;
                    n0=n1;
                    n1=tmp;
                }

                // If the sub range is out of bounds, don't acumulate.
                if ( n0>docTotal )
                    break;

                // If part of the sub range is out of bounds, cap it.
                if ( n1>docTotal )
                    n1=docTotal;

                // Acumulate sub range to total.
                total += n1-n0+1;

            } // end else if
        } // end foreach
    } // end else
    return total/_pagesPerSide;
}

int dPrinter::waitStateChange()
{
    if ( _printer.state() != 1 )
        while ( static_cast<int>(_printer.state()) == 1 )
            QThread::msleep(600);
    return static_cast<int>( _printer.state() );
}
QString dPrinter::state()
{
    QString ret="";
    switch ( static_cast<int>(_printer.state()) )
    {
        case 0 : ret = tr(" accepts jobs!"); break;  // Idle
        case 1 : ret = tr(" is operating."); break;  // Active
        case 2 : ret = tr(" aborted."); break;  // Aborted
        case 3 : ret = tr("... An error has ocurred with it."); break;  // Error
        default: throw;
    }
    return ret;
}

// PROCESS PIPE
void dPrinter::process()
{
    // Started signal.
    emit processProgress(0);
    //* Get Paths to each process from last session.
#if defined (WINNT) || defined (WIN32)
    QString GhostScript = "C:/Program Files/gs/gs9.14/bin/gswin64c.exe",
            PSselect = "psselect",
            PSnup = "psnup",
            PSblank = "C:/Program Files/DualPrint/DuplexPrinter/bin/PSblank.exe",
            //-Split = "C:/Program Files/DualPrint/DuplexPrinter/bin/wtee.exe",  //"wtee"
            dos2unix = "C:/Program Files/DualPrint/DuplexPrinter/bin/dos2unix.exe";
#else
    QString GhostScript = "gs",
            PSselect = "psselect",
            PSnup = "psnup",
            PSblank = "psblank",
            //-Split = "tee",
            dos2unix = "dos2unix";
#endif
    //+ Wondering if saving path in const will affect binary portabilty...
    const QString temp = QDir::tempPath()+"/";
    const QString outA = temp+"dpA.ps",
                  outB = temp+"dpB.ps",
                  outC = temp+"dpC.ps";
    const int PROCESSES_AMNT = 8;
    int first=0, last=0;
    // Objects will be set up and used as necesary.
    struct sProcess
    {
        bool run;         // Set true is process is needed to run.
        QString path;     // Name/location.
        QStringList args; // Arguments...
        QProcess * exec;  // The Process.
    } p[PROCESSES_AMNT];
    // Init p[array]
    for (int i=0; i<PROCESSES_AMNT; i++)
    {
        p[i].run = false;
        p[i].path = "";
        p[i].exec = NULL;
        //QStringList are empty by default.
    }

    // Set up arguments for each process.
    //> p0: Toast print using PostScript. Avoid incompatibilities with PSutils.
    if ( _toast==true || _colorProfile!=color )
    {
        p[0].path = GhostScript;
        p[0].args << "-sOutputFile=-" << "-q";  // Use stdout and Quiet mode.
        // Redirect remaining messages to STDERR
#if defined(WINNT) || defined(WIN32)
        p[0].args << "-sstdout=%%stderr";
#else
        p[0].args << "-sstdout=%stderr";
#endif
        p[0].args << "-dNOPAUSE" << "-dBATCH"  // Prompt nothing.
                  << "-sDEVICE=ps2write";      // Output in PostScript format.
        // Convert to grayscale and use black ink or toner only.
        if ( _colorProfile==grayscale || _colorProfile==monochrome )
            p[0].args << "-sColorConversionStrategy=Gray";
        if ( _colorProfile==monochrome )
            p[0].args << "-sProcessColorModel=DeviceGray";
        p[0].args << "-";                      // Use stdin.
        p[0].run = true;
    }
    //> p1: Prepare PSselect for Page Selection if necesary.
    if ( static_cast<bool>( QString::compare(
                                _selection, "all", Qt::CaseInsensitive ) ) )
    {
        p[1].path = PSselect;
        p[1].args << "-q" << "-p"+_selection;
        p[1].run = true;
    }
    //> p2: Prepare PSnUp only if more than one page per side will be printed.
    if ( _pagesPerSide>1 )
    {
        p[2].path = PSnup;
        p[2].args     << "-q"
                      << "-w"+QString::number(_sourceWidth)
                      << "-h"+QString::number(_sourceHeight)
                      << "-W"+QString::number(_destinationWidth)
                      << "-H"+QString::number(_destinationHeight);
        if (!_horizontal_Layout)
            p[2].args << "-c";
        if (!_topToButton_Layout)
            p[2].args << "-l";
        if (!_leftToRight_Layout)
            p[2].args << "-r";
        p[2].args     << "-m"+QString::number(_margin)
                      << "-b"+QString::number(_border)
                      << "-d1" // Draw border line 1 point width.
                      << "-nup" << QString::number(_pagesPerSide);
        p[2].run = true;
    }    
    //> p5single: FIRST PRINT => single & reverse
    if ( _reverse && _duplex==single )
    {
        p[5].path = PSselect;
        p[5].args << "-q" << "-r";
        p[5].run = true;
    }
    //> p5odd || 6: => odd, duplex + reverse
    else if ( _duplex==odd || _duplex==duplex )
    {
        int id = _duplex==odd? 5 : 6;
        p[id].path = PSselect;
        p[id].args << "-q" << "-o";
        if ( _duplex==duplex? true : _reverse )
            p[id].args << "-r";
        p[id].run = true; // Only p[4] will start by QT, duplex case does Split.
    }
    //> p5even || 7: => even + reverse, duplex <= SECOND PRINT
    //>> p3: Blank page
    if ( _duplex==even || _duplex==duplex )
    {
        int id = _duplex==even? 5 : 7;
        p[id].path = PSselect;
        p[id].args << "-q" << "-e";
        if ( _reverse )
            p[id].args << "-r";
        p[id].run = true;
        //> p3: Add extra blank page for duplex if the total of pages is uneven.
        if ( total()%2==1 && total()>1 )
        {
            p[3].path = PSblank;
            p[3].args << "1";  // Add 1 blank page.
            p[3].run = true;

            // EOL must be fixed when using PSBlank's stdout on Windows.
#if defined(WINNT) || defined(WIN3)
            p[4].path = dos2unix;
            p[4].run = true;
#endif
        }
    }
    //> p5duplex: Split pipe in two
    if ( _duplex==duplex )
    {
        /* Tee and wtee cannot be succesfully started from QT Framework 5.3
        *  as of July 2014. Pipe split will be emmulated for all supported OS.
        * p[5].path = Split+" >( "+p[5].path+" "+p[5].args.join(" ")+" > "+outA+
        *            " ) >( "+p[6].path+" "+p[6].args.join(" ")+" > "+outB+" )";
        * qDebug() << p[4].path;
        * p[5].run = true;
        */
        // Prevent split receiving process from being started on the main pipe.
        p[PROCESSES_AMNT-2].run = p[PROCESSES_AMNT-1].run = false; // p6 & 7 false
    }

    // Initialize needed QProcess
    for ( int i=0; i<PROCESSES_AMNT; i++ )
        if ( p[i].run )
            p[i].exec = new QProcess( this );

    // Get last process //-and Set its output destination.
    for ( int i=PROCESSES_AMNT-1 ; i>=0; i-- )
        if ( p[i].run )
        {
            last=i;
            switch ( _duplex )
            {
                case single:
                case even:  // Duplex's outC file will be used as infile ahead.
                case odd: p[i].exec->setStandardOutputFile( outA ); break;
                case duplex: p[i].exec->setStandardOutputFile( outC ); break;
                default: return;// 20;
            }
            break;
        }

    // Get first process' ID to feed it input.
    for ( int i=0 ; i<=last; i++ )
        if ( p[i].run )
        {
            first=i;
            if ( _inFileName!="" )
                p[first].exec->setStandardInputFile( _inFileName );
            break;
        }

    // Pipe remaining processes together.
    if ( _emulatePipes )
    {
        // EMULATE PIPES by saving to and reading from disk.
        // I wish there was another way, but pipes in Windows won't make their
        // .. processes wait for input.
        for ( int i=last-1; i>=first; i-- )
            if ( p[i].run )
                p[i].exec->setStandardOutputFile("dPp"+QString::number(i)+".ps");
        for ( int i=first+1, prv=first; i<=last; i++ )
            if ( p[i].run )
            {
                p[i].exec->setStandardInputFile("dPp"+QString::number(prv)+".ps");
                prv=i; // previous
            }
    }
    else
        // USE REGULAR PIPES
        for ( int i=last-1, next=last; i>=first; i-- )
            if ( p[i].run )
            {
                p[i].exec->setStandardOutputProcess( p[next].exec );
                next=i;
            }

    // Start main pipe.
    for ( int i=first; i<=last; i++ )
        if ( p[i].run )
        {
            // Started signal.
            emit processProgress(i+1);  // Process will be 1 above process #.
            p[i].exec->setStandardErrorFile(temp+"dP"+QString::number(i)+".log");
            //if ( p[i].args.isEmpty() )
            //    p[i].exec->start( p[i].path );
            //else
                p[i].exec->start( p[i].path, p[i].args );
            if ( _emulatePipes && !p[i].exec->waitForFinished( 40000 ) )
            {
                emit error( 100+i, tr("Something went wrong down the pipes.") );
                return;
            }
        }

    // Always wait for last process of main pipe to finish.
    //+ Wait for finished cannot be run twice in a row or will return fail.
    if ( !_emulatePipes && !p[last].exec->waitForFinished( 25000 ) )
    {
        emit error( 100+last, tr("Something went wrong down the pipes.") );
        return ;//last;
    }

    // If duplex, split in two files.
    emit processProgress( PROCESSES_AMNT-1 );
    if ( _duplex==duplex )
    {
        for (int i=PROCESSES_AMNT-2; i<PROCESSES_AMNT; i++)  // x2
        {
            p[i].exec = new QProcess( this );
            p[i].exec->setStandardInputFile( outC );
            p[i].exec->setStandardErrorFile(temp+"dP"+QString::number(i)+".log");
            p[i].exec->setStandardOutputFile( i==PROCESSES_AMNT-2? outA : outB );
            p[i].exec->start( p[i].path, p[i].args );
        }
        // Process are run async. Waits are performed after full batches run.
        if ( !p[PROCESSES_AMNT-2].exec->waitForFinished( 8000 ) )
        {
            emit error( 100+PROCESSES_AMNT-2,
                        tr("Something went wrong processing Side A.") );
            return;
        }
        emit processProgress( PROCESSES_AMNT );
        if ( !p[PROCESSES_AMNT-1].exec->waitForFinished( 8000 ) )
        {
            emit error( 100+PROCESSES_AMNT-1,
                        tr("Something went wrong processing Side B.") );
            return;
        }
    }
/*
    // PIPE DEBUGGER
    const int IN = 0, PIPE = 2, OUT = 4;
    qDebug() << _inFileName;

    // IN
    p[IN].exec->setStandardInputFile( _inFileName );
    p[PIPE].exec->setStandardInputFile( "t"+QString::number(IN)+".ps" );
    p[OUT].exec->setStandardInputFile( "t"+QString::number(PIPE)+".ps" );

    // PIPE
    //p[IN].exec->setStandardOutputProcess( p[PIPE].exec );
    //p[PIPE].exec->setStandardOutputProcess( p[OUT].exec );

    // OUT
    p[IN].exec->setStandardOutputFile( "t"+QString::number(IN)+".ps" );
    p[PIPE].exec->setStandardOutputFile( "t"+QString::number(PIPE)+".ps" );
    p[OUT].exec->setStandardOutputFile( "debug.ps" );

    // STDERR
    xwp[IN].exec->setStandardErrorFile( "p"+QString::number(IN)+".txt" );
    p[PIPE].exec->setStandardErrorFile( "p"+QString::number(PIPE)+".txt" );
    p[OUT].exec->setStandardErrorFile( "p"+QString::number(OUT)+".txt" );

    // RUN
    p[IN].exec->start( p[IN].path, p[IN].args );
    p[IN].exec->waitForFinished();
    qDebug() << p[IN].path <<' '<< p[IN].args
             << "\nIn state: " << static_cast<int>( p[IN].exec->state() )
             << ", exitcode: " << p[IN].exec->exitCode();

    p[PIPE].exec->start( p[PIPE].path, p[PIPE].args );
    p[PIPE].exec->waitForFinished();
    qDebug() << p[PIPE].path <<' '<< p[PIPE].args
             << "\nPipe state: "<< static_cast<int>( p[PIPE].exec->state() )
             << ", exitcode: "<< p[PIPE].exec->exitCode();

    p[OUT].exec->start( p[OUT].path, p[OUT].args );
    p[OUT].exec->waitForFinished();
    qDebug() << p[OUT].path <<' '<< p[OUT].args
             << "\nOut state: "<< static_cast<int>( p[OUT].exec->state() )
             << ", exitcode: "<< p[OUT].exec->exitCode();
*/
    // Process complete.
    emit processProgress( PROCESSES_AMNT+1 );
    return;
}

// PRINT
void dPrinter::print( QString file )
{
    //* Get path from last session.
    QStringList args;

#if defined (WINNT) || defined (WIN32)

    // WORKING - GSview silent print
    QString path = "C:/Program Files/Ghostgum/gsview/gsprint.exe";
    args << "-printer" << _printer.printerName()
         << "-copies" << QString::number(_copies);
    if ( _colorProfile==monochrome )
        args << "-mono";
    else if ( _colorProfile==grayscale )
        args << "-gray";
    else
        args << "-color";
    // Other "default" values and file
    args << "-noquery"
         << "-portrait" // orientation changes are done automatically by PSnup.
         << QDir::toNativeSeparators( file );

    // NON FUNCTIONAL - GhostScript silent print
    //QString path = "C:/Program Files/gs/gs9.14/bin/gswin64c.exe";
    //args //<< "-dNOPAUSE" << "-dBATCH"  // Prompt nothing.
    //     << "-sDEVICE=mswinpr"
    //     << "-dNoCancel"
    //     << "-sOutputFile=\"%%printer%%"+_printer.printerName()+"\""  // Will only work with printer names that contain no spaces.
    //     << "-";

#else
    // OS X and LINUX
    QString path = "lpr";
    args << "-P" << _printer.printerName()
         << "-#" //<< _copies
         << QDir::toNativeSeparators(file);
#endif

    QProcess *exec = new QProcess( this );
    //exec->setStandardInputFile( file );
    exec->setStandardErrorFile( file+".log" );
    exec->start( path, args );

    //+ WaitForFinished doesn't time out because printer may wait for physical
    // printer to finish. We'll be informed if some errors ocurr.
    if ( !exec->waitForFinished( -1 ) )
        emit error(200, tr("A problem ocurred while sending the print."));
    return;
}
void dPrinter::firstPrint()
{
    //* Rename files to Title gotten from Capture and send to print.
    //+ This allow users to see the Document's name when printing to a server.
    print( QDir::tempPath()+"/dpA.ps" );
    emit printComplete(0);
    //* Delete file
    return;
}
void dPrinter::secondPrint()
{
    //* Rename files to Title gotten from Capture and send to print.
    //+ This allow users to see the Document's name when printing to a server.
    print( QDir::tempPath()+"/dpB.ps" );
    emit printComplete(1);
    //* Delete file
    return;
}

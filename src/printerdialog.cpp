/*  Duplex Printer
* Virtual printer adds ecological features to physical printers.
* Copyright (C) 2014, 2020  Javier Oscar Cordero Pérez <javier@imaginary.tech>
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

#include "printerdialog.h"
#include "ui_printerdialog.h"

// Duplex emulation
#include "duplex.h"
#include "duplexsetupdialog.h"
#include "duplexdialog.h"
// Remaining windows
#include "marginsandborders.h"
#include "aboutdialog.h"

// CONSTRUCTOR
// Get printers, use default and Set values from last session.
PrinterDialog::PrinterDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PrinterDialog)
{
    ui->setupUi(this);
    ui->progressBar->setHidden(true);
    printer = new dPrinter;
    processThread = new QThread;

    // LIST PRINTERS
    // Save list of printers and the default's name.
    printerList = QPrinterInfo::availablePrinters();
    // Populate comboBoxPrinter list.
    QString printerName;
    bool defaultFound=false;
    foreach (QPrinterInfo printerInfo, printerList)
    {
        // Save current printer's name
        printerName = printerInfo.printerName();
        // Ommit "DuplexPrinter" and other virtual printers from the list.
        if ( printerName == "DuplexPrinter" /*|| printerName == "GreenCloud" ||
                           printerName.contains("PDF", Qt::CaseInsensitive)*/ )
            continue;
        else
        {
            // Add every other printer to the comboBox.
            ui->comboBox_Printer->addItem( printerName );
            // When system default printer is added,
            if ( printerInfo.isDefault() )
            {
                // Select it
                ui->comboBox_Printer->setCurrentIndex(
                            ui->comboBox_Printer->findText( printerName ) );
                defaultFound = true;
            }
        } // end else
    } // end foreach
    // If no default was found among the non virtual printers,
    // .. setCurrentIndex to get the first printer's data.
    if ( !defaultFound )
        ui->comboBox_Printer->setCurrentIndex( 0 );

    // VALUES FROM LAST SESSION
    ui->actionAlways_refry_PostScript->trigger();

    // Get MARGIN and BORDERS size from last session.
    if ( settings.contains("nup/margin") )
        printer->setMargin(settings.value("nup/margin").toInt());
    else
        printer->setMargin( 0.5, "in" );
    if ( settings.contains("nup/border") )
        printer->setBorder(settings.value("nup/border").toInt());
    else
        printer->setBorder( 0.3, "in" );
    // Get MultiPage layout from last session.
    if ( settings.contains("nup/layout") )
        switch( settings.value("nup/layout").toInt() )
        {
            case 0:
                ui->actionLeft_to_Right_Top_to_Bottom->setChecked(true); break;
            case 1:
                ui->actionRight_to_Left_Top_to_Bottom->setChecked(true); break;
            case 2:
                ui->actionTop_to_Bottom_Left_to_Right->setChecked(true); break;
            case 3:
                ui->actionTop_to_Bottom_Right_to_Left->setChecked(true); break;
            default: throw;
        }
    else
        ui->actionLeft_to_Right_Top_to_Bottom->setChecked(true);

    // SETUP SIGNALS AND SLOTS
    connect( printer, SIGNAL(processProgress(int)),
             this, SLOT(ProcessProgress(int)), Qt::UniqueConnection );
    connect( printer, SIGNAL(printComplete(int)),
             this, SLOT(PrintComplete(int)), Qt::UniqueConnection );
    connect( printer, SIGNAL(error(int,QString)),
             this, SLOT(PrinterError(int,QString)), Qt::UniqueConnection );
    printer->setupThread_Process(processThread);
} // end constructor

// CLOSE AND DECONSTRUCTOR
PrinterDialog::~PrinterDialog()
{
    settings.sync();
    delete ui;
}
void PrinterDialog::on_pushButton_Cancel_clicked()
{
    ui->actionExit->trigger();
}
void PrinterDialog::on_actionExit_triggered()
{
    this->close();
}

// COMMAND LINE ARGUMENT PARSE
// Load command line parsed or piped file, if available.
void PrinterDialog::runArguments(const QStringList &positionalArguments)
{
    //+ The following code is giving trouble under Linux.
    // GET FILE from Pipe into dPrinter
    // Attempt to read from standard input.
    //QFile inFile;
    //inFile.open(stdin, QIODevice::ReadOnly, QFileDevice::AutoCloseHandle);
    // Reading should be performed through a QIODevice object
    // .. to keep stdin pointer in sync and to have more control over reading.
    //QTextStream stream(&inFile);
    // All the characters in the PostScript specs can be read using UTF-8.
    //stream.setCodec("UTF-8");
    //qint64 pos = stream.pos();
    //if ( stream.readLine().startsWith("%!PS-Adobe") )
    //{
    //    stream.seek(pos);
    //    printer.consoleStreamParse( );
    //}
    // OR FROM given location. If not, ask for one
    //else
    if ( !positionalArguments.isEmpty() && positionalArguments.size()<2 )
        printer->setFileLoc( positionalArguments.first() );
    else
        ui->actionOpen->trigger();
}

// OPEN FILE
void PrinterDialog::on_actionOpen_triggered()
{
    // Get file address from user
    QString filename = QFileDialog::getOpenFileName(
                this, tr("Open PostScript File"), "~",
                tr("PostScript (*.ps);;All files (*.*)") );
    // Open file
    QFile file(filename);
    // Inform user if file fails to open
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(0,"Error",file.errorString());
        return;
    }

    // File located succesfully.
    printer->setFileLoc( filename );
    loadFile();
}

//*+ Load File data into Form.
void PrinterDialog::loadFile()
{
    /*
    // LOAD PAGES from File
    const int PAGESTOTAL = 7; // * Get this value from document.

    // Create and configure layout structure that will hold buttons
    QGridLayout * pagesLay = new QGridLayout();

    // Add a button per page.
    for (int i=0; i<PAGESTOTAL; i++)
    {
        // Create "page" button
        int zoomFactor = 10;  // * Stored elsewhere, no less than 1.
        int zoom = zoomFactor/72;
        QPushButton *pageBtn = new QPushButton("This button represents a page");
        pageBtn->setFixedWidth( //printer->getWidth())
            8.5*zoom);
        pageBtn->setFixedHeight( //printer->getHeight())
            11*zoom);
        // * make button transparent
        // * assign icon's size
        // * assign button's size
        // * assign button's icon
        // Add button to layout.
        //pageBtn->setEnabled(true);
        pageBtn->setFlat(false);
        pagesLay->addWidget(pageBtn);
    }
    ui->scrollContents->setLayout(pagesLay);
    */
}

// PRINTER IS SELECTED, load referencial printer data.
void PrinterDialog::on_comboBox_Printer_currentTextChanged(
                                                const QString &selectedPrinter )
{
    //Get current page value or value from last session.
    QString currentSize = ui->comboBox_PageSize->currentText();
    if ( settings.contains("Printer/"+selectedPrinter+"/PageSize") )
        currentSize = settings.value("Printer/"+selectedPrinter+"/PageSize").toString();
    else
        currentSize = QPageSize::name(QPageSize::Letter);

    // Populate load info on currently selected printer
    foreach ( QPrinterInfo printerInfo, printerList )
        if ( selectedPrinter == printerInfo.printerName() )
        {
            // Send printerInfo to dPrinter library for later processing
            printer->set( printerInfo );
            // Update supported page sizes
            QList<QPageSize> paperSizes =
                    printerInfo.supportedPageSizes();
            // Remove duplicates, remove "Custom" and Sort using enum's order.
            paperSizes=QList<QPageSize>(paperSizes.begin(), paperSizes.end());
            // std::sort( paperSizes.begin(), paperSizes.end() );
            // paperSizes.removeOne( QPrinter::Custom ); // Deprecation related changes to the code made this operation invalid.
            // Clear comboBox and add new sizes
            ui->comboBox_PageSize->clear();
            if ( !paperSizes.isEmpty() )
                foreach ( QPageSize paper, paperSizes )
                {
                    ui->comboBox_PageSize->addItem( paper.name()
                        /*QPageSize::name(static_cast<QPageSize::PageSizeId>(paper) )*/
                    );
                    compatPaperSizes << paper
                        /*QPageSize(static_cast<QPageSize::PageSizeId>(paper) )*/
                    ;
                }
            else
            {
                // The average printer should support these:
                QList<QString> defaultSizes;
                defaultSizes << QPageSize::name(QPageSize::A4)
                             << QPageSize::name(QPageSize::B5)
                             << QPageSize::name(QPageSize::Letter)
                             << QPageSize::name(QPageSize::Legal)
                             << QPageSize::name(QPageSize::Executive);
                ui->comboBox_PageSize->addItems( defaultSizes );
            }
        }
    // Attempt to reset last size to comboBox.
    ui->comboBox_PageSize->setCurrentText( currentSize );

    // Finally statusBar update
    ui->statusBar->showMessage( selectedPrinter+printer->state() );
}

// UI INTERACTION
//> Refry button
void PrinterDialog::on_actionAlways_refry_PostScript_triggered(bool checked)
{
    printer->setToast(checked);
}

//> SELECTION
void PrinterDialog::on_radioButton_RangeWhole_toggled(bool checked)
{
    ui->lineEdit_RangeSelection->setEnabled(!checked);
    printer->setSelection( "All" );
}
void PrinterDialog::on_radioButton_RangeSelection_toggled(bool checked)
{
    ui->lineEdit_RangeSelection->setEnabled(checked);
    ui->lineEdit_RangeSelection->setFocus();
    printer->setSelection( "" );
}
void PrinterDialog::on_lineEdit_RangeSelection_editingFinished()
{
    printer->setSelection( ui->lineEdit_RangeSelection->text() );
}
//>> INPUT VALIDATION: SELECTION
bool PrinterDialog::validRange()
{
    QString rSel = ui->lineEdit_RangeSelection->text();
    int errorPos = printer->validateRange( rSel );

    // Display any corrections that were done to the RangeSelection.
    ui->lineEdit_RangeSelection->setText( rSel );

    // If the user broke out of the for loop,
    // .. its p value will indicate where an input error is.
    if (errorPos>-1)
    {
        ui->lineEdit_RangeSelection->setFocus();
        ui->lineEdit_RangeSelection->setSelection( errorPos, 1 );
        QMessageBox::warning(this, tr("Input error"),
                             tr("There's an error in your range selection."));
    }
    // Also, if lineEdit is empty tell the user to decide whether
    else if ( ui->lineEdit_RangeSelection->text().isEmpty() )
    {
        QMessageBox::information(this, tr("Input error"),
           tr("You must either print the whole document or a range of pages."));
        ui->lineEdit_RangeSelection->setFocus();
    }
    else
        return true;
    return false;
}

//> LAYOUT
//>> Set Pages Per Side
void PrinterDialog::on_spinBox_PagesPerSide_valueChanged(int value )
{
    int newValue = printer->setPagesPerSide( value );

    // Set pagesPerSide, do PSnUp tolerance check
    if ( newValue == -1 )
        QMessageBox::information(this, tr("Error"),
        tr("Page margins are too large for current paper size.\
 Check your margin setup in Advanced Settings."));
    else if ( newValue != value )
        ui->spinBox_PagesPerSide->setValue( newValue );
}
//>> Set Multipage Layout
void PrinterDialog::on_actionLeft_to_Right_Top_to_Bottom_toggled(bool on)
{
    if ( on )
    {
        const bool horizontal_priority=true,
                   leftToRight_reading=true,
                   topToButton_reading=true;

        //ui->actionLeft_to_Right_Top_to_Bottom->setChecked(false);
        ui->actionRight_to_Left_Top_to_Bottom->setChecked(false);
        ui->actionTop_to_Bottom_Left_to_Right->setChecked(false);
        ui->actionTop_to_Bottom_Right_to_Left->setChecked(false);

        printer->setLayout( horizontal_priority,
                            leftToRight_reading, topToButton_reading );
    }
    settings.setValue("nup/layout", 0);
    //* Fix cosmetic bug: if toggled off, all seem off.
}
void PrinterDialog::on_actionRight_to_Left_Top_to_Bottom_toggled(bool on)
{
    if ( on )
    {
        const bool horizontal_priority=true,
                   leftToRight_reading=false,
                   topToButton_reading=true;

        ui->actionLeft_to_Right_Top_to_Bottom->setChecked(false);
        //-ui->actionRight_to_Left_Top_to_Bottom->setChecked(false);
        ui->actionTop_to_Bottom_Left_to_Right->setChecked(false);
        ui->actionTop_to_Bottom_Right_to_Left->setChecked(false);

        printer->setLayout( horizontal_priority,
                            leftToRight_reading, topToButton_reading );
    }
    settings.setValue("nup/layout", 1);
}
void PrinterDialog::on_actionTop_to_Bottom_Left_to_Right_toggled(bool on)
{
    if ( on )
    {
        const bool horizontal_priority=false,
                   leftToRight_reading=true,
                   topToButton_reading=true;

        ui->actionLeft_to_Right_Top_to_Bottom->setChecked(false);
        ui->actionRight_to_Left_Top_to_Bottom->setChecked(false);
        //-ui->actionTop_to_Bottom_Left_to_Right->setChecked(false);
        ui->actionTop_to_Bottom_Right_to_Left->setChecked(false);

        printer->setLayout( horizontal_priority,
                            leftToRight_reading, topToButton_reading );
    }
    settings.setValue("nup/layout", 2);
}
void PrinterDialog::on_actionTop_to_Bottom_Right_to_Left_toggled(bool on)
{
    if ( on )
    {
        const bool horizontal_priority=false,
                   leftToRight_reading=false,
                   topToButton_reading=true;

        ui->actionLeft_to_Right_Top_to_Bottom->setChecked(false);
        ui->actionRight_to_Left_Top_to_Bottom->setChecked(false);
        ui->actionTop_to_Bottom_Left_to_Right->setChecked(false);
        //-ui->actionTop_to_Bottom_Right_to_Left->setChecked(false);

        printer->setLayout( horizontal_priority,
                            leftToRight_reading, topToButton_reading );
    }
    settings.setValue("nup/layout", 3);
}

//> DUPLEX
void PrinterDialog::on_comboBox_Duplex_currentIndexChanged( int index )
{
    printer->setDuplex(index);
    if (index==0)
        ui->checkBox_Reverse->setEnabled(false);
    else
        ui->checkBox_Reverse->setEnabled(true);
}
//> REVERSE
void PrinterDialog::on_checkBox_Reverse_toggled(bool checked)
{
    printer->setReverse(checked);
}
//> SET PAGE SIZE
void PrinterDialog::on_comboBox_PageSize_currentTextChanged(
                                                       const QString &pageName )
{
    QString selectedPrinter = ui->comboBox_Printer->currentText();
    settings.setValue("Printer/"+selectedPrinter+"/PageSize", pageName);
    foreach (QPageSize pageSize, compatPaperSizes)
        if ( pageSize.name()==pageName )
        {
            QSizeF points = QPageSize::size( pageSize.id(), QPageSize::Point );
            printer->setDestinationHeight( points.height() );
            printer->setDestinationWidth( points.width() );
        }
}
//> SET COLOR PROFILE
void PrinterDialog::on_comboBox_ColorProfile_currentIndexChanged( int index )
{
    printer->setColorProfile(index);
}
//> MANAGE COLOR PROFILES
void PrinterDialog::on_pushButton_Margins_clicked()
{
}
//void PrinterDialog::on_pushButton_ManageProfiles_clicked()
//{
    //> Allow user to create and use custom ICC Color profiles
    //+ Not yet implemented.
//}
//> SET COPIES
void PrinterDialog::on_spinBox_Copies_valueChanged( int value )
{
    if ( value>1 )
        ui->checkBox_Collate->setEnabled(true);
    else
    {
        ui->checkBox_Collate->setChecked(true);
        ui->checkBox_Collate->setEnabled(false);
    }
    printer->setCopies(value);
}
//>> SET COLLATE
void PrinterDialog::on_checkBox_Collate_toggled( bool checked )
{
    printer->setCollate(checked);
}
//>> SET ENABLE ALL WIDGETS
void PrinterDialog::setEnableAllWidgets( bool set )
{
    // Action
    //-ui->actionAbout->setEnabled(set);
    ui->actionAlways_refry_PostScript->setEnabled(set);
    //-ui->actionExit->setEnabled(set);
    ui->actionE_mail_PDF->setEnabled(set);
    ui->actionLeft_to_Right_Top_to_Bottom->setEnabled(set);
    ui->actionMargins_and_Borders->setEnabled(set);
    ui->actionOpen->setEnabled(set);
    ui->actionPaths->setEnabled(set);
    ui->actionPrint->setEnabled(set);
    ui->actionRight_to_Left_Top_to_Bottom->setEnabled(set);
    ui->actionSave_As_PDF->setEnabled(set);
    ui->actionSave_As_PostScript->setEnabled(set);
    ui->actionTop_to_Bottom_Left_to_Right->setEnabled(set);
    ui->actionTop_to_Bottom_Right_to_Left->setEnabled(set);
    //-ui->actionTutorials->setEnabled(set);

    // ComboBox
    ui->comboBox_ColorProfile->setEnabled(set);
    ui->comboBox_Duplex->setEnabled(set);
    ui->comboBox_PageSize->setEnabled(set);
    ui->comboBox_Printer->setEnabled(set);

    // CheckBox
    if( ui->spinBox_Copies->value()>1 )
        ui->checkBox_Collate->setEnabled(set);
    if( ui->comboBox_Duplex->currentIndex()!=0 )
        ui->checkBox_Reverse->setEnabled(set);

    // LineEdit
    if( ui->radioButton_RangeSelection->isChecked() )
        ui->lineEdit_RangeSelection->setEnabled(set);

    // PushButtons
    ui->pushButton_Cancel->setEnabled(set);
    ui->pushButton_DuplexSetup->setEnabled(set);
    //ui->pushButton_ManageProfiles->setEnabled(set);  // Not yet implemented.
    ui->pushButton_Preview->setEnabled(set);
    ui->pushButton_Print->setEnabled(set);
    //ui->pushButton_Properties->setEnabled(set);  // Not yet implemented.
    //ui->pushButton_Tutorials->setEnabled(set);  // Not yet implemented.

    // RadioButton
    ui->radioButton_RangeSelection->setEnabled(set);
    ui->radioButton_RangeWhole->setEnabled(set);

    // SpinBox
    ui->spinBox_Copies->setEnabled(set);
    ui->spinBox_PagesPerSide->setEnabled(set);
}
//+ PRINT
void PrinterDialog::on_pushButton_Print_clicked()
{
    ui->actionPrint->trigger();
}
void PrinterDialog::on_actionPrint_triggered()
{
    // Check if current printer has duplez support
    Duplex duplex( ui->comboBox_Printer->currentText() );
    // Check for duplex setup before any print that involves duplex.
    if ( ui->comboBox_Duplex->currentIndex()==0 && !duplex.isSetup() )
    {
        QMessageBox::information(this, tr("Information"),
            tr("Manual duplex has not been setup yet for this printer.\
\nYou must configure it before you can continue."));
        ui->pushButton_DuplexSetup->click();
    }
    // SELECTION Validation and Send print.
    else
        if ( ui->radioButton_RangeWhole->isChecked() ||
             ( ui->radioButton_RangeSelection->isChecked() && validRange() ) )
        {
            setEnableAllWidgets(false);
            printer->moveToThread(processThread);
            connect( processThread, SIGNAL(finished()),
                     printer, SLOT(firstPrint()) );
            processThread->start();
        }
} // end function - on_actionPrint

void PrinterDialog::progressPlusPlus( bool reset )
{
    static int counter=0;
    if ( reset )
        counter=0;
    if ( counter<10 )
    {
        ui->progressBar->setValue( ui->progressBar->value()+1 );
        counter++;
    }
}

void PrinterDialog::ProcessProgress( int progress )
{
    const int PROCESSAMNT = 9;  //* Equivalent to PROCESS_AMNT in process func.
    if( ui->progressBar->isHidden() )
    {
        ui->progressBar->setMaximum( PROCESSAMNT*10 );
        ui->progressBar->setValue( 0 );
        ui->pushButton_Tutorials->setHidden(true);
        ui->progressBar->setHidden(false);
        ui->statusBar->showMessage(tr("Processing print..."));
        QTimer * timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), this, SLOT(progressPlusPlus()));
        timer->start(1250);
    }

    ui->progressBar->setValue(progress*10);
    progressPlusPlus( true );  // Reset Progress++ counter

    if( progress>=PROCESSAMNT )
    {
        processThread->exit();  // Launches First Print
        if ( ui->comboBox_Duplex->currentIndex()==3 )
           ui->statusBar->showMessage(tr("Process complete. Printing side B."));
        else
           ui->statusBar->showMessage(tr("Process complete. Printing side A."));
        ui->progressBar->setHidden(true);
        ui->pushButton_Tutorials->setHidden(false);
        disconnect( processThread, SIGNAL(finished()),
                    printer, 0 );
        printer->moveToThread( QThread::currentThread() );
    }
}

void PrinterDialog::PrinterError( int id, QString message )
{
    ui->progressBar->setHidden(true);
    ui->pushButton_Tutorials->setHidden(false);
    QMessageBox::critical(this, tr("Error #")+QString::number(id), message);
    printer->moveToThread( QThread::currentThread() );
    setEnableAllWidgets(true);
}

void PrinterDialog::PrintComplete( int toDo )
{
    // Print First side and send in Second.
    if( toDo==0 && ui->comboBox_Duplex->currentIndex()==0 )
    {
        //Prompt pages reinsertion.
        DuplexDialog dDialog( this, ui->comboBox_Printer->currentText() );
        dDialog.setModal(true);
        if ( dDialog.exec() )
        {
            ui->statusBar->showMessage(tr("Manual duplex. Printing side B."));
            printer->secondPrint();
        }
        else
            emit PrintComplete(1);
    }
    // Save As PDF
    else if ( toDo==2 )
    {
        ;  //* Save As PDF
        //*> And e-mail if
    }
    // Save As PS
    else if ( toDo==3 )
    {
        ;  //* Save As PostScript
    }
    // Done
    else
    {
        setEnableAllWidgets(true);
        ui->statusBar->showMessage(tr("Printing complete."));
    }
}

// ADITIONAL WINDOW FORMS
void PrinterDialog::on_pushButton_DuplexSetup_clicked()
{
    DuplexSetupDialog * setup = new DuplexSetupDialog( this, printer );
    setup->setModal(true);
    setup->exec();
}

void PrinterDialog::on_actionAbout_triggered()
{
    AboutDialog * about = new AboutDialog;
    about->show();
}

void PrinterDialog::on_actionMargins_and_Borders_triggered()
{
    MarginsAndBorders *mb = new MarginsAndBorders(this);
    mb->setModal(true);
    mb->exec();
}

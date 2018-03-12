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
#include "duplexsetupdialog.h"
#include "ui_duplexsetupdialog.h"

DuplexSetupDialog::DuplexSetupDialog(QWidget *parent, dPrinter *engine ) :
    QDialog(parent),
    ui(new Ui::DuplexSetupDialog)
{
    ui->setupUi(this);
    // -1 selection means nothing selected.
    // .. User that attempts to save before saving will get an error.
    _engine = engine;
    selection = -1;
    ui->label_PrinterName->setText( ui->label_PrinterName->text() +
                                    engine->getPrinter().printerName() );
}

DuplexSetupDialog::~DuplexSetupDialog()
{
    delete ui;
}

void DuplexSetupDialog::on_pushButton_FirstPrint_clicked()
{
    Print( "test0.ps" );
}

void DuplexSetupDialog::on_pushButton_SecondPrint_clicked()
{
    Print( "test1.ps" );
}

void DuplexSetupDialog::Print( QString file )
{
    //* Print this file
    _engine->print( QDir::currentPath()+"/"+file );
}

// Save and close
void DuplexSetupDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if ( ui->buttonBox->standardButton(button) == QDialogButtonBox::Cancel )
        this->close();
    else // Save
        if ( selection==-1 )
            QMessageBox::warning( this, tr("Error"),
                         tr("Select which image best represents your print") );
        else
        {
            set( selection );
            this->close();
        }
}

void DuplexSetupDialog::on_pushButton_result0_clicked()
{
    selection = 0;
}

void DuplexSetupDialog::on_pushButton_result1_clicked()
{
    selection = 1;
}

void DuplexSetupDialog::on_pushButton_result2_clicked()
{
    selection = 2;
}

void DuplexSetupDialog::on_pushButton_result3_clicked()
{
    selection = 3;
}

void DuplexSetupDialog::set( int Set )
{
    QString printerName = _engine->getPrinter().printerName();
    Duplex duplex( printerName );
    switch( Set )
    {
        case 0: duplex.setup( DUPLEX::NO_FLIP, DUPLEX::_none );
                break;
        case 1: duplex.setup( DUPLEX::NO_FLIP, DUPLEX::_180deg );
                break;
        case 2: duplex.setup( DUPLEX::FLIP_SHORT, DUPLEX::_none );
                break;
        case 3: duplex.setup( DUPLEX::FLIP_LONG, DUPLEX::_none );
                break;
        default: throw;
    }
}

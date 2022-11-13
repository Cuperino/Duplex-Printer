/*  Duplex Printer
* Virtual printer adds ecological features to physical printers.
* Copyright (C) 2014  Javier Oscar Cordero Pérez <<javier@imaginary.tech>
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
#include "duplexdialog.h"
#include "ui_duplexdialog.h"

#include "duplex.h"

DuplexDialog::DuplexDialog(QWidget *parent, QString Printer) :
    QDialog(parent),
    ui(new Ui::DuplexDialog)
{
    ui->setupUi(this);    

    // Get instructions for current printer.
    Duplex duplex( Printer );
    int flip = duplex.getFlip(),
        rot = duplex.getRotation();

    //* Show corresponding animation to user.
    //+ A caption is used instead for current prototype.
    switch( flip )
    {
        case 0 :
            ui->label_Caption->setText("No flip ");
            break;
        case 1 :
            ui->label_Caption->setText("Flip on the Long Side ");
            break;
        case 2 :
            ui->label_Caption->setText("Flip on the Short Side ");
            break;
        default : throw;
    }
    QString curr = ui->label_Caption->text();
    switch( rot )
    {
        case 0 :
            break;
        case 1 :
            ui->label_Caption->setText(curr+"Rotate 180 deg");
            break;
        case 2 :
            ui->label_Caption->setText(curr+"Rotate 90 deg clockwise");
            break;
        case 3 :
            ui->label_Caption->setText(curr+"Rotate 90 counterclockwise");
            break;
        default : throw;
    }
}

DuplexDialog::~DuplexDialog()
{
    delete ui;
}

void DuplexDialog::on_buttonBox_accepted()
{
    QDialog::accept();
}

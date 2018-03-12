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
#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    ui->label_ProgramName->setText(QCoreApplication::applicationName());
    ui->label_Version->setText("v. "+QCoreApplication::applicationVersion());
    ui->pushButton_Website->setText("www."+QCoreApplication::organizationDomain());
    ui->label_Contributions->setText(
"Ghostscript by Artifex Software, Inc. (C) 1988-2014\n\
GSView by Ghostgum Software Pty Ltd. (C) 1993-2012\n\
PS Utilities by Angus J. C. Duggan. (C) 1991-1995\n\
PSblank by Javier O. Cordero Pérez. (C) 2014\n" );
#if defined(WINNT) || defined(WIN32)
    ui->label_Contributions->setText( ui->label_Contributions->text()+"PrinterPlusPlus by Printer++ (C) 2012" );
#endif
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_pushButton_Ok_clicked()
{
    this->close();
}

void AboutDialog::on_pushButton_License_clicked()
{

}

void AboutDialog::on_pushButton_Website_clicked()
{

}

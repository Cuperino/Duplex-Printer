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
#include "marginsandborders.h"
#include "ui_marginsandborders.h"

MarginsAndBorders::MarginsAndBorders(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MarginsAndBorders)
{
    ui->setupUi(this);
    // Get supported units from units class
    u = new units;
    QStringList list = u->list();
    modelUnits = new QStringListModel(this);
    modelUnits->setStringList( list );
    ui->comboBoxMargin->setModel(modelUnits);
    ui->comboBoxBorder->setModel(modelUnits);

    QSettings settings;
    if ( settings.contains("nup/margin") )
        ui->doubleSpinBoxMargin->setValue( settings.value("nup/margin").toDouble() );
    else
        ui->doubleSpinBoxMargin->setValue( 0.5 );
    if ( settings.contains("nup/border") )
        ui->doubleSpinBoxBorder->setValue(
                    settings.value("nup/border").toDouble() );
    else
        ui->doubleSpinBoxBorder->setValue( 0.3 );

//    ui->doubleSpinBoxMargin->setValue( settings );
}

MarginsAndBorders::~MarginsAndBorders()
{
    delete ui;
}

void MarginsAndBorders::on_buttonBox_accepted()
{
    units unit;
    QSettings settings;

    // Get values from user input.
    double margin = ui->doubleSpinBoxMargin->value();
    double border = ui->doubleSpinBoxBorder->value();
    margin *= 72*unit.factor( ui->comboBoxMargin->currentText() );
    border *= 72*unit.factor( ui->comboBoxBorder->currentText() );

    // Save values to disk.
    settings.setValue("nup/margin", margin);
    settings.setValue("nup/border", border);
    settings.sync();

    this->close();
}

void MarginsAndBorders::on_buttonBox_rejected()
{
    this->close();
}

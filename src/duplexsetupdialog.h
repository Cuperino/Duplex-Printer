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
#ifndef DUPLEXSETUPDIALOG_H
#define DUPLEXSETUPDIALOG_H

#include <QDialog>
#include <QDir>
#include <QMessageBox>
#include "duplex.h"
#include "dprinter.h"

namespace Ui {
class DuplexSetupDialog;
}

class DuplexSetupDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DuplexSetupDialog(QWidget *parent = 0, dPrinter *engine = 0);
    ~DuplexSetupDialog();

private slots:
    void on_pushButton_FirstPrint_clicked();

    void on_pushButton_SecondPrint_clicked();

    void on_buttonBox_clicked(QAbstractButton *button);

    void on_pushButton_result0_clicked();

    void on_pushButton_result1_clicked();

    void on_pushButton_result2_clicked();

    void on_pushButton_result3_clicked();

private:
    // Objects
    Ui::DuplexSetupDialog *ui;
    dPrinter *_engine;
    int selection;
    // Functions
    void Print( QString );
    void set( int );
};

#endif // DUPLEXSETUPDIALOG_H

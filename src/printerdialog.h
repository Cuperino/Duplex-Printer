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

#ifndef PRINTERDIALOG_H
#define PRINTERDIALOG_H

// UI Interaction
#include <QMainWindow>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
// Multiplatform support for information on printers.
#include <QtPrintSupport/QPrinterInfo>
#include <QPageSize>
#include <QList>
// Unstuck the progress bar
#include <QTimer>
// dPrinter does the work requested through UI
#include "dprinter.h"

namespace Ui {
class PrinterDialog;
}

class PrinterDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit PrinterDialog(QWidget *parent = 0);
    ~PrinterDialog();
    void runArguments(const QStringList &positionalArguments);

private slots:

    void on_pushButton_Print_clicked();

    void on_actionPrint_triggered();

    void on_pushButton_Cancel_clicked();

    void on_actionExit_triggered();

    void on_actionOpen_triggered();

    void on_comboBox_Printer_currentTextChanged(const QString &arg1);

    void on_radioButton_RangeWhole_toggled(bool checked);

    void on_radioButton_RangeSelection_toggled(bool checked);

    void on_lineEdit_RangeSelection_editingFinished();

    void on_spinBox_PagesPerSide_valueChanged(int arg1);

    void on_actionLeft_to_Right_Top_to_Bottom_toggled(bool on);

    void on_actionRight_to_Left_Top_to_Bottom_toggled(bool on);

    void on_actionTop_to_Bottom_Left_to_Right_toggled(bool on);

    void on_actionTop_to_Bottom_Right_to_Left_toggled(bool on);

    void on_comboBox_Duplex_currentIndexChanged(int index);

    void on_comboBox_PageSize_currentTextChanged(const QString &pageName);

    void on_comboBox_ColorProfile_currentIndexChanged(int index);

    void on_spinBox_Copies_valueChanged(int arg1);

    void on_checkBox_Collate_toggled(bool checked);

    void on_checkBox_Reverse_toggled(bool checked);

    void on_actionAlways_refry_PostScript_triggered(bool checked);

    void on_pushButton_Margins_clicked();

    // Handmade
    void progressPlusPlus( bool reset=false );
    void ProcessProgress( int progress );
    void PrintComplete(int toDo );
    void PrinterError( int id, QString message );

    void on_actionAbout_triggered();

    void on_pushButton_DuplexSetup_clicked();

    void on_actionMargins_and_Borders_triggered();

private:
    // Objects
    Ui::PrinterDialog *ui;
    QSettings settings;
    QThread *processThread;
        QList<QPrinterInfo> printerList;
    QList<QPageSize> compatPaperSizes;
    dPrinter * printer;
    // Functions
    void loadFile();
    bool validRange();
    void setEnableAllWidgets(bool);
};

#endif // PRINTERDIALOG_H

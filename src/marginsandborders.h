/*  Duplex Printer
* Virtual printer adds ecological features to physical printers.
* Copyright (C) 2014  Javier Oscar Cordero Pérez <javier@imaginary.tech>
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
#ifndef MARGINSANDBORDERS_H
#define MARGINSANDBORDERS_H

#include <QDialog>
#include <QSettings>
#include <QStringListModel>
#include "units.h"

namespace Ui {
class MarginsAndBorders;
}

class MarginsAndBorders : public QDialog
{
    Q_OBJECT

public:
    explicit MarginsAndBorders(QWidget *parent = 0);
    ~MarginsAndBorders();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::MarginsAndBorders *ui;
    Units *u;
    QStringListModel *modelUnits;
};

#endif // MARGINSANDBORDERS_H

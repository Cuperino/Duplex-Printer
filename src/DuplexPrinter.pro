#------------------------------------------------------------------------------
#
#  Duplex Printer
#  A virtual printer emulates various ecological features on physical printers.
#  Copyright (C) 2014  Javier Oscar Cordero Pérez <javier.cordero@upr.edu>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#------------------------------------------------------------------------------

#-------------------------------------------------
#
# Project created by QtCreator 2014-07-12T00:34:05
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DuplexPrinter
TEMPLATE = app


SOURCES += main.cpp\
        printerdialog.cpp \
    dprinter.cpp \
    duplex.cpp \
    duplexdialog.cpp \
    aboutdialog.cpp \
    duplexsetupdialog.cpp \
    marginsandborders.cpp \
    units.cpp

HEADERS  += printerdialog.h \
    dprinter.h \
    duplex.h \
    duplexdialog.h \
    aboutdialog.h \
    duplexsetupdialog.h \
    marginsandborders.h \
    units.h

FORMS    += printerdialog.ui \
    duplexdialog.ui \
    aboutdialog.ui \
    duplexsetupdialog.ui \
    marginsandborders.ui

OTHER_FILES +=

RESOURCES += \
    dpResources.qrc

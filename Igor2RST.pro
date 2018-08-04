#-------------------------------------------------
#
# Project created by QtCreator 2018-07-11T09:14:33
#
#-------------------------------------------------

QT       += core gui serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = UserTRX
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        MainWindow.cpp \
    Tab5Tone.cpp \
    MemoryTableModel.cpp \
    Tab2Tone.cpp \
    MemoryDelegate.cpp \
    CheckBox.cpp \
    RadioDevice.cpp \
    LineEdit.cpp \
    Log.cpp \
    CheckConnnectionToRadio.cpp

HEADERS += \
        MainWindow.h \
    Tab5Tone.h \
    MemoryTableModel.h \
    Tab2Tone.h \
    MemoryDelegate.h \
    CheckBox.h \
    RadioDevice.h \
    LineEdit.h \
    Log.h \
    CheckConnnectionToRadio.h

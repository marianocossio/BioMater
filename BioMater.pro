#-------------------------------------------------
#
# Project created by QtCreator 2017-10-11T10:25:28
#
#-------------------------------------------------

QT       += core gui serialport multimedia opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BioMater
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp\
        acquisitionserver.cpp\
        dataconditioner.cpp\
        dataset.cpp\
        graph.cpp\
        serverthread.cpp \
        system.cpp \
        audioplayer.cpp \
        filter.cpp \
    bandpass.cpp

HEADERS  += mainwindow.h\
            acquisitionserver.h\
            dataconditioner.h\
            dataset.h\
            graph.h\
            serverthread.h \
            system.h \
            audioplayer.h \
            filter.h \
    bandpass.h

FORMS    += mainwindow.ui

RESOURCES += \
    images.qrc

win32 {
    LIBS += -lOpengl32
}

RC_ICONS = Images/icon.ico

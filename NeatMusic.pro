#-------------------------------------------------
#
# Project created by QtCreator 2018-05-04T20:04:06
#
#-------------------------------------------------
CONFIG +=  c++11
QT += core gui sql concurrent
QT += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NeatMusic
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
    mainwindow.cpp \
    musicview.cpp \
    musicviewdelegrate.cpp \
    listviewitemdate.cpp \
    listviewitemmodel.cpp \
    musicinfodata.cpp \
    musiclistmodel.cpp \
    musicinfolistdelegrate.cpp \
    musicinfoview.cpp \
    musicdirdlg.cpp \
    toast.cpp \
    playmusicshow.cpp \
    albumshow.cpp \
    bar.cpp \
    musicdbmanager.cpp \
    favourite.cpp \
    favouritedelegrate.cpp
HEADERS += \
        mainwindow.h \
    musicview.h \
    musicviewdelegrate.h \
    listviewitemdate.h \
    listviewitemmodel.h \
    musicinfodata.h \
    musiclistmodel.h \
    musicinfolistdelegrate.h \
    musicinfoview.h \
    musicdirdlg.h \
    toast.h \
    playmusicshow.h \
    filter.h \
    albumshow.h \
    bar.h \
    musicdbmanager.h \
    favourite.h \
    favouritedelegrate.h
FORMS += \
        mainwindow.ui \
    musicdirdlg.ui \
    favourite.ui

RESOURCES += \
    rescources.qrc \
    fonts.qrc \
    stylesheets.qrc
LIBS += -LE:/ffmpeg/lib/
LIBS +=-lavcodec -lavformat -lswscale -lavutil
INCLUDEPATH += E:/ffmpeg/include
DEPENDPATH += E:/ffmpeg/include
LIBS+= -L$$PWD/pinyin -lpinyin_win
INCLUDEPATH+=$$PWD/pinyin
DEPENDPATH+=$$PWD/pinyin

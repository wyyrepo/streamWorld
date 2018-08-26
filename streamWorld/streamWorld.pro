#-------------------------------------------------
#
# Project created by QtCreator 2016-04-22T13:37:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = streamWorld
TEMPLATE = app

INCLUDEPATH += D:\sdk\include        #这个是LibVLC的头文件包含目录，请自行修改。

LIBS += D:\sdk\lib\libvlc.lib        #这两行是LibVLC的库文件路径，请自行修改。
LIBS += D:\sdk\lib\libvlccore.lib

SOURCES += main.cpp\
        videodialog.cpp \
    radiodialog.cpp \
    system.cpp \
    fulldialog.cpp \
    setdialog.cpp \
    listdialog.cpp \
    Model/stationgroup.cpp \
    Model/stationlist.cpp \
    Model/station.cpp \
    Tools/textio.cpp \
    widget/adddialog.cpp \
    widget/systemtray.cpp \
    widget/aboutdialog.cpp \
    vlc.cpp

HEADERS  += videodialog.h \
    radiodialog.h \
    system.h \
    fulldialog.h \
    setdialog.h \
    listdialog.h \
    Model/stationgroup.h \
    Model/stationlist.h \
    Model/station.h \
    Tools/textio.h \
    widget/adddialog.h \
    widget/systemtray.h \
    widget/aboutdialog.h \
    vlc.h

FORMS    += videodialog.ui \
    radiodialog.ui \
    fulldialog.ui \
    setdialog.ui \
    listdialog.ui \
    widget/adddialog.ui \
    widget/aboutdialog.ui

RESOURCES += \
    radio.qrc \
    list.qrc \
    settings.qrc \
    video.qrc

RC_FILE += myico.rc

#-------------------------------------------------
#
# Project created by QtCreator 2024-10-10T12:05:08
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = xUtils
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

CONFIG +=   c++17\
            depend_includepath
QMAKE_CXXFLAGS += -std=c++17 -lstdc++fs -lX11 -lXrandr -L/usr/bin/X11

debug {
    QMAKE_CXXFLAGS += -DDEBUG
}


SOURCES =  $$files(src/lib/*.cpp) \
             $$files(src/qt/*.cpp) \

#SOURCES +=  src/main.cpp \
#            src/mainwindow.cpp \
#            src/xdrivers.cpp
INCLUDEPATH += ./include/lib \
               ./include/qt

HEADERS +=  $$files(include/lib/*) \
            $$files(include/qt/*) \



FORMS +=    ui/mainwindow.ui \
            ui/monitorbutton.ui \
            ui/confirm.ui \
            ui/driverdialog.ui \

LIBS += -lstdc++fs -L/usr/bin/X11 -lX11 -lXrandr

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

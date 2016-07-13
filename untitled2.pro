#-------------------------------------------------
#
# Project created by QtCreator 2016-07-11T10:24:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    kdtree.cpp

HEADERS  += mainwindow.h \
    kdtree.h \
    allocator.hpp \
    function.hpp \
    iterator.hpp \
    kdtree.hpp \
    node.hpp \
    region.hpp

FORMS    += mainwindow.ui

QMAKE_CXXFLAGS = -fpermissive

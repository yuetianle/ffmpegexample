#-------------------------------------------------
#
# Project created by QtCreator 2015-01-09T10:17:34
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = ffmpegexample
CONFIG   += console
CONFIG   += c++11
CONFIG   -= app_bundle

TEMPLATE = app
LIBS += -L /usr/local/lib

LIBS += /usr/local/lib/libavcodec.so
LIBS += /usr/local/lib/libavutil.so
LIBS += /usr/local/lib/libavformat.so
LIBS += /usr/local/lib/libswscale.so
LIBS += /usr/local/lib/libSDL2.so


SOURCES += main.cpp \
    qvideo.cpp \
    vqthread.cpp \
    audio.cpp

HEADERS += \
    qvideo.h \
    vqthread.h \
    audio.h

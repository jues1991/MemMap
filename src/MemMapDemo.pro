QT += core
QT -= gui

CONFIG += c++11

TARGET = MemMapDemo
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    memmap.cpp

HEADERS += \
    memmap.h

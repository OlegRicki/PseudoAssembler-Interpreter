QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += tst_test.cpp \
    ../PseudoASMInterpreter/Functions.cpp \

HEADERS += ../PseudoASMInterpreter/Functions.h

#-------------------------------------------------
#
# Project created by QtCreator 2017-02-01T20:08:55
#
#-------------------------------------------------
TARGET = NeuralNetworkView
TEMPLATE = lib
QT += widgets

SOURCES += \
    neuralnetworkview.cpp \
    mainform.cpp

HEADERS += \
    ../../System/MASS/interfaces.h \
    neuralnetworkview.h \
    mainform.h

include(../../System/MASS/BuildInfo.pri)

DISTFILES += \
    PluginMeta.json

FORMS += \
    mainform.ui

#-------------------------------------------------
#
# CRITERIA3D
# interpolation library
#
#-------------------------------------------------

QT   -= gui core

TEMPLATE = lib
CONFIG += staticlib

CONFIG += debug_and_release

unix:{
    CONFIG(debug, debug|release) {
        TARGET = debug/interpolation
    } else {
        TARGET = release/interpolation
    }
}
win32:{
    TARGET = interpolation
}

INCLUDEPATH += ../crit3dDate ../mathFunctions ../gis ../meteo

SOURCES += interpolation.cpp \
    interpolationSettings.cpp \
    interpolationPoint.cpp \
    kriging.cpp \
    spatialControl.cpp

HEADERS += interpolation.h \
    interpolationSettings.h \
    interpolationPoint.h \
    kriging.h \
    interpolationConstants.h \
    spatialControl.h


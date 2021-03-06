#-----------------------------------------------------
#
#   PRAGA GIS
#
#   this file is part of CRITERIA3D distribution
#
#-----------------------------------------------------

QT       += core gui network widgets

TARGET = PRAGA_GIS
TEMPLATE = app

INCLUDEPATH +=  ../PRAGA/shared ../mapGraphics  \
                ../crit3dDate ../mathFunctions \
                ../gis  \

CONFIG += debug_and_release

unix:{
    LIBS += -L../mapGraphics/release -lMapGraphics
}
win32:{
    CONFIG(debug, debug|release) {
         LIBS += -L../mapGraphics/debug -lMapGraphics
    } else {
        LIBS += -L../mapGraphics/release -lMapGraphics
    }
}

CONFIG(debug, debug|release) {
    LIBS += -L../crit3dDate/debug -lcrit3dDate
    LIBS += -L../mathFunctions/debug -lmathFunctions
    LIBS += -L../gis/debug -lgis

} else {

    LIBS += -L../crit3dDate/release -lcrit3dDate
    LIBS += -L../mathFunctions/release -lmathFunctions
    LIBS += -L../gis/release -lgis
}


SOURCES += main.cpp\
    ../PRAGA/shared/rasterObject.cpp \
    ../PRAGA/shared/colorlegend.cpp \
    mainWindow.cpp \
    gisProject.cpp


HEADERS += \
    ../PRAGA/shared/rasterObject.h \
    ../PRAGA/shared/colorlegend.h \
    mainWindow.h \
    gisProject.h


FORMS += mainWindow.ui \
    ../PRAGA/shared/formInfo.ui


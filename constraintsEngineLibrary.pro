#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T16:36:37
#
#-------------------------------------------------

# ensure one "debug_and_release" in CONFIG, for clarity...
debug_and_release {
    CONFIG -= debug_and_release
    CONFIG += debug_and_release
}
    # ensure one "debug" or "release" in CONFIG so they can be used as
    #   conditionals instead of writing "CONFIG(debug, debug|release)"...
CONFIG(debug, debug|release) {
    CONFIG -= debug release
    CONFIG += debug
}
CONFIG(release, debug|release) {
    CONFIG -= debug release
    CONFIG += release
}

QT       -= gui

TARGET = constraintsEngineLibrary
TEMPLATE = lib

DEFINES += CONSTRAINTSENGINELIBRARY_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

debug {
    QMAKE_POST_LINK=X:\constraintsEngine\constraintEngine\setDLL.bat $$shell_path($$OUT_PWD/debug) debug
    INCLUDEPATH += X:\fluidicMachineModel\dll_debug\include

    LIBS += -L$$quote(X:\fluidicMachineModel\dll_debug\bin) -lFluidicMachineModel
}

!debug {
    QMAKE_POST_LINK=X:\constraintsEngine\constraintEngine\setDLL.bat $$shell_path($$OUT_PWD/release) release
    INCLUDEPATH += X:\fluidicMachineModel\dll_release\include

    LIBS += -L$$quote(X:\fluidicMachineModel\dll_release\bin) -lFluidicMachineModel
}

INCLUDEPATH += X:\swipl\include
LIBS += -L$$quote(X:\swipl\bin) -llibswipl
LIBS += -L$$quote(X:\swipl\lib) -llibswipl

HEADERS += \
    constraintengine/constraintsenginelibrary_global.h \
    constraintengine/prologexecutor.h \
    constraintengine/prologtranslationstack.h

SOURCES += \
    constraintengine/prologexecutor.cpp \
    constraintengine/prologtranslationstack.cpp


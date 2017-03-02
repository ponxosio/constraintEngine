#-------------------------------------------------
#
# Project created by QtCreator 2017-03-02T16:36:37
#
#-------------------------------------------------

QT       -= gui

TARGET = constraintsEngineLibrary
TEMPLATE = lib

DEFINES += CONSTRAINTSENGINELIBRARY_LIBRARY

SOURCES += \
    prologexecutor.cpp \
    prologtranslationstack.cpp

HEADERS +=\
        constraintsenginelibrary_global.h \
    prologexecutor.h \
    prologtranslationstack.h

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


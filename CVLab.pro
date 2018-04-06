#-------------------------------------------------
#
# Project created by QtCreator 2018-03-25T08:35:34
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CVLab
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


SOURCES += \
        main.cpp \
        View.cpp \
    imageProcesser.cpp \
    foo.cpp

HEADERS += \
        View.h \
    qssloader.h \
    imageProcesser.h \
    foo.h

FORMS += \
        view.ui

RESOURCES += \
    res.qrc

INCLUDEPATH += \
    D:\opencvlib32withExtra\install\include\
    D:\opencvlib32withExtra\install\include\opencv \
    D:\opencvlib32withExtra\install\include\opencv2

LIBS += \
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_highgui310.dll\
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_features2d310.dll\
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_imgproc310.dll\
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_xfeatures2d310.dll\
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_imgproc310.dll\
    D:\opencvlib32withExtra\install\x86\mingw\bin\libopencv_imgcodecs310.dll


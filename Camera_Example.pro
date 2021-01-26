#-------------------------------------------------
#
# Project created by QtCreator 2017-09-14T11:13:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Camera_Example
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
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui



#Path for QT
#MSCV 2017 32 :            D:\Work\program\Qt5.14.2\5.14.2\msvc2017\bin
#MINGW 32     :            D:\Work\program\Qt5.14.2\5.14.2\mingw73_32\bin
#Path for opencv
#4.1 MSCV 2017 32 bit :   C:\OpenCV-4.1\msvc_2017_release\install\x86\vc15\bin
#3.2 MINGW32 :            C:\OpenCV-3.2.0\release\bin
#2.4.11 MINGW 32 :        C:\OpenCV-2.4.11\release\bin




#LIBS += -LC:\OpenCV-4.1\msvc_2017_release\install\x86\vc15\lib -lopencv_core410 -lopencv_imgproc410 -lopencv_highgui410 -lopencv_imgcodecs410 -lopencv_videoio410 -lopencv_video410 -lopencv_calib3d410 -lopencv_photo410 -lopencv_features2d410
#INCLUDEPATH += C:\OpenCV-4.1\msvc_2017_release\install\include
#DEPENDPATH += C:\OpenCV-4.1\msvc_2017_release\install\include


#INCLUDEPATH += C:\OpenCV-2.4.11\release\install\include
#LIBS += C:\OpenCV-2.4.11\release\bin\libopencv_core2411.dll
#LIBS += C:\OpenCV-2.4.11\release\bin\libopencv_highgui2411.dll
#LIBS += C:\OpenCV-2.4.11\release\bin\libopencv_imgproc2411.dll
#LIBS += C:\OpenCV-2.4.11\release\bin\libopencv_calib3d2411.dll


INCLUDEPATH += C:\opencv-3.2.0\release\install\include
LIBS += C:\opencv-3.2.0\release\bin\libopencv_core320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_highgui320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_imgproc320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_calib3d320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_imgcodecs320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_objdetect320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_videoio320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_ml320.dll
LIBS += C:\opencv-3.2.0\release\bin\libopencv_stitching320.dll

#INCLUDEPATH += /usr/local/include/opencv
#LIBS += -L/usr/local/lib -lopencv_objdetect   -lopencv_highgui  -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

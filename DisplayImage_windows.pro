#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T07:52:20
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DisplayImage
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicscene.cpp \
    graphicwidget.cpp \
    convertimage.cpp \
    netforward.cpp \
    yolo_output.cpp \
    opctionswindow.cpp\
    yolo_v2_output.cpp \
    classesdialog.cpp \
    socketgraphicsitem.cpp \
    graphicscene.cpp \
    include/proto/data.pb.cc 

HEADERS  += mainwindow.h \
    graphicscene.h \
    graphicwidget.h \
    convertimage.h \
    functor.h \
    netforward.h \
    yolo_output.h \
    opctionswindow.h\
    yolo_v2_output.h \
    classesdialog.h \
    socketgraphicsitem.h \
    graphicscene.h \
    include/proto/data.pb.h 


FORMS += mainwindow.ui \
    graphicwidget.ui \
    opctionswindow.ui \
    classesdialog.ui

RESOURCES += \
    buttonpicture.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opencv_3.1.0/lib/ -lopencv_world310
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opencv_3.1.0/lib/ -lopencv_world310d

INCLUDEPATH += $$PWD/opencv_3.1.0/include
DEPENDPATH += $$PWD/opencv_3.1.0/include

#QMAKE_CXXFLAGS += -D USE_CAFFE_ -D CPU_ONLY

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -llibprotobuf
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -llibprotobufd

INCLUDEPATH += $$PWD/lib/release
DEPENDPATH += $$PWD/lib/release

INCLUDEPATH += $$PWD/include

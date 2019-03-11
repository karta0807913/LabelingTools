QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DisplayImage
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    graphicwidget.cpp \
    convertimage.cpp \
    netforward.cpp \
    yolo_output.cpp \
    opctionswindow.cpp\
    yolo_v2_output.cpp \
    classesdialog.cpp \
    socketgraphicsitem.cpp \
    graphicscene.cpp \
    proto2/data.pb.cc

HEADERS  += mainwindow.h \
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
    proto2/data.pb.h

FORMS    += mainwindow.ui \
    graphicwidget.ui \
    opctionswindow.ui \
    classesdialog.ui

HEADERS += \
    netforward.h

RESOURCES += \
    buttonpicture.qrc

LIBS += $${PWD}/include/opencv/x64/vc14/bin/opencv_ffmpeg401_64.dll
LIBS += $${PWD}/include/opencv/x64/vc14/bin/opencv_world401.dll


LIBS += -L$${PWD}/include/protobuf/lib -lprotobuf -lprotobuf-lite -lprotoc

INCLUDEPATH += $${PWD}/include/opencv/include
INCLUDEPATH += $${PWD}/include/protobuf/include

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

CAFFE_HOME = $$(CAFFE_HOME)
HEADERS += \
    netforward.h

RESOURCES += \
    buttonpicture.qrc

# caffe
INCLUDEPATH +=$${CAFFE_HOME}/build/include \
              $${CAFFE_HOME}/include \
              $${CAFFE_HOME}/src
LIBS += -L$${CAFFE_HOME}/build/lib -lcaffe

# cuda
INCLUDEPATH += \
    /usr/local/cuda/include
LIBS += -L/usr/local/cuda/lib64 -lcudart -lcublas -lcurand

#open cv
LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui  -lopencv_gpu

LIBS += -lprotobuf -lglog -lboost_system -lgflags -lboost_filesystem

QMAKE_CXXFLAGS += --std=c++11 \
                  -D USE_CAFFE_

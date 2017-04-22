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
    include/proto/data.pb.cc \
    include/google/protobuf/stubs/common.cc\
    include/google/protobuf/stubs/once.cc\
    include/google/protobuf/stubs/atomicops_internals_x86_msvc.cc\
    include/google/protobuf/io/coded_stream.cc\
    include/google/protobuf/message_lite.cc\
    include/google/protobuf/descriptor.cc\
    include/google/protobuf/generated_message_reflection.cc\
    include/google/protobuf/reflection_ops.cc\
    include/google/protobuf/wire_format.cc\
    include/google/protobuf/repeated_field.cc\
    include/google/protobuf/generated_message_util.cc\
    include/google/protobuf/message.cc\
    include/google/protobuf/unknown_field_set.cc\
    include/google/protobuf/wire_format_lite.cc\
    include/google/protobuf/descriptor.pb.cc\
    include/google/protobuf/arena.cc\
    include/google/protobuf/arenastring.cc\
    include/google/protobuf/extension_set.cc\
    include/google/protobuf/io/strtod.cc

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
    include/proto/data.pb.h \
    include/google/protobuf/stubs/common.h\
    include/google/protobuf/stubs/port.h\
    include/google/protobuf/stubs/macros.h\
    include/google/protobuf/stubs/platform_macros.h\
    include/google/protobuf/stubs/logging.h\
    include/google/protobuf/stubs/scoped_ptr.h\
    include/google/protobuf/stubs/mutex.h\
    include/google/protobuf/stubs/callback.h\
    include/google/protobuf/stubs/type_traits.h\
    include/google/protobuf/stubs/template_util.h\
    include/google/protobuf/stubs/once.h\
    include/google/protobuf/stubs/atomicops.h\
    include/google/protobuf/stubs/atomicops_internals_x86_msvc.h\
    include/google/protobuf/stubs/shared_ptr.h\
    include/google/protobuf/stubs/casts.h\
    include/google/protobuf/stubs/atomic_sequence_num.h\
    include/google/protobuf/stubs/fastmem.h\
    include/google/protobuf/io/coded_stream.h\
    include/google/protobuf/wire_format_lite_inl.h\
    include/google/protobuf/message_lite.h\
    include/google/protobuf/descriptor.h\
    include/google/protobuf/generated_message_reflection.h\
    include/google/protobuf/reflection_ops.h\
    include/google/protobuf/wire_format.h\
    include/google/protobuf/repeated_field.h\
    include/google/protobuf/generated_enum_reflection.h\
    include/google/protobuf/generated_message_util.h\
    include/google/protobuf/message.h\
    include/google/protobuf/metadata.h\
    include/google/protobuf/unknown_field_set.h\
    include/google/protobuf/wire_format_lite.h\
    include/google/protobuf/descriptor.pb.h\
    include/google/protobuf/arena.h\
    include/google/protobuf/has_bits.h\
    include/google/protobuf/arenastring.h\
    include/google/protobuf/extension_set.h\
    include/google/protobuf/io/strtod.h


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

cmake_minimum_required(VERSION 3.1.0)

project(LabelingTools)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

find_package(QT5 REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(QT5Network REQUIRED)

find_package(OpenCV REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Tensorflow REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCE_FILE
  main.cpp
  mainwindow.cpp
  graphicscene.cpp
  graphicwidget.cpp
  convertimage.cpp
  netforward.cpp
  yolo_output.cpp
  opctionswindow.cpp
  yolo_v2_output.cpp
  classesdialog.cpp
  socketgraphicsitem.cpp
  proto2/data.pb.cc
  )

set(INCLUDE_FILE
  mainwindow.h
  graphicscene.h
  graphicwidget.h
  convertimage.h
  functor.h
  netforward.h
  yolo_output.h
  opctionswindow.h
  yolo_v2_output.h
  classesdialog.h
  socketgraphicsitem.h
  proto2/data.pb.h
  )

set(QT_FORMS
  mainwindow.ui
  graphicwidget.ui
  opctionswindow.ui
  classesdialog.ui
  )

  set(QT_RESOURCES
  buttonpicture.qrc
  )

message("PROTOBUF INCLUDES ${Protobuf_INCLUDE_DIRS}")
message("PROTOBUF LIBS ${Protobuf_LIBRARIES}")
message("opencv INCLUDES ${OpenCV_INCLUDE_DIRS}")
message("opencv LIBS ${OpenCV_LIBS}")
message("QT LIBS ${QT_LIBS}")
message("QT Weidgets LIBS ${Qt5Network_LIBRARIES}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Protobuf_INCLUDE_DIRS})

qt5_add_resources(RESOURCES ${QT_RESOURCES})

add_executable(LabelingTools ${SOURCE_FILE} ${INCLUDE_FILE} ${QT_FORMS} ${RESOURCES})
target_link_libraries(LabelingTools ${Protobuf_LIBRARIES} ${OpenCV_LIBS}  Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Network)

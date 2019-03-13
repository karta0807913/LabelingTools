cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)

project(LabelingTools)

include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

find_package(Protobuf REQUIRED)
find_package(OpenCV REQUIRED)

find_package(Qt REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(Qt5Network REQUIRED)


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

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto2/data.proto)
message("${PROTO_SRCS}")
message("${PROTO_HDRS}")
message("PROTOBUF INCLUDES ${PROTOBUF_INCLUDE_DIR}")
message("PROTOBUF LIBS ${PROTOBUF_LIBRARIES}")
message("opencv INCLUDES ${OpenCV_INCLUDE_DIRS}")
message("opencv LIBS ${OpenCV_LIBS}")
message("QT LIBS ${Qt_LIBS}")
message("QT Weidgets LIBS ${Qt5Core_LIBRARIES}")
message("${CMAKE_CURRENT_BINARY_DIR}")

include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROTOBUF_INCLUDE_DIR})

qt5_add_resources(RESOURCES ${QT_RESOURCES})

add_executable(LabelingTools ${SOURCE_FILE} ${INCLUDE_FILE} ${QT_FORMS} ${RESOURCES} ${PROTO_HDRS} ${PROTO_SRCS})
target_link_libraries(LabelingTools ${PROTOBUF_LIBRARIES} ${OpenCV_LIBS}  Qt5::Core Qt5::Widgets Qt5::Xml Qt5::Network)

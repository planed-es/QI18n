cmake_minimum_required(VERSION 3.14)

project(QI18n LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_library(QI18n SHARED
  QI18n_global.h
  qi18n.cpp
  qi18n.h
)

target_link_libraries(QI18n PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_compile_definitions(QI18n PRIVATE QI18N_LIBRARY)

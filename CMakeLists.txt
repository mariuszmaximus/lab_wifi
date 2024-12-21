cmake_minimum_required(VERSION 3.16)

project(lab_wifi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

add_library(lab_wifi

 # main.cpp

  WifiScanner.cpp
  WifiScanner.h

  # ConsoleHandler.cpp
  # ConsoleHandler.h

  WifiNetworkModel.h
  WifiNetworkModel.cpp  

)
target_link_libraries(lab_wifi Qt${QT_VERSION_MAJOR}::Core  Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(lab_wifi PUBLIC ./)

include(GNUInstallDirs)
install(TARGETS lab_wifi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(exampleWiFi)

add_executable(wpa_cli wpa_cli.cpp)

cmake_minimum_required(VERSION 3.16)

project(lab_wifi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(lab_wifi

 main.cpp

  WiFi/WifiScanner.cpp
  WiFi/WifiScanner.h

)
target_link_libraries(lab_wifi Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS lab_wifi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

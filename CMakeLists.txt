cmake_minimum_required(VERSION 3.16)

project(lab_wifi LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

add_library(lab_wifi
  WiFi/WifiScanner.cpp
  WiFi/WifiScanner.h

  WiFi/MVC/WifiNetworkModel.h
  WiFi/MVC/WifiNetworkModel.cpp  

  WiFi/UI/WifiDelegate.cpp
  WiFi/UI/WifiDelegate.h

  WiFi/UI/WiFiSwitch.cpp
  WiFi/UI/WiFiSwitch.h
  
  WiFi/UI/WiFiSlider.cpp
  WiFi/UI/WiFiSlider.h

  WiFi/UI/KeyboardBlocker.cpp
  WiFi/UI/KeyboardBlocker.h

  WiFi/UI/WiFiConfigDialog.cpp
  WiFi/UI/WiFiConfigDialog.h
)

target_link_libraries(lab_wifi Qt${QT_VERSION_MAJOR}::Core  Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(lab_wifi PUBLIC ./)

include(GNUInstallDirs)
install(TARGETS lab_wifi
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(example)

if(WIN32)

    # Pobranie folderu wyjściowego dla app1
    get_target_property(APP_OUTPUT_DIR exampleWiFi RUNTIME_OUTPUT_DIRECTORY)
    message("UUUUUUUUUUUUUUUUUUUUUUUU ${APP_OUTPUT_DIR}")

    add_executable(wpa_cli wpa_cli.cpp)
    # Sprawdzenie, czy ścieżka została poprawnie pobrana
    if(APP_OUTPUT_DIR)
        # Ustawienie folderu wyjściowego dla app2 na taki sam jak dla app1
        set_target_properties(wpa_cli PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${APP_OUTPUT_DIR}"
        )
    else()
        message(FATAL_ERROR "Could not retrieve RUNTIME_OUTPUT_DIRECTORY for app1")
    endif()

endif()
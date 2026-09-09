cmake_minimum_required(VERSION 3.21)

project(ProductPhotoAutomation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.11.2/msvc2022_64")

find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Widgets
        SerialPort
)

qt_standard_project_setup()

qt_add_executable(
    ProductPhotoAutomation

    src/main.cpp

    src/MainWindow.cpp
    src/MainWindow.h

    src/ScaleManager.cpp
    src/ScaleManager.h

    src/FileManager.cpp
    src/FileManager.h

    src/CameraManager.cpp
    src/CameraManager.h

    resources.qrc
    app.rc
)

target_link_libraries(
    ProductPhotoAutomation
    PRIVATE
        Qt6::Widgets
        Qt6::SerialPort
)
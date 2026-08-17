cmake_minimum_required(VERSION 3.10)

project("WellTool" VERSION 1.0.1 LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set( STYLES
	resources.qrc
)

set( SOURCES
	src/main.cpp
	src/mainPanel.cpp
	src/drawingCanvas.cpp
	src/calculator.cpp
	src/notesWidget.cpp
	src/screenDefinition.cpp
	src/themeLoader.cpp
	src/saveTheme.cpp

)
set( HEADERS
	include/mainPanel.h
	include/drawingCanvas.h
	include/calculator.h
	include/notesWidget.h
	include/screenDefinition.h
	include/themeLoader.h
	include/saveTheme.h
)

add_executable("WellTool" WIN32 ${SOURCES} ${HEADERS} ${STYLES})
include_directories(include)
target_link_libraries("WellTool" Qt6::Widgets)

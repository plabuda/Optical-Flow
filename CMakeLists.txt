project(Optical-Flow)
cmake_minimum_required(VERSION 2.8)

find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( ${CMAKE_SOURCE_DIR}/images )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
file(COPY "${CMAKE_SOURCE_DIR}/banan_ffmpeg.avi" DESTINATION ${CMAKE_BINARY_DIR})

set(SOURCES
    main.cpp
    BGS.cpp
    mMinWindow.cpp
    Vehicle.cpp
    stdafx.h
)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})

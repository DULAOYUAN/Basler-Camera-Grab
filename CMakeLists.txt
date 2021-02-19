cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

set(project_name TestWindow)
set(CMAKE_CXX_STANDARD 11)
project(${project_name})

# find_package(Qt5 REQUIRED Widgets)
# set(OpenCV_DIR /usr/local/opencv3.4.0/share/OpenCV)

set(OpenCV_DIR /opt/opencv-3.4.0/include/release)

find_package(OpenCV REQUIRED)
set(PYLON_ROOT "/opt/pylon")

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PYLON_ROOT}/include")

EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --cflags OUTPUT_VARIABLE CPPFLAGS)
#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPFLAGS}" CACHE STRING "CPPFLAGS")
SET(CPPFLAGS "${CPPFLAGS}" CACHE STRING "CPPFLAGS")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CPPFLAGS}")

EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --libs-rpath OUTPUT_VARIABLE LDFLAGS)
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}" CACHE STRING "LDFLAGS")
SET(LDFLAGS "${LDFLAGS}" CACHE STRING "LDFLAGS")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

EXEC_PROGRAM("${PYLON_ROOT}/bin/pylon-config" ARGS --libs OUTPUT_VARIABLE LDLIBS)
SET(LDLIBS "${LDLIBS}" CACHE STRING "LDLIBS")

add_library(open_image_lib ./include/Grab.h ./src/Grab.cpp ./include/TCPServer.h ./src/server.cpp)
target_link_libraries(open_image_lib ${OpenCV_LIBS} ${LDLIBS}  )
# include_directories(. ${OpenCV_INCLUDE_DIRS})
include_directories(./include ${OpenCV_INCLUDE_DIRS})

# set(target_name mainwindow)


add_executable(main main.cpp)
                
target_link_libraries(main open_image_lib)
find_package(Threads)

target_link_libraries(main ${CMAKE_THREAD_LIBS_INIT})
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(flow_drawing_tools)
#set(CMAKE_CXX_FLAGS "-Wall -DDiscriminatorMode")
set(CMAKE_CXX_FLAGS "-Wall")
set (QnDrawer_VERSION_MAJOR 0)
set (QnDrawer_VERSION_MINOR 9)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE debug)

option(DISCRIMINATOR_MODE "Whether to activate StatDiscriminator or not" OFF)
if (DISCRIMINATOR_MODE)
    add_definitions(-DDiscriminatorMode)
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

find_package( Boost REQUIRED COMPONENTS program_options )
find_package(ROOT REQUIRED)

if (NOT ROOT_FOUND)
    message(WARNING "ROOT was not found with CMAKE_PREFIX_PATH...")
    list(APPEND CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/cmake)
    find_package(ROOT REQUIRED)
endif ()

if (ROOT_USE_FILE)
    message("-- ROOT_USE_FILE ${ROOT_USE_FILE}")
    include(${ROOT_USE_FILE})
endif()

list(APPEND CMAKE_PREFIX_PATH $ENV{QnTools})

find_package(QnTools REQUIRED)

include_directories(
        QnDrawer/include
        ${CMAKE_SOURCE_DIR}
        ${Base_INCLUDE_DIR}
        ${Boost_INCLUDE_DIRS}
        ${QnTools_INCLUDE_DIR}
)

#add_executable(draw src/main.cc src/draw_functions.cc  src/json_reader.cc src/file_manager.cc)
#target_link_libraries(draw QnTools::Base ${ROOT_LIBRARIES} ${Boost_LIBRARIES} )
#
set(SOURCES
        draw/readable_object.cpp
        draw/drawable_object.cpp
        draw/graph.cpp
        draw/histogram_1_d.cpp
        draw/histogram_2_d.cpp
        draw/correlation.cpp
        draw/picture.cpp
        draw/picture_2_d.cpp
        draw/heap_picture.cpp
        draw/ratio.cpp
        draw/dv1dy_container.cpp
        draw/double_differential_correlation.cpp
        draw/double_differential_histogram.cpp
        draw/multi_correlation.cpp
        draw/multi_picture.cpp
        )

string(REPLACE ".cpp" ".h" HEADERS "${SOURCES}")

list(APPEND HEADERS "draw/Helper.h" "draw/QnToolsHelper.h")

set(PCM_FILE_NAME libDraw)
message(STATUS "QnTools include ${QnTools_INCLUDE_DIR}")
ROOT_GENERATE_DICTIONARY( G__Draw ${HEADERS}
        LINKDEF
        draw/DrawLinkDef.h
        OPTIONS
#        -s ${PCM_FILE_NAME}
        -I ${QnTools_INCLUDE_DIR}/QnTools
        -I ${CMAKE_SOURCE_DIR}
        )
add_library(Draw SHARED ${SOURCES} G__Draw.cxx )
target_link_libraries(Draw PRIVATE QnTools::Base ${ROOT_LIBRARIES})

add_executable(draw_picture draw/draw_picture.cpp)
target_link_libraries(draw_picture QnTools::Base ${ROOT_LIBRARIES} Draw )

install(TARGETS Draw EXPORT DrawTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
        INCLUDES DESTINATION include
        )

install(
        FILES
        ${HEADERS}
        DESTINATION
        include
        COMPONENT
        Devel
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}_rdict.pcm"
        DESTINATION
        lib
        OPTIONAL
)

install(
        FILES
        "${CMAKE_CURRENT_BINARY_DIR}/${PCM_FILE_NAME}.rootmap"
        DESTINATION
        lib
        OPTIONAL
)

install (TARGETS draw_picture RUNTIME DESTINATION bin)

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(pepper_control_itd)

# this lets you find the qibuild cmake frameworl
find_package(qibuild)

find_package(VISP REQUIRED)

find_package(visp_naoqi REQUIRED)

message("VISP_NAOQI_LIBRARIES: ${VISP_NAOQI_LIBRARIES}")

# Propagate in subdirs
subdirs(src)

include_directories(${PROJECT_SOURCE_DIR}/src/common ${CMAKE_BINARY_DIR}/include)




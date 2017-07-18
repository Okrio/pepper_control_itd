cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(pepper_control_itd)

# By default set release configuration
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE String "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)
endif()



find_package(Boost  COMPONENTS filesystem system REQUIRED)
find_package(naoqi_libqi)
find_package(naoqi_libqicore)

find_package(VISP REQUIRED)
find_package(visp_naoqi REQUIRED)

include_directories(
  ${visp_naoqi_INCLUDE_DIRS}
  ${VISP_INCLUDE_DIRS}
  ${naoqi_libqi_INCLUDE_DIRS}
  ${naoqi_libqicore_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

# Propagate in subdirs
subdirs(src)






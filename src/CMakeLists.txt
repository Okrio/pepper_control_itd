set(source
  itd_control.cpp
  )

set(ITPP_INCLUDE ./robot/itpp/include/)
include_directories(./robot/itpp/include/)
add_subdirectory(robot)


include_directories(./robot/Misc/)
include_directories(./robot/BSSLocate/)

add_library(misc ./robot/Misc/misc.cpp)
add_library(bsslocate ./robot/BSSLocate/BSSlocate.cpp)
add_library(bssstft ./robot/BSSLocate/BSSstft.cpp)

LINK_DIRECTORIES(/home/aly/workspace/pepper_control_itd/src/robot/itpp/lib/)



foreach(src ${source})
  get_filename_component(binary ${src} NAME_WE)
  qi_create_bin(${binary} ${src})
  qi_use_lib(${binary} visp_naoqi QI ALCOMMON ALPROXIES ALVISION)
  TARGET_LINK_LIBRARIES(${binary} libitpp.so   bsslocate bssstft misc)
endforeach()



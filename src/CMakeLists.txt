set(source
  itd_control.cpp
  )

add_subdirectory(robot)

include_directories(./robot/Misc/)
include_directories(./robot/BSSLocate/)

add_library(misc ./robot/Misc/misc.cpp)
add_library(bsslocate ./robot/BSSLocate/BSSlocate.cpp)
add_library(bssstft ./robot/BSSLocate/BSSstft.cpp)

foreach(src ${source})
  get_filename_component(binary ${src} NAME_WE)
  qi_create_bin(${binary} ${src})
  qi_use_lib(${binary} visp_naoqi QI ALCOMMON ALPROXIES ALVISION)
  TARGET_LINK_LIBRARIES(${binary} itpp bsslocate bssstft misc)
endforeach()



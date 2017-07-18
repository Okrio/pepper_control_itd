set(source
  itd_control.cpp
  )

include_directories(./robot/Misc/)
include_directories(./robot/BSSLocate/)

add_library(misc ./robot/Misc/misc.cpp)
add_library(bsslocate ./robot/BSSLocate/BSSlocate.cpp)
add_library(bssstft ./robot/BSSLocate/BSSstft.cpp)

foreach(src ${source})
  get_filename_component(binary ${src} NAME_WE)
  add_executable(${binary} ${src})
  target_link_libraries(${binary} itpp bsslocate bssstft misc ${visp_naoqi_LIBRARIES} ${VISP_LIBRARIES} ${naoqi_libqi_LIBRARIES}
                        ${naoqi_libqicore_LIBRARIES} ${Boost_LIBRARIES})
endforeach()




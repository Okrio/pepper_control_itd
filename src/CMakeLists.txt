#############################################################################
#
# Copyright (C) 2005 - 2017 by Inria. All rights reserved.
#
# This software is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# ("GPL") version 2 as published by the Free Software Foundation.
# See the file LICENSE.txt at the root directory of this source
# distribution for additional information about the GNU GPL.
#
# For using this software that can not be combined with the GNU
# GPL, please contact Inria about acquiring a Professional
# Edition License.
#
# See http://visp.inria.fr for more information.
#
# This software was developed at:
# Inria Rennes - Bretagne Atlantique
# Campus Universitaire de Beaulieu
# 35042 Rennes Cedex
# France
#
# If you have questions regarding the use of this file, or concerning the
# license please contact Inria at Fabien.Spindler@inria.fr
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Authors:
# Fabien Spindler, Giovanni Claudio, Aly Magassouba
#
#############################################################################

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




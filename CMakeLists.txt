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
# Fabien Spindler, Giovanni Claudio
#
#############################################################################

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




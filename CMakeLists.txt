#
#  The RealityGrid Steering Library VTK Data Reader
#
#  Copyright (c) 2002-2010, University of Manchester, United Kingdom.
#  All rights reserved.
#
#  This software is produced by Research Computing Services, University
#  of Manchester as part of the RealityGrid project and associated
#  follow on projects, funded by the EPSRC under grants GR/R67699/01,
#  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
#  EP/F00561X/1.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of The University of Manchester nor the names
#      of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written
#      permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  Author: Robert Haines

project(REGVTK)

# cmake setup boiler-plate
cmake_minimum_required(VERSION 2.6)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# version numbers
set(REGVTK_VERSION_MAJ 3)
set(REGVTK_VERSION_MIN 0)
set(REGVTK_VERSION_PAT 0)
set(REGVTK_VERSION
    "${REGVTK_VERSION_MAJ}.${REGVTK_VERSION_MIN}.${REGVTK_VERSION_PAT}")

# library version numbers
set(REGVTK_LIBRARY_VERSION
  VERSION "${REGVTK_VERSION}"
  SOVERSION "${REGVTK_VERSION_MAJ}"
)

# set output directories.
if(NOT LIBRARY_OUTPUT_PATH)
  set(LIBRARY_OUTPUT_PATH ${REGVTK_BINARY_DIR}/lib CACHE INTERNAL "Single output directory for building all libraries.")
endif(NOT LIBRARY_OUTPUT_PATH)
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${REGVTK_BINARY_DIR}/bin CACHE INTERNAL "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)

# need ReG
find_package(RealityGrid REQUIRED)
include(${RealityGrid_USE_FILE})
reg_check_version("VTK Modules" ${REGVTK_VERSION_MAJ})

# need VTK
include(InitializeVTK)

# set up install location
#
# optionally install into the current ReG location
# if not building against a build tree install
reg_setup_install_dirs("VTK_Modules")

# set up the RealityGrid library link line
#
# if the Steering Library uses modules, this macro
# does "the right thing"
reg_link_modules()

# common configuration settings
configure_file(
  ${REGVTK_SOURCE_DIR}/vtkRealityGridConfigure.h.in
  ${REGVTK_BINARY_DIR}/vtkRealityGridConfigure.h
)
include_directories(${REGVTK_BINARY_DIR})

# include and then process the directories
# with all the actual classes in them
include_directories(${REGVTK_SOURCE_DIR}/Steering)
add_subdirectory(Steering)

# build examples too?
option(BUILD_EXAMPLES "Build examples." ON)
if(BUILD_EXAMPLES)
  add_subdirectory(Examples)
endif(BUILD_EXAMPLES)

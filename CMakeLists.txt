#
#  CMake build machinery for the RealityGrid VTK Data Reader Module.
#
#  (C) Copyright 2009, University of Manchester, United Kingdom,
#  all rights reserved.
#
#  This software is produced by the Supercomputing, Visualization and
#  e-Science Group, Manchester Computing, University of Manchester
#  as part of the RealityGrid project (http://www.realitygrid.org),
#  funded by the EPSRC under grants GR/R67699/01 and GR/R67699/02.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
#  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
#  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
#  CORRECTION.
#
#  Author.........: Robert Haines
#----------------------------------------------------------------------

project(REGVTK)

# cmake setup boiler-plate
cmake_minimum_required(VERSION 2.6)
if(COMMAND CMAKE_POLICY)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND CMAKE_POLICY)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# version numbers
set(REGVTK_VERSION_MAJ 1)
set(REGVTK_VERSION_MIN 0)
set(REGVTK_VERSION_PAT 0-rc1)
set(REGVTK_VERSION 
    "${REGVTK_VERSION_MAJ}.${REGVTK_VERSION_MIN}.${REGVTK_VERSION_PAT}")

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

# need VTK
include(InitializeVTK)

# set up install location
#
# optionally install into the current ReG location
# if not building against a build tree install
if(${RealityGrid_INSTALL_TYPE} MATCHES "install")
  option(REGVTK_INSTALL_TO_REG_DIR
    "Install the VTK modules to the same location as the RealityGrid installation that they are being built for?"
    OFF
  )
endif(${RealityGrid_INSTALL_TYPE} MATCHES "install")

if(REGVTK_INSTALL_TO_REG_DIR)
  set(CMAKE_INSTALL_PREFIX ${RealityGrid_INSTALL_PREFIX}
    CACHE PATH
    "Install path prefix, prepended onto install directories. The default setting is to install the VTK modules to the same location as the RealityGrid installation that they are being built for."
    FORCE
  )

  string(
    REPLACE "${RealityGrid_INSTALL_PREFIX}/" "" REGVTK_LIB_INSTALL
    ${RealityGrid_LIBRARY_DIRS}
  )
  string(
    REPLACE "${RealityGrid_INSTALL_PREFIX}/" "" REGVTK_INC_INSTALL
    ${RealityGrid_INCLUDE_DIRS}
  )
  string(
    REPLACE "${RealityGrid_INSTALL_PREFIX}/" "" REGVTK_BIN_INSTALL
    ${RealityGrid_BIN_DIR}
  )
  string(
    REPLACE "${RealityGrid_INSTALL_PREFIX}/" "" REGVTK_DOC_INSTALL
    ${RealityGrid_DOCS_DIR}
  )
else(REGVTK_INSTALL_TO_REG_DIR)
  set(REGVTK_LIB_INSTALL "lib/RealityGrid")
  set(REGVTK_INC_INSTALL "include/RealityGrid")
  set(REGVTK_BIN_INSTALL "bin")
  set(REGVTK_DOC_INSTALL "share/doc/RealityGrid")
endif(REGVTK_INSTALL_TO_REG_DIR)

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

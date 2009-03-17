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

find_package(PythonInterp REQUIRED)

# This is how VTK does it too...
#
#  1. Create a list of Python files to be installed/copied.
#  2. Copy them to REGVTK_BINARY_DIR if it is different from REGVTK_SOURCE_DIR.
#  3. Use Python's compileall to compile the copied files.
#
# *** Step 1 has to be done carefully to avoid missing out files ***

# Make directory
file(MAKE_DIRECTORY ${REGVTK_BINARY_DIR}/Wrapping/Python/RealityGrid/vtk)

# List of files
set(REGVTK_PYTHON_FILES
  RealityGrid/__init__
  RealityGrid/vtk/__init__
  RealityGrid/vtk/steering
)

# Copy source files if necessary
set(REGVTK_PYTHON_SOURCE_FILES)
set(REGVTK_PYTHON_OUTPUT_FILES)
foreach(file ${REGVTK_PYTHON_FILES})
  set(src "${REGVTK_BINARY_DIR}/Wrapping/Python/${file}.py")
  set(REGVTK_PYTHON_SOURCE_FILES ${REGVTK_PYTHON_SOURCE_FILES} ${src})
endforeach(file)

if(NOT "${REGVTK_BINARY_DIR}" MATCHES "^${REGVTK_SOURCE_DIR}$")
  foreach(file ${REGVTK_PYTHON_FILES})
    set(src "${REGVTK_SOURCE_DIR}/Wrapping/Python/${file}.py")
    set(tgt "${REGVTK_BINARY_DIR}/Wrapping/Python/${file}.py")
    add_custom_command(DEPENDS ${src}
      COMMAND ${CMAKE_COMMAND} -E copy ${src} ${tgt}
      OUTPUT ${tgt}
      COMMENT "source copy")
  endforeach(file)
endif(NOT "${REGVTK_BINARY_DIR}" MATCHES "^${REGVTK_SOURCE_DIR}$")

# Byte compile the Python files
configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/compile_all.py.in
  ${CMAKE_CURRENT_BINARY_DIR}/compile_all.py
  @ONLY IMMEDIATE
)
add_custom_command(
  COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/compile_all.py
  DEPENDS ${REGVTK_PYTHON_SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/compile_all.py
  OUTPUT "${REGVTK_BINARY_DIR}/Wrapping/Python/compile_complete"
  COMMENT "Byte compiling Python code"
)

# Add a target that depends on the above so it gets done
add_custom_target(regvtk_python ALL echo "..."
  DEPENDS "${REGVTK_BINARY_DIR}/Wrapping/Python/compile_complete"
)

# Add a post-install rule and scripts to install the python wrappers
# using distutils. setup.py does the work, PythonInstall.cmake drives it.
set(DOLLAR "$") # Need to deref the dollar sign...

if(NOT DEFINED REGVTK_PYTHON_SETUP_ARGS)
  set(REGVTK_PYTHON_SETUP_ARGS "--prefix=\"${DOLLAR}{CMAKE_INSTALL_PREFIX}\""
    CACHE STRING "Arguments passed to \"python setup.py install ...\" during installation."
  )
  mark_as_advanced(REGVTK_PYTHON_SETUP_ARGS)
endif(NOT DEFINED REGVTK_PYTHON_SETUP_ARGS)

configure_file(
  ${REGVTK_SOURCE_DIR}/Wrapping/Python/PythonInstall.cmake.in
  ${REGVTK_BINARY_DIR}/Wrapping/Python/PythonInstall.cmake
  @ONLY IMMEDIATE
)
set_target_properties(regvtk_python PROPERTIES POST_INSTALL_SCRIPT
  ${REGVTK_BINARY_DIR}/Wrapping/Python/PythonInstall.cmake
)

configure_file(
  ${REGVTK_SOURCE_DIR}/Wrapping/Python/setup.py.in
  ${REGVTK_BINARY_DIR}/Wrapping/Python/setup.py
  @ONLY IMMEDIATE
)

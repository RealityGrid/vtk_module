#
#  The RealityGrid Steering Library VTK Data Reader
#
#  Copyright (c) 2002-2009, University of Manchester, United Kingdom.
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

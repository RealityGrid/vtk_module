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

#
# Try to find VTK and include its settings (otherwise complain)
#
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

#
# Build shared libs ?
#
# Defaults to the same VTK setting.
#

# Standard CMake option for building libraries shared or static by default.
option(BUILD_SHARED_LIBS
       "Build with shared libraries."
       ${VTK_BUILD_SHARED_LIBS})
# Copy the CMake option to a setting with REGVTK_ prefix for use in
# our project.  This name is used in regvtkConfigure.h.in.
set(REGVTK_BUILD_SHARED_LIBS ${BUILD_SHARED_LIBS})

#
# Wrap Tcl, Java, Python
#
# Rational: even if your VTK was wrapped, it does not mean that you want to
# wrap your own local classes.
# Default value is OFF as the VTK cache might have set them to ON but
# the wrappers might not be present (or yet not found).
#

#
# Tcl
#

#if(VTK_WRAP_TCL)
#
#  option(REGVTK_WRAP_TCL
#         "Wrap classes into the TCL interpreted language."
#         OFF)
#
#  if(REGVTK_WRAP_TCL)
#    include(${VTK_CMAKE_DIR}/vtkWrapTcl.cmake)
#  endif(REGVTK_WRAP_TCL)
#
#else(VTK_WRAP_TCL)
#
#  if(REGVTK_WRAP_TCL)
#    message("Warning. REGVTK_WRAP_TCL is ON but the VTK version you have "
#            "chosen has not support for Tcl (VTK_WRAP_TCL is OFF).  "
#            "Please set REGVTK_WRAP_TCL to OFF.")
#    set(REGVTK_WRAP_TCL OFF)
#  endif(REGVTK_WRAP_TCL)
#
#endif(VTK_WRAP_TCL)

#
# Python
#

if(VTK_WRAP_PYTHON)

  option(REGVTK_WRAP_PYTHON
         "Wrap classes into the Python interpreted language."
         OFF)

  if(REGVTK_WRAP_PYTHON)
    include(${VTK_CMAKE_DIR}/vtkWrapPython.cmake)
    if(WIN32)
      if(NOT BUILD_SHARED_LIBS)
        message(FATAL_ERROR "Python support requires BUILD_SHARED_LIBS to be ON.")
        set(REGVTK_CAN_BUILD 0)
      endif(NOT BUILD_SHARED_LIBS)
    endif(WIN32)
  endif(REGVTK_WRAP_PYTHON)

else(VTK_WRAP_PYTHON)

  if(REGVTK_WRAP_PYTHON)
    message("Warning. REGVTK_WRAP_PYTHON is ON but the VTK version you have "
            "chosen has not support for Python (VTK_WRAP_PYTHON is OFF).  "
            "Please set REGVTK_WRAP_PYTHON to OFF.")
    set(REGVTK_WRAP_PYTHON OFF)
  endif(REGVTK_WRAP_PYTHON)

endif(VTK_WRAP_PYTHON)

#
# Java
#

if(VTK_WRAP_JAVA)

  option(REGVTK_WRAP_JAVA
         "Wrap classes into the Java interpreted language."
         OFF)

  if(REGVTK_WRAP_JAVA)
    set(VTK_WRAP_JAVA3_INIT_DIR "${REGVTK_SOURCE_DIR}/Wrapping")
    include(${VTK_CMAKE_DIR}/vtkWrapJava.cmake)
    if(WIN32)
      if(NOT BUILD_SHARED_LIBS)
        message(FATAL_ERROR "Java support requires BUILD_SHARED_LIBS to be ON.")
        set(REGVTK_CAN_BUILD 0)
      endif(NOT BUILD_SHARED_LIBS)
    endif(WIN32)

    # Tell the java wrappers where to go.
    set(VTK_JAVA_HOME ${REGVTK_BINARY_DIR}/java/vtk)
    make_directory(${VTK_JAVA_HOME})
  endif(REGVTK_WRAP_JAVA)

else(VTK_WRAP_JAVA)

  if(REGVTK_WRAP_JAVA)
    message("Warning. REGVTK_WRAP_JAVA is ON but the VTK version you have "
            "chosen has not support for Java (VTK_WRAP_JAVA is OFF).  "
            "Please set REGVTK_WRAP_JAVA to OFF.")
    set(REGVTK_WRAP_JAVA OFF)
  endif(REGVTK_WRAP_JAVA)

endif(VTK_WRAP_JAVA)

# Setup our local hints file in case wrappers need them.
set(VTK_WRAP_HINTS ${REGVTK_SOURCE_DIR}/Wrapping/hints)

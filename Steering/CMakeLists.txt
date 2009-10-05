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

set(Steering_SRCS
  vtkRealityGridDataReader.cxx
  vtkRealityGridDataSlice.cxx
  vtkRealityGridDataSliceCollection.cxx
  vtkRealityGridIOChannel.cxx
)

# Get list of headers for installation
foreach(file ${Steering_SRCS})
  string(REPLACE ".cxx" ".h" temp ${file})
  set(Steering_INCS ${Steering_INCS} ${temp})
endforeach(file ${Steering_SRCS})

# Generate typed data slice classes
set(TYPES Int Char Float Double)
foreach(Type ${TYPES})
  string(TOLOWER ${Type} type)
  string(TOUPPER ${Type} TYPE)
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkRealityGridTypedDataSlice.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/vtkRealityGrid${Type}DataSlice.h
  )
  configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/vtkRealityGridTypedDataSlice.cxx.in
    ${CMAKE_CURRENT_BINARY_DIR}/vtkRealityGrid${Type}DataSlice.cxx
  )
  set(Steering_INCS ${Steering_INCS}
    ${CMAKE_CURRENT_BINARY_DIR}/vtkRealityGrid${Type}DataSlice.h
  )
  set(Steering_SRCS ${Steering_SRCS}
    ${CMAKE_CURRENT_BINARY_DIR}/vtkRealityGrid${Type}DataSlice.cxx
  )
endforeach(Type ${TYPES})

# add include directory for the generated classes
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Create the vtkRealityGridSteering C++ library.
add_library(vtkRealityGridSteering ${Steering_SRCS})
target_link_libraries(vtkRealityGridSteering 
  ${REG_LINK_LIBRARIES}
  ${REG_EXTERNAL_LIBS}
  vtkCommon
)
set(Steering_INSTALL_TARGETS vtkRealityGridSteering)

# Label certain classes as abstract for the wrappers
set_source_files_properties(
  vtkRealityGridDataSlice.cxx
  ABSTRACT
)

# Create the vtkRealityGridSteering Tcl library.
if(VTK_WRAP_TCL AND REGVTK_WRAP_TCL)
  VTK_WRAP_TCL3(vtkRealityGridSteeringTCL SteeringTCL_SRCS "${Steering_SRCS}" "")
  add_library(vtkRealityGridSteeringTCL ${SteeringTCL_SRCS})
  target_link_libraries(vtkRealityGridSteeringTCL vtkRealityGridSteering vtkCommonTCL)
  set(Steering_INSTALL_TARGETS ${Steering_INSTALL_TARGETS} vtkRealityGridSteeringTCL)
endif(VTK_WRAP_TCL AND REGVTK_WRAP_TCL)

# Create the vtkRealityGridSteering Python library.
if(VTK_WRAP_PYTHON AND REGVTK_WRAP_PYTHON)
  VTK_WRAP_PYTHON3(vtkRealityGridSteeringPython SteeringPython_SRCS "${Steering_SRCS}")
  add_library(vtkRealityGridSteeringPythonD ${SteeringPython_SRCS})
  add_library(vtkRealityGridSteeringPython MODULE vtkRealityGridSteeringPythonInit.cxx)
  target_link_libraries(vtkRealityGridSteeringPythonD vtkRealityGridSteering vtkCommon vtkCommonPythonD)
  target_link_libraries(vtkRealityGridSteeringPython vtkRealityGridSteeringPythonD)
  set(Steering_INSTALL_TARGETS ${Steering_INSTALL_TARGETS} vtkRealityGridSteeringPythonD)

  # install rules for python stuff
  add_subdirectory(../Wrapping/Python ${REGVTK_BINARY_DIR}/Wrapping/Python)
endif(VTK_WRAP_PYTHON AND REGVTK_WRAP_PYTHON)

# Create the vtkRealityGridSteering Java library.
if(VTK_WRAP_JAVA AND REGVTK_WRAP_JAVA)
  VTK_WRAP_JAVA3(vtkRealityGridSteeringJava SteeringJava_SRCS "${Steering_SRCS}")
  add_library(vtkRealityGridSteeringJava SHARED ${SteeringJava_SRCS})
  target_link_libraries(vtkRealityGridSteeringJava vtkRealityGridSteering vtkCommonJava)
  set(Steering_INSTALL_TARGETS ${Steering_INSTALL_TARGETS} vtkRealityGridSteeringJava)

  # build jar file
  add_subdirectory(../Wrapping/Java ${REGVTK_BINARY_DIR}/Wrapping/Java)
endif(VTK_WRAP_JAVA AND REGVTK_WRAP_JAVA)

# set library version numbers
set_target_properties(${Steering_INSTALL_TARGETS}
  PROPERTIES ${REGVTK_LIBRARY_VERSION}
)

# install rules
install(TARGETS ${Steering_INSTALL_TARGETS}
  LIBRARY DESTINATION ${REGVTK_LIB_INSTALL}
)
install(FILES ${Steering_INCS}
  DESTINATION ${REGVTK_INC_INSTALL}
)

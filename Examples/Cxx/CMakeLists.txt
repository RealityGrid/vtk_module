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

set(mini_SRCS mini_viz.cxx)

# add some platform specific stuff
set(GUI_TYPE)
set(EXE_NAME mini_viz)
if(APPLE)
  # set gui type and app name
  set(GUI_TYPE MACOSX_BUNDLE)
  set(EXE_NAME MiniViz)

  # add icons to the build
  set(mini_ICNS ${PROJECT_SOURCE_DIR}/Resources/realitygrid.icns)
  set(mini_SRCS ${mini_SRCS} ${mini_ICNS})
  set(MACOSX_BUNDLE_ICON_FILE ${mini_ICNS})
  set_source_files_properties(${mini_ICNS}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

  # set bundle info strings
  set(MACOSX_BUNDLE_INFO_STRING "${REGVTK_VERSION}, (c) 2002-2010, The University of Manchester, UK.")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${REGVTK_VERSION}")
  set(MACOSX_BUNDLE_COPYRIGHT "(c) 2002-2010, University of Manchester, UK.")
endif(APPLE)

# build the executable
add_executable(${EXE_NAME} ${GUI_TYPE} ${mini_SRCS})
target_link_libraries(${EXE_NAME}
  vtkRealityGridSteering
  vtkRendering
  vtkFiltering
)

# install rules
install(TARGETS ${EXE_NAME}
  RUNTIME DESTINATION ${VTK_MODULES_BIN_INSTALL}
  BUNDLE  DESTINATION ${VTK_MODULES_BIN_INSTALL}
)

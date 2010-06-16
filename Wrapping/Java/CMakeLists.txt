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

# Java compile stuff we have overridden to compensate for CMake bugs
set(CMAKE_USER_MAKE_RULES_OVERRIDE "JavaCompileOverride")

enable_language(Java)
mark_as_advanced(CMAKE_Java_ARCHIVE)
mark_as_advanced(CMAKE_Java_RUNTIME)

# the set of classes to build
set(REGVTK_JAVA_DEPENDENCIES "")
include(${REGVTK_BINARY_DIR}/Steering/JavaDependencies.cmake)
set_source_files_properties(
  ${VTK_JAVA_DEPENDENCIES}
  PROPERTIES GENERATED TRUE
)

# add the directory that the java files are generated in to the classpath
include_directories(${REGVTK_BINARY_DIR}/java)

# find the vtk jar file
# need the extra glob as the jar file is sometimes
# installed to the wrong place.
file(GLOB_RECURSE VTK_JAR_FILE ${VTK_JAVA_JAR})
include_directories(${VTK_JAR_FILE})

# build the jar
add_library(vtkRealityGrid STATIC ${VTK_JAVA_DEPENDENCIES})
add_dependencies(vtkRealityGrid vtkRealityGridSteeringJavaJavaClasses)

# install it
install(TARGETS vtkRealityGrid
  ARCHIVE DESTINATION ${VTK_MODULES_LIB_INSTALL}/java
)

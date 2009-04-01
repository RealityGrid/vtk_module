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

# Java compile stuff we have overridden to compensate for CMake bugs
set(CMAKE_USER_MAKE_RULES_OVERRIDE "JavaCompileOverride")

enable_language(Java)

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
  ARCHIVE DESTINATION ${REGVTK_LIB_INSTALL}/java
)

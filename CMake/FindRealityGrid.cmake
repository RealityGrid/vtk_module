#
#  CMake script to find a RealityGrid Steering Library installation.
#
#  (C) Copyright 2004-2009, University of Manchester, United Kingdom,
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

set(REG_DIR_DESCRIPTION "directory containing RealityGridConfig.cmake.  This is either the root of the build tree, or PREFIX/lib/RealityGrid for an installation.")
set(REG_DIR_MESSAGE "RealityGrid not found.  Set the RealityGrid_DIR cmake cache entry to the ${REG_DIR_DESCRIPTION}")


#---------------------------------------------------------

# Search only if the location is not already known.
if(NOT RealityGrid_DIR)
  # Get the system search path as a list.
  if(UNIX)
    string(REGEX MATCHALL "[^:]+" REG_DIR_SEARCH1 "$ENV{PATH}")
  else(UNIX)
    string(REGEX REPLACE "\\\\" "/" REG_DIR_SEARCH1 "$ENV{PATH}")
  endif(UNIX)
  string(REGEX REPLACE "/;" ";" REG_DIR_SEARCH2 "${REG_DIR_SEARCH1}")

  # Construct a set of paths relative to the system search path.
  set(REG_DIR_SEARCH "")
  foreach(dir ${REG_DIR_SEARCH2})
    set(REG_DIR_SEARCH ${REG_DIR_SEARCH}
      ${dir}/../lib/RealityGrid
    )
  endforeach(dir)

  find_path(RealityGrid_DIR UseRealityGrid.cmake
    # look for expected environment having been set
    $ENV{REG_STEER_HOME}/lib/RealityGrid
    $ENV{REG_STEER_HOME}/lib
    $ENV{REG_STEER_HOME}
    $ENV{REG_HOME}/lib/RealityGrid
    $ENV{REG_HOME}/lib
    $ENV{REG_HOME}

    # relative to the executable path
    ${REG_DIR_SEARCH}

    # standard unix locations
    /usr/local/lib/RealityGrid
    /usr/local/lib
    /usr/lib/RealityGrid
    /usr/lib

    # give the user some clues if we can't find it
    DOC "The ${REG_DIR_DESCRIPTION}"
  )
endif(NOT RealityGrid_DIR)

#---------------------------------------------------------

if(RealityGrid_DIR)
  # have we really found it?
  if(EXISTS ${RealityGrid_DIR}/RealityGridConfig.cmake)
    # yes - load settings
    set(RealityGrid_FOUND 1)
    include(${RealityGrid_DIR}/RealityGridConfig.cmake)
  else(EXISTS ${RealityGrid_DIR}/RealityGridConfig.cmake)
    # no
    set(RealityGrid_FOUND 0)
  endif(EXISTS ${RealityGrid_DIR}/RealityGridConfig.cmake)
else(RealityGrid_DIR)
  # not found
  set(RealityGrid_FOUND 0)
endif(RealityGrid_DIR)

#---------------------------------------------------------

if(NOT RealityGrid_FOUND)
  if(RealityGrid_FIND_REQUIRED)
    message(FATAL_ERROR ${REG_DIR_MESSAGE})
  else(RealityGrid_FIND_REQUIRED)
    if(NOT RealityGrid_FIND_QUIETLY)
      message(STATUS ${REG_DIR_MESSAGE})
    endif(NOT RealityGrid_FIND_QUIETLY)
  endif(RealityGrid_FIND_REQUIRED)
endif(NOT RealityGrid_FOUND)

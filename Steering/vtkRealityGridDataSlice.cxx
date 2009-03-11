/*-------------------------------------------------------------------------
  (C) Copyright 2009, University of Manchester, United Kingdom,
  all rights reserved.

  This software was developed by the RealityGrid project
  (http://www.realitygrid.org), funded by the EPSRC under grants
  GR/R67699/01 and GR/R67699/02.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
  CORRECTION.

  Author: Robert Haines
---------------------------------------------------------------------------*/

#include "vtkRealityGridDataSlice.h"

#include "vtkRealityGridCharDataSlice.h"
#include "vtkRealityGridDoubleDataSlice.h"
#include "vtkRealityGridFloatDataSlice.h"
#include "vtkRealityGridIntDataSlice.h"

vtkCxxRevisionMacro(vtkRealityGridDataSlice, "Revision: 0.01");

// instantiate default
vtkRealityGridDataSlice::vtkRealityGridDataSlice() {
  this->size = 0;
}

void vtkRealityGridDataSlice::PrintSelf(ostream& os, vtkIndent indent) {
  os << indent << "Data size: " << this->size << "\n";
}

vtkRealityGridDataSlice* vtkRealityGridDataSlice::CreateDataSlice(int type) {
  switch(type) {
  case REG_INT:
    return vtkRealityGridIntDataSlice::New();
  case REG_CHAR:
    return vtkRealityGridCharDataSlice::New();
  case REG_FLOAT:
    return vtkRealityGridFloatDataSlice::New();
  case REG_DOUBLE:
    return vtkRealityGridDoubleDataSlice::New();
  }

  std::cerr << "Unrecognised data type: " << type << ". Setting to REG_CHAR\n";

  return vtkRealityGridCharDataSlice::New();
}

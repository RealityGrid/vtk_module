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

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkRealityGridDataSlice, "Revision: 0.01");
vtkStandardNewMacro(vtkRealityGridDataSlice);

// instantiate default
vtkRealityGridDataSlice::vtkRealityGridDataSlice() {
  this->type = -1;
  this->size = 0;
  this->data = NULL;
}

vtkRealityGridDataSlice::~vtkRealityGridDataSlice() {
  if(this->data) {
    switch(this->type) {
    case REG_INT:
      delete [] (int*) this->data;
      break;
    case REG_CHAR:
      delete [] (char*) this->data;
      break;
    case REG_FLOAT:
      delete [] (float*) this->data;
      break;
    case REG_DBL:
      delete [] (double*) this->data;
      break;
    }
  }
}

void vtkRealityGridDataSlice::PrintSelf(ostream& os, vtkIndent indent) {
  os << indent << "vtkRealityGridDataSlice (" << this << ")\n";
  indent = indent.GetNextIndent();
  os << indent << "Data type: ";
  switch(this->type) {
  case REG_INT:
    os << "Int\n";
    break;
  case REG_CHAR:
    os << "Char\n";
    break;
  case REG_FLOAT:
    os << "Float\n";
    break;
  case REG_DBL:
    os << "Double\n";
    break;
  default:
    os << "(none)\n";
    break;
  }
  os << indent << "Data size: " << this->size << "\n";
}

void vtkRealityGridDataSlice::SetDataType(int t) {
  this->type = t;
}

void vtkRealityGridDataSlice::SetDataSize(int s) {
  this->size = s;
}

void vtkRealityGridDataSlice::SetData(void* d) {
  this->data = d;
}

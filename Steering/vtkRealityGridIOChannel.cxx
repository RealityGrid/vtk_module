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

#include "vtkRealityGridDataSliceCollection.h"
#include "vtkRealityGridIOChannel.h"

#include "vtkObjectFactory.h"

vtkCxxRevisionMacro(vtkRealityGridIOChannel, "Revision: 0.01");
vtkStandardNewMacro(vtkRealityGridIOChannel);

vtkRealityGridIOChannel::vtkRealityGridIOChannel() {
  this->handle = -1;
  this->io_direction = -1;
  this->data_slices = vtkRealityGridDataSliceCollection::New();
}

vtkRealityGridIOChannel::~vtkRealityGridIOChannel() {
  if(this->data_slices) {
    this->data_slices->Delete();
    this->data_slices = NULL;
  }
}

void vtkRealityGridIOChannel::PrintSelf(ostream& os, vtkIndent indent) {
  os << indent << "vtkRealityGridIOChannel (" << this << ")\n";
  indent = indent.GetNextIndent();
  os << indent << "Name: " << (name != NULL ? name : "(none)") << "\n";
  os << indent << "IO Handle: " << this->handle << "\n";
  os << indent << "IO Direction: ";
  switch(this->io_direction) {
  case REG_IO_IN:
    os << "IN\n";
    break;
  case REG_IO_OUT:
    os << "OUT\n";
    break;
  case REG_IO_INOUT:
    os << "IN and OUT\n";
    break;
  }
  os << indent << "Data:";
  if(data_slices == NULL) {
    os << " (none)\n";
  }
  else {
    os << "\n";
    data_slices->PrintSelf(os, indent.GetNextIndent());
  }
}

void vtkRealityGridIOChannel::SetHandle(int h) {
  this->handle = h;
}

void vtkRealityGridIOChannel::SetName(const char* n) {
  this->name = (char*) n;
}

void vtkRealityGridIOChannel::SetIODirection(const int d) {
  this->io_direction = d;
}

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
#include "vtkRealityGridDataSliceCollection.h"
#include "vtkRealityGridIOChannel.h"

#include "vtkObjectFactory.h"

#include "ReG_Steer_Appside.h"

vtkCxxRevisionMacro(vtkRealityGridIOChannel, "Revision: 0.01");
vtkStandardNewMacro(vtkRealityGridIOChannel);

vtkRealityGridIOChannel::vtkRealityGridIOChannel() {
  this->handle = -1;
  this->io_direction = -1;
  this->data_in = vtkRealityGridDataSliceCollection::New();
  this->data_out = vtkRealityGridDataSliceCollection::New();
}

vtkRealityGridIOChannel::~vtkRealityGridIOChannel() {
  if(this->data_in)
    this->data_in->Delete();

  if(this->data_out)
    this->data_out->Delete();
}

void vtkRealityGridIOChannel::PrintSelf(ostream& os, vtkIndent indent) {
  os << indent << "vtkRealityGridIOChannel (" << this << ")\n";
  indent = indent.GetNextIndent();
  os << indent << "Name: " << (name != NULL ? name : "(none)") << "\n";
  os << indent << "IO Handle: " << this->handle << "\n";
  os << indent << "IO Direction: ";
  switch(this->io_direction) {
  case REG_IO_IN:
    os << "IN\n" << indent << "Data Slices:\n";
    data_in->PrintSelf(os, indent.GetNextIndent());
    break;
  case REG_IO_OUT:
    os << "OUT\n" << indent << "Data Slices:\n";
    data_out->PrintSelf(os, indent.GetNextIndent());
    break;
  case REG_IO_INOUT:
    os << "IN and OUT\n" << indent << "Data Slices In:\n";
    data_in->PrintSelf(os, indent.GetNextIndent());
    os << indent << "Data Slices Out:\n";
    data_out->PrintSelf(os, indent.GetNextIndent());
    break;
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

bool vtkRealityGridIOChannel::Update(int loop) {
  bool result = false;

  switch(io_direction) {
  case REG_IO_IN:
    result = RecvData();
    break;
  case REG_IO_OUT:
    SendData(loop);
    break;
  case REG_IO_INOUT:
    SendData(loop);
    result = RecvData();
    break;
  }

  return result;
}

bool vtkRealityGridIOChannel::RecvData() {
  int status;
  int data_type;
  int data_count;
  int slices_read = 0;
  REG_IOHandleType iohandle;

  vtkRealityGridDataSlice* slice;
  void* data;

  status = Consume_start(handle, &iohandle);
  if(status == REG_SUCCESS) {

    // data is available to read. get header describing it
    status = Consume_data_slice_header(iohandle, &data_type, &data_count);

    while(status == REG_SUCCESS) {

      // get slice to put data into
      slice = data_in->GetDataSlice(slices_read);

      if(slice == NULL) {
	slice = vtkRealityGridDataSlice::New();
	data_in->AddItem(slice);
      }

      switch(data_type) {
      case REG_INT:
	data = slice->GetData();
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (int*) data;
	  data = new int[data_count];
	  slice->SetDataSize(data_count);
	  slice->SetData(data);
	}
	slice->SetDataType(REG_INT);
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_CHAR:
	data = slice->GetData();
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (char*) data;
	  data = new char[data_count];
	  slice->SetDataSize(data_count);
	  slice->SetData(data);
	}
	slice->SetDataType(REG_CHAR);
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_FLOAT:
	data = slice->GetData();
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (float*) data;
	  data = new float[data_count];
	  slice->SetDataSize(data_count);
	  slice->SetData(data);
	}
	slice->SetDataType(REG_FLOAT);
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_DBL:
	data = slice->GetData();
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (double*) data;
	  data = new double[data_count];
	  slice->SetDataSize(data_count);
	  slice->SetData(data);
	}
	slice->SetDataType(REG_DBL);
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      } // end switch(data_type)

      slices_read++;
      status = Consume_data_slice_header(iohandle, &data_type, &data_count);
    } // end while

    status = Consume_stop(&iohandle);
  }

  return (slices_read != 0);
}

void vtkRealityGridIOChannel::SendData(int loop) {
  int status;
  int num_slices;
  REG_IOHandleType iohandle;
  vtkRealityGridDataSlice* slice;

  num_slices = data_out->GetNumberOfItems();
  if(num_slices == 0)
    return;

  status = Emit_start(handle, loop, &iohandle);
    
  if(status == REG_SUCCESS) {
    for(int i = 0; i < num_slices; i++) {
      slice = data_out->GetDataSlice(i);
      
      status = Emit_data_slice(iohandle,
			       slice->GetDataType(),
			       slice->GetDataSize(),
			       slice->GetData());
    }
  }

  Emit_stop(&iohandle);
}

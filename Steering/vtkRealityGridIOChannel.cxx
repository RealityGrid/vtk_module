/*
  The RealityGrid Steering Library VTK Data Reader

  Copyright (c) 2002-2009, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  Author: Robert Haines
 */

#include "vtkRealityGridDataReader.h"
#include "vtkRealityGridDataSlice.h"
#include "vtkRealityGridDataSliceCollection.h"
#include "vtkRealityGridIOChannel.h"

#include "vtkCommand.h"
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

void vtkRealityGridIOChannel::Register(vtkRealityGridDataReader* dr, int f) {
  dr->RegisterIOChannel(this, f);
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

  if(result)
    InvokeEvent(vtkCommand::UserEvent, NULL);

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
	slice = vtkRealityGridDataSlice::CreateDataSlice(data_type);
	data_in->AddItem(slice);
      }

      // get pointer to data from slice
      data = slice->GetVoidData();

      switch(data_type) {
      case REG_INT:
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (int*) data;
	  data = new int[data_count];
	  slice->SetData(data, data_count);
	}
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_CHAR:
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (char*) data;
	  data = new char[data_count];
	  slice->SetData(data, data_count);
	}
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_FLOAT:
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (float*) data;
	  data = new float[data_count];
	  slice->SetData(data, data_count);
	}
	status = Consume_data_slice(iohandle, data_type, data_count, data);
	break;
      case REG_DOUBLE:
	if(data_count > slice->GetDataSize()) {
	  if(data) delete [] (double*) data;
	  data = new double[data_count];
	  slice->SetData(data, data_count);
	}
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
			       slice->GetVoidData());
    }
  }

  Emit_stop(&iohandle);
}

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

#include <iostream>

#include "vtkRealityGridDataReader.h"
#include "vtkRealityGridDataSliceCollection.h"
#include "vtkRealityGridIOChannel.h"

#include "vtkCallbackCommand.h"
#include "vtkCommand.h"
#include "vtkRenderWindowInteractor.h"

#include "ReG_Steer_Appside.h"

vtkCxxRevisionMacro(vtkRealityGridDataReader, "Revision: 0.01");

vtkRealityGridDataReader* vtkRealityGridDataReader::_instance = NULL;

vtkRealityGridDataReader* vtkRealityGridDataReader::New() {
  if(_instance == NULL)
    _instance = new vtkRealityGridDataReader();

  return _instance;
}

vtkRealityGridDataReader* vtkRealityGridDataReader::GetInstance() {
  return _instance;
}

// instantiate default
vtkRealityGridDataReader::vtkRealityGridDataReader() {
  for(int i = 0; i < REG_INITIAL_NUM_IOTYPES; i++) {
    this->io_channels[i] = vtkRealityGridIOChannel::New();
  }
  this->interactor = NULL;
  this->update_callback = NULL;
  this->user_data = NULL;
  this->InitializeRealityGrid();
}

vtkRealityGridDataReader::~vtkRealityGridDataReader() {
  for(int i = 0; i < REG_INITIAL_NUM_IOTYPES; i++) {
    if(io_channels[i]) {
      this->io_channels[i]->Delete();
    }
  }
  this->FinalizeRealityGrid();
  _instance = NULL;
}

void vtkRealityGridDataReader::PrintSelf(ostream& os, vtkIndent indent) {
  this->Superclass::PrintSelf(os, indent);
  os << indent << "Interactor: " << (interactor == NULL ? "not set" : "set") << "\n";
  os << indent << "Update Callback: " << (update_callback == NULL ? "not set" : "set") << "\n";
  os << indent << "IO Channels: ";
  if(num_io_channels > 0) {
    os << num_io_channels << std::endl;
    for(int i = 0; i < num_io_channels; i++) {
      io_channels[i]->PrintSelf(os, indent.GetNextIndent());
    }
  }
  else {
      os << "(none)\n";
  }
  os << indent << "Loop number: " << this->loop_number << "\n";
}

void vtkRealityGridDataReader::SetInteractor(vtkRenderWindowInteractor* i) {
  this->interactor = i;

  vtkCallbackCommand* timer_callback = vtkCallbackCommand::New();
  timer_callback->SetCallback(_poll);
  timer_callback->SetClientData((void*) this);
  this->interactor->AddObserver(vtkCommand::TimerEvent, timer_callback);
  this->interactor->CreateTimer(VTKI_TIMER_FIRST);
}

void vtkRealityGridDataReader::SetUpdateUserData(void* ud) {
  this->user_data = ud;
}

// Initialize the RealityGrid steering library
void vtkRealityGridDataReader::InitializeRealityGrid() {

  // allocate memory
  changedParamLabels = Alloc_string_array(REG_MAX_STRING_LENGTH,
					  REG_MAX_NUM_STR_PARAMS);
  recvdCmdParams = Alloc_string_array(REG_MAX_STRING_LENGTH,
				      REG_MAX_NUM_STR_CMDS);

  // initialise steering library
  Steering_enable(REG_TRUE);
  Steering_initialize("RealityGrid VTK Data Reader", 0, NULL);

  loop_number = 0;
  num_io_channels = 0;
}

// Call Steering_control and do required steering ops
bool vtkRealityGridDataReader::PollRealityGrid() {
  int status;
  int data_type;
  int data_count;
  int slices_read = 0;
  REG_IOHandleType iohandle;

  status = Steering_control(loop_number++,
			    &numParamsChanged,
			    changedParamLabels,
			    &numRecvdCmds,
			    recvdCmds,
			    recvdCmdParams);

  if(status != REG_SUCCESS) {
    std::cerr << "Call to Steering_control failed...\n";
    return false;
  }

  // deal with steering commands
  for(int i = 0; i < numRecvdCmds; i++) {

    // work out which IO handle we need to read from
    int io_handle_num = -1;
    for(int j = 0; j < REG_INITIAL_NUM_IOTYPES; j++) {
      if(recvdCmds[i] == io_channels[j]->GetHandle())
	io_handle_num = j;
    }

    // read from the IO handle
    if(io_handle_num != -1) {
      // open the channel to consume data
      status = Consume_start(io_channels[io_handle_num]->GetHandle(), &iohandle);
      if(status == REG_SUCCESS) {

	// data is available to read. get header describing it
	slices_read = 0;
	status = Consume_data_slice_header(iohandle, &data_type, &data_count);

	while(status == REG_SUCCESS) {

	  vtkRealityGridDataSliceCollection* slices;
	  vtkRealityGridDataSlice* slice;
	  void* data;

	  // get slice to put data into
	  slices = io_channels[io_handle_num]->GetDataSlices();
	  slice = slices->GetDataSlice(slices_read);

	  if(slice == NULL) {
	    slice = vtkRealityGridDataSlice::New();
	    slices->AddItem(slice);
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
      } // Consume_start
    }
  } // numRecvdCmds

  return (slices_read != 0);
}

void vtkRealityGridDataReader::FinalizeRealityGrid() {
  // clean up steering library
  Steering_finalize();
}

// Register an IO channel
// TODO: check bounds of io_handles[]
void vtkRealityGridDataReader::RegisterIOChannel(const char* name, int dir, int freq) {
  int status;
  int io_handle;

  status = Register_IOType((char*) name, dir, freq, &io_handle);
  if(status == REG_SUCCESS) {
    io_channels[num_io_channels]->SetHandle(io_handle);
    io_channels[num_io_channels]->SetName(name);
    io_channels[num_io_channels]->SetIODirection(dir);
    num_io_channels++;
  }
}

// friend method for VTK polling loop callback
// TODO: Might need some throttling at some point?
void _poll(vtkObject* obj, unsigned long eid, void* cd, void* calld) {
  // get VTK interactor
  vtkRenderWindowInteractor* i = vtkRenderWindowInteractor::SafeDownCast(obj);

  // get vtkRealityGridDataReader instance
  vtkRealityGridDataReader* reader = (vtkRealityGridDataReader*) cd;

  // return if there is no update callback registered.
  if(reader->update_callback == NULL) {
    // reset timer
    i->CreateTimer(VTKI_TIMER_UPDATE);
    return;
  }

  // do ReG stuff and re-render if necessary
  if(reader->PollRealityGrid()) {
    // call update callback
    reader->update_callback(reader->io_channels, reader->user_data);

    // re-render
    i->Render();
  }

  // reset timer
  i->CreateTimer(VTKI_TIMER_UPDATE);
}

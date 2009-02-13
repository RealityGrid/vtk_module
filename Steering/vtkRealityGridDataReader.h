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

#ifndef __vtkRealityGridDataReader_h__

#include "vtkSteeringWin32Header.h"
#include "ReG_Steer_types.h"

#include "vtkObject.h"

class vtkRenderWindowInteractor;

class vtkRealityGridDataSliceCollection;

class REGVTK_STEERING_EXPORT vtkRealityGridDataReader : public vtkObject {
 private:
  static vtkRealityGridDataReader* _instance;

  // steering params
  int loop_number;
  int numParamsChanged;
  int numRecvdCmds;
  int recvdCmds[REG_MAX_NUM_STR_CMDS];
  char** changedParamLabels;
  char** recvdCmdParams;
  int num_io_handles;
  int io_handles[REG_INITIAL_NUM_IOTYPES];
  vtkRealityGridDataSliceCollection* slices[REG_INITIAL_NUM_IOTYPES];

  void* user_data;
  void (*update_callback) (vtkRealityGridDataSliceCollection**, void*);
  vtkRenderWindowInteractor* interactor;

  void InitializeRealityGrid();
  bool PollRealityGrid();
  void FinalizeRealityGrid();

 protected:
  vtkRealityGridDataReader();
  ~vtkRealityGridDataReader();

 public:
  enum TypeIds {
    None = -1,
    Int = REG_INT,
    Float = REG_FLOAT,
    Double = REG_DBL,
    Char = REG_CHAR,
    Bin = REG_BIN,
    Long = REG_LONG
  };

  enum IODir {
    Undef = -1,
    In = REG_IO_IN,
    Out = REG_IO_OUT,
    Both = REG_IO_INOUT
  };

  static vtkRealityGridDataReader* New();
  static vtkRealityGridDataReader* GetInstance();
  vtkTypeRevisionMacro(vtkRealityGridDataReader,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkRealityGridDataSliceCollection* GetDataSlices(int);
  int GetMaxIOChannels();

  void SetUpdateCallback(void (*func)(vtkRealityGridDataSliceCollection** slices, void* user_data)) {
    this->update_callback = func;
  }
  void SetUpdateUserData(void*);

  void SetInteractor(vtkRenderWindowInteractor*);

  void RegisterIOChannel(const char*, vtkRealityGridDataReader::IODir, int);

  friend void _poll(vtkObject*, unsigned long, void*, void*);
};

inline vtkRealityGridDataSliceCollection* vtkRealityGridDataReader::GetDataSlices(int i) {
  return this->slices[i];
}

inline int vtkRealityGridDataReader::GetMaxIOChannels() {
  return REG_INITIAL_NUM_IOTYPES;
}

#define __vtkRealityGridDataReader_h__
#endif // __vtkRealityGridDataReader_h__

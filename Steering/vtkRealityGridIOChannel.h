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

#ifndef __vtkRealityGridIOChannel_h__

#include "vtkSteeringWin32Header.h"

#include "vtkObject.h"

class vtkRealityGridDataReader;
class vtkRealityGridDataSliceCollection;

class REGVTK_STEERING_EXPORT vtkRealityGridIOChannel : public vtkObject {
 private:
  int handle;
  char* name;
  int io_direction;
  vtkRealityGridDataSliceCollection* data_in;
  vtkRealityGridDataSliceCollection* data_out;

  bool RecvData();
  void SendData(int);

 protected:
  vtkRealityGridIOChannel();
  ~vtkRealityGridIOChannel();

 public:
  static vtkRealityGridIOChannel* New();
  vtkTypeRevisionMacro(vtkRealityGridIOChannel,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  void SetHandle(const int);
  int GetHandle();

  void SetName(const char*);
  char* GetName();

  void SetIODirection(const int);
  int GetIODirection();

  vtkRealityGridDataSliceCollection* GetDataSlicesIn();
  vtkRealityGridDataSliceCollection* GetDataSlicesOut();

  void Register(vtkRealityGridDataReader*, int);

  bool Update(int);
};

inline int vtkRealityGridIOChannel::GetHandle() {
  return this->handle;
}

inline char* vtkRealityGridIOChannel::GetName() {
  return this->name;
}

inline int vtkRealityGridIOChannel::GetIODirection() {
  return this->io_direction;
}

inline vtkRealityGridDataSliceCollection* vtkRealityGridIOChannel::GetDataSlicesIn() {
  return this->data_in;
}

inline vtkRealityGridDataSliceCollection* vtkRealityGridIOChannel::GetDataSlicesOut() {
  return this->data_out;
}

#define __vtkRealityGridIOChannel_h__
#endif // __vtkRealityGridIOChannel_h__

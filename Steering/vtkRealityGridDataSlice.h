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

#ifndef __vtkRealityGridDataSlice_h__

#include "vtkSteeringWin32Header.h"

#include "vtkObject.h"

#include "ReG_Steer_types.h"

class REGVTK_STEERING_EXPORT vtkRealityGridDataSlice : public vtkObject {
 protected:
  int size;

  vtkRealityGridDataSlice();
  ~vtkRealityGridDataSlice() {}

 public:
  vtkTypeRevisionMacro(vtkRealityGridDataSlice,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  virtual int GetDataType() = 0;
  int GetDataSize();
  virtual void* GetVoidData() = 0;
  virtual void SetData(void*, int) = 0;

  static vtkRealityGridDataSlice* CreateDataSlice(int);
};

inline int vtkRealityGridDataSlice::GetDataSize() {
  return this->size;
}

#define __vtkRealityGridDataSlice_h__
#endif // __vtkRealityGridDataSlice_h__

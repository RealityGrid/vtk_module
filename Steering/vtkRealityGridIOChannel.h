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

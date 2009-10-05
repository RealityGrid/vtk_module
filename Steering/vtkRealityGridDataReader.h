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

#ifndef __vtkRealityGridDataReader_h__

#include "vtkSteeringWin32Header.h"

#include "vtkObject.h"

#include "ReG_Steer_types.h"

class vtkRenderWindowInteractor;

class vtkRealityGridIOChannel;

void _poll(vtkObject*, unsigned long, void*, void*);

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
  int num_io_channels;
  vtkRealityGridIOChannel* io_channels[REG_INITIAL_NUM_IOTYPES];

  vtkRenderWindowInteractor* interactor;

  void InitializeRealityGrid();
  bool PollRealityGrid();
  void FinalizeRealityGrid();

 protected:
  vtkRealityGridDataReader();
  ~vtkRealityGridDataReader();

 public:
  static vtkRealityGridDataReader* New();
  static vtkRealityGridDataReader* GetInstance();
  vtkTypeRevisionMacro(vtkRealityGridDataReader,vtkObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  vtkRealityGridIOChannel* GetIOChannel(int);
  int GetMaxIOChannels();

  void SetInteractor(vtkRenderWindowInteractor*);

  void RegisterIOChannel(vtkRealityGridIOChannel*, int);

  //BTX
  friend void _poll(vtkObject*, unsigned long, void*, void*);
  //ETX
};

inline vtkRealityGridIOChannel* vtkRealityGridDataReader::GetIOChannel(int i) {
  return this->io_channels[i];
}

inline int vtkRealityGridDataReader::GetMaxIOChannels() {
  return REG_INITIAL_NUM_IOTYPES;
}

#define __vtkRealityGridDataReader_h__
#endif // __vtkRealityGridDataReader_h__

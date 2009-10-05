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

#ifndef __vtkRealityGridDataSliceCollection_h__

#include "vtkSteeringWin32Header.h"
#include "vtkRealityGridDataSlice.h"

#include "vtkCollection.h"

class REGVTK_STEERING_EXPORT vtkRealityGridDataSliceCollection : public vtkCollection {
 private:
  // hide the standard AddItem from the user and the compiler.
  void AddItem(vtkObject* o) { this->vtkCollection::AddItem(o); };

 protected:
  vtkRealityGridDataSliceCollection() {};
  ~vtkRealityGridDataSliceCollection();

 public:
  static vtkRealityGridDataSliceCollection* New();
  vtkTypeRevisionMacro(vtkRealityGridDataSliceCollection,vtkCollection);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Add a data slice to the list.
  void AddItem(vtkRealityGridDataSlice*);

  // Description:
  // Get the data slice at the specific index in the list.
  vtkRealityGridDataSlice* GetDataSlice(int);

  // Description:
  // Get the next data slice in the list.
  vtkRealityGridDataSlice* GetNextDataSlice();

  // Description:
  // Get the last data slice in the list.
  vtkRealityGridDataSlice* GetLastDataSlice();

};

inline void vtkRealityGridDataSliceCollection::AddItem(vtkRealityGridDataSlice* a) {
  this->vtkCollection::AddItem((vtkObject*)a);
}

inline vtkRealityGridDataSlice* vtkRealityGridDataSliceCollection::GetDataSlice(int i) {
  if(i > this->NumberOfItems)
    return NULL;

  return static_cast<vtkRealityGridDataSlice*>(this->GetItemAsObject(i));
}

inline vtkRealityGridDataSlice* vtkRealityGridDataSliceCollection::GetNextDataSlice() { 
  return static_cast<vtkRealityGridDataSlice*>(this->GetNextItemAsObject());
}

inline vtkRealityGridDataSlice* vtkRealityGridDataSliceCollection::GetLastDataSlice() { 
  if(this->Bottom == NULL) {
    return NULL;
  }
  else {
    return static_cast<vtkRealityGridDataSlice*>(this->Bottom->Item);
  }
}

#define __vtkRealityGridDataSliceCollection_h__
#endif // __vtkRealityGridDataSliceCollection_h__

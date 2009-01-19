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

#ifndef __vtkRealityGridDataSliceCollection_h__

#include "vtkRealityGridDataSlice.h"

#include "vtkCollection.h"

class vtkRealityGridDataSliceCollection : public vtkCollection {
 private:
  char* name;
  // hide the standard AddItem from the user and the compiler.
  void AddItem(vtkObject* o) { this->vtkCollection::AddItem(o); };

 protected:
  vtkRealityGridDataSliceCollection() {};
  ~vtkRealityGridDataSliceCollection();

 public:
  static vtkRealityGridDataSliceCollection* New();
  vtkTypeRevisionMacro(vtkRealityGridDataSliceCollection,vtkCollection);

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

  void SetName(const char*);
  char* GetName();
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

inline char* vtkRealityGridDataSliceCollection::GetName() {
  return this->name;
}

#define __vtkRealityGridDataSliceCollection_h__
#endif // __vtkRealityGridDataSliceCollection_h__

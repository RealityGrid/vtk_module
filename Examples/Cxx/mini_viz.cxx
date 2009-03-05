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
#include "vtkRealityGridDataSlice.h"
#include "vtkRealityGridDataSliceCollection.h"
#include "vtkRealityGridIOChannel.h"

#include "vtkActor.h"
#include "vtkCellArray.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"

#include "vtkCallbackCommand.h"
#include "vtkCommand.h"

// struct for passing components to callbacks
struct UserData {
  vtkPolyData* surface;
  vtkTextActor* text;
};

void redrawCallback(vtkObject*, unsigned long, void*, void*);

int main(int argc, char** argv) {
  vtkPolyData* surface = vtkPolyData::New();

  // calculate normals of surface
  vtkPolyDataNormals* norms = vtkPolyDataNormals::New();
  norms->SetInput(surface);
  norms->ConsistencyOn();
  norms->SplittingOff();

  vtkPolyDataMapper* surfaceMapper = vtkPolyDataMapper::New();
  surfaceMapper->SetInputConnection(norms->GetOutputPort());

  vtkActor* surfaceActor = vtkActor::New();
  surfaceActor->SetMapper(surfaceMapper);

  // create frame text label
  vtkTextActor* text = vtkTextActor::New();
  text->SetInput("Data size: 000000");
  text->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  text->GetPositionCoordinate()->SetValue(0.70, 0.95);
  text->GetTextProperty()->SetFontSize(24);
  text->GetTextProperty()->SetFontFamilyToArial();
  text->GetTextProperty()->BoldOn();
  text->GetTextProperty()->ItalicOn();

  // set up renderer
  vtkRenderer* renderer = vtkRenderer::New();
  renderer->AddActor(surfaceActor);
  renderer->AddActor2D(text);
  renderer->SetBackground(0.1, 0.2, 0.4);

  // set up window
  vtkRenderWindow* renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("RealityGrid Data Reader Test");
  renderWindow->SetSize(700, 700);

  // set up interactor and style
  vtkInteractorStyleSwitch* interactorStyle = vtkInteractorStyleSwitch::New();
  interactorStyle->SetCurrentStyleToTrackballCamera();

  vtkRenderWindowInteractor* reni = vtkRenderWindowInteractor::New();
  reni->SetRenderWindow(renderWindow);
  reni->SetInteractorStyle(interactorStyle);
  reni->Initialize();

  // set up ReG reader
  UserData* ud = new UserData();
  ud->surface = surface;
  ud->text = text;

  vtkCallbackCommand* data_callback = vtkCallbackCommand::New();
  data_callback->SetCallback(redrawCallback);
  data_callback->SetClientData((void*) ud);

  vtkRealityGridIOChannel* io_in = vtkRealityGridIOChannel::New();
  io_in->SetName("mini_app in");
  io_in->SetIODirection(REG_IO_IN);
  io_in->AddObserver(vtkCommand::UserEvent, data_callback);

  vtkRealityGridDataReader* reader = vtkRealityGridDataReader::New();
  reader->RegisterIOChannel(io_in, 10);
  reader->SetInteractor(reni);

  reader->Print(std::cout);

  reni->Start();

  reader->Delete();
  delete ud;

  return 0;
}

void redrawCallback(vtkObject* obj, unsigned long eid, void* cd, void* calld) {
  int data_size;
  int num_points, index;
  float* data;
  char label[20];
  vtkPoints* points = vtkPoints::New();
  vtkCellArray* strips = vtkCellArray::New();
  vtkPolyData* surface = ((UserData*) cd)->surface;
  vtkTextActor* text = ((UserData*) cd)->text;
  vtkRealityGridDataSliceCollection* slices;
  vtkRealityGridDataSlice* slice;

  // get the io channel with the new data
  vtkRealityGridIOChannel* ioc = vtkRealityGridIOChannel::SafeDownCast(obj);

  // get data dimensions
  slices = ioc->GetDataSlicesIn();
  slice = slices->GetDataSlice(0);
  data_size = *((int*) slice->GetData());
  num_points = data_size * data_size;

  // get points
  slice = slices->GetDataSlice(1);
  data = (float*) slice->GetData();
  points->Allocate(num_points);
  index = 0;
  for(int j = 0; j < data_size; j++) {
    for(int i = 0; i < data_size; i++) {
      points->InsertNextPoint(i, j, data[index++]);
    }
  }

  // create strips
  for(int i = 1; i < data_size; i++) {
    strips->InsertNextCell(data_size * 2);
    for(int j = 0; j < data_size; j++) {
      strips->InsertCellPoint((data_size * j) + (i - 1));
      strips->InsertCellPoint((data_size * j) + i);
    }
  }

  // build surface
  surface->SetPoints(points);
  surface->SetStrips(strips);

  // update text
  sprintf(label, "Data size: %6d", (data_size * data_size));
  text->SetInput(label);

  // clean up
  points->Delete();
  strips->Delete();
}

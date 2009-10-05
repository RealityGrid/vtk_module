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
  int num_points;
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
  ud->num_points = -1;

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
  int num_points, num_points_old, index;
  float* data;
  char label[20];
  vtkPoints* points;
  vtkPolyData* surface = ((UserData*) cd)->surface;
  vtkTextActor* text = ((UserData*) cd)->text;
  vtkRealityGridDataSliceCollection* slices;
  vtkRealityGridDataSlice* slice;

  // get the io channel with the new data
  vtkRealityGridIOChannel* ioc = vtkRealityGridIOChannel::SafeDownCast(obj);

  // get data dimensions
  slices = ioc->GetDataSlicesIn();
  slice = slices->GetDataSlice(0);
  data_size = *((int*) slice->GetVoidData());
  num_points = data_size * data_size;
  num_points_old = ((UserData*) cd)->num_points;

  slice = slices->GetDataSlice(1);
  data = (float*) slice->GetVoidData();

  // only create a new set of points, re-organise triangle
  // strips and alter text if data size has changed
  if(num_points != num_points_old) {
    // set points
    points = vtkPoints::New();
    points->SetNumberOfPoints(num_points);
    index = 0;
    for(int j = 0; j < data_size; j++) {
      for(int i = 0; i < data_size; i++) {
	points->SetPoint(index, i, j, data[index]);
	index++;
      }
    }

    // create strips
    vtkCellArray* strips = vtkCellArray::New();
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
    sprintf(label, "Data size: %6d", num_points);
    text->SetInput(label);

    // clean up
    points->Delete();
    strips->Delete();

    // save new number of points for next time
    ((UserData*) cd)->num_points = num_points;
  }
  else {
    // set points
    points = surface->GetPoints();
    index = 0;
    for(int j = 0; j < data_size; j++) {
      for(int i = 0; i < data_size; i++) {
	points->SetPoint(index, i, j, data[index]);
	index++;
      }
    }

    points->Modified();
  }
}

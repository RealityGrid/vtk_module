#
#  The RealityGrid Steering Library VTK Data Reader
#
#  Copyright (c) 2002-2009, University of Manchester, United Kingdom.
#  All rights reserved.
#
#  This software is produced by Research Computing Services, University
#  of Manchester as part of the RealityGrid project and associated
#  follow on projects, funded by the EPSRC under grants GR/R67699/01,
#  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
#  EP/F00561X/1.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of The University of Manchester nor the names
#      of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written
#      permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  Author: Robert Haines

from RealityGrid.vtk import *

# redraw callback must be defined before it is registered
def redraw_callback(obj, event):
    surface = obj.surface
    text = obj.text
    slices = obj.GetDataSlicesIn()
    
    slce = slices.GetDataSlice(0)
    slce_data = slce.GetData()
    side_length = slce_data.GetValue(0)
    num_points = side_length * side_length
    num_points_old = io.num_points

    slce = slices.GetDataSlice(1)
    slce_data = slce.GetData()

    # only rebuild surface if number of points has changed
    if num_points != num_points_old:
        # create points
        points = vtkPoints()
        points.SetNumberOfPoints(num_points)
        index = 0
        for j in xrange(side_length):
            for i in xrange(side_length):
                points.SetPoint(index, i, j, slce_data.GetValue(index))
                index += 1
                
        # create triangle strips
        strips = vtkCellArray()
        for i in xrange(1, side_length):
            strips.InsertNextCell(side_length * 2)
            for j in xrange(side_length):
                strips.InsertCellPoint((side_length * j) + (i - 1))
                strips.InsertCellPoint((side_length * j) + i)

        surface.SetPoints(points)
        surface.SetStrips(strips)

        text.SetInput('Data size: %6d' % num_points)

        io.num_points = num_points
    else:
        # adjust point positions
        points = surface.GetPoints()
        index = 0
        for j in xrange(side_length):
            for i in xrange(side_length):
                points.SetPoint(index, i, j, slce_data.GetValue(index))
                index += 1

        points.Modified()


# script entry point
surface = vtkPolyData()

normals = vtkPolyDataNormals()
normals.SetInput(surface)
normals.ConsistencyOn();
normals.SplittingOff();

surface_map = vtkPolyDataMapper()
surface_map.SetInputConnection(normals.GetOutputPort())

surface_actor = vtkActor()
surface_actor.SetMapper(surface_map)

# create frame text label
text = vtkTextActor()
text.SetInput("Data size: 000000")
text.GetPositionCoordinate().SetCoordinateSystemToNormalizedViewport()
text.GetPositionCoordinate().SetValue(0.70, 0.95)
text.GetTextProperty().SetFontSize(24)
text.GetTextProperty().SetFontFamilyToArial()
text.GetTextProperty().BoldOn()
text.GetTextProperty().ItalicOn()

# set up renderer
renderer = vtkRenderer()
renderer.AddActor(surface_actor)
renderer.AddActor(text)
renderer.SetBackground(0.1, 0.2, 0.4)

window = vtkRenderWindow()
window.AddRenderer(renderer)
window.SetWindowName("RealityGrid Python Data Reader Test")
window.SetSize(700, 700)

# set up interactor and style
istyle = vtkInteractorStyleSwitch()
istyle.SetCurrentStyleToTrackballCamera()

reni = vtkRenderWindowInteractor()
reni.SetRenderWindow(window)
reni.SetInteractorStyle(istyle)
reni.Initialize()

io = vtkRealityGridIOChannel()
io.SetName("mini_app python in")
io.SetIODirection(0)
io.AddObserver("UserEvent", redraw_callback)
io.surface = surface
io.text = text
io.num_points = -1

dr = vtkRealityGridDataReader()
dr.RegisterIOChannel(io, 10)
dr.SetInteractor(reni)

# start the interaction loop
reni.Start()

print 'Exiting...'


#
#  (C) Copyright 2009, University of Manchester, United Kingdom,
#  all rights reserved.
#
#  This software is produced by the Supercomputing, Visualization and
#  e-Science Group, Manchester Computing, University of Manchester
#  as part of the RealityGrid project (http://www.realitygrid.org),
#  funded by the EPSRC under grants GR/R67699/01 and GR/R67699/02.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#  1. Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#  2. Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED  TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
#  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
#  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
#  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
#  CORRECTION.
#
#  Author.........: Robert Haines
#----------------------------------------------------------------------

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


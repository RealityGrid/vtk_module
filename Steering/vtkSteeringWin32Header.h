#ifndef __vtkSteeringWin32Header_h
#define __vtkSteeringWin32Header_h

#include <vtkRealityGridConfigure.h>

#if defined(WIN32) && !defined(REGVTK_STATIC)
#if defined(vtkSteering_EXPORTS)
#define REGVTK_STEERING_EXPORT __declspec( dllexport ) 
#else
#define REGVTK_STEERING_EXPORT __declspec( dllimport ) 
#endif
#else
#define REGVTK_STEERING_EXPORT
#endif

#endif

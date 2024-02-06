/**
 * @brief this file works for generating double T cells in vtk
 * @author zwx3991@gmail.com
 * @date 2023-11-27
 */
#include <vtkAutoInit.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkNamedColors.h>
#include <vtkPolyDataMapper.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTriangleFilter.h>
#include <vtkVectorText.h>
#include <vtkStripper.h>

int main(int, char *[])
{
  vtkNew<vtkNamedColors> colors;
  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;

  // create a vtk object and insert some points
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0, 0, 0); //  (x, y, z)
  points->InsertNextPoint(0, 1, 0);
  points->InsertNextPoint(3, 1, 0);
  points->InsertNextPoint(3, 0, 0);
  points->InsertNextPoint(2, 0, 0);
  points->InsertNextPoint(2, -1, 0);
  points->InsertNextPoint(3, -1, 0);
  points->InsertNextPoint(3, -2, 0);
  points->InsertNextPoint(0, -2, 0);
  points->InsertNextPoint(0, -1, 0);
  points->InsertNextPoint(1, -1, 0);
  points->InsertNextPoint(1, 0, 0);


// Create a vtkCellArray object to define polygons
vtkNew<vtkCellArray> polygons;

// Create a vtkIdList object containing indices of polygon vertices
vtkNew<vtkIdList> pointIds;
pointIds->SetNumberOfIds(12);

// Set indices of vertices for the polygon
for (int i = 12; i > 0; i--)
{
  pointIds->SetId(i, i);
}

// Add the polygon to the vtkCellArray
polygons->InsertNextCell(pointIds);

// Create a vtkPolyData object
vtkNew<vtkPolyData> polyData;

// Add vtkPoints and vtkCellArray to vtkPolyData
polyData->SetPoints(points);
polyData->SetPolys(polygons);


  //Apply linear extrusion
  vtkNew<vtkLinearExtrusionFilter> extrude;
  extrude->SetInputData(polyData);
  extrude->SetExtrusionTypeToNormalExtrusion();
  extrude->SetVector(0, 0, 10);
  extrude->SetScaleFactor(0.5);
  
  //required for non-convex shape
  vtkNew<vtkTriangleFilter> triangleFilter;
  triangleFilter->SetInputConnection(extrude->GetOutputPort());

  /*if we would like to link extrusion to mapper,
  * we have to use vtkDataSetMapper instead of vtkPolyDataMapper
  */ 
  vtkNew<vtkDataSetMapper> mapper; 
  mapper->SetInputConnection(triangleFilter->GetOutputPort());

  /*Functions for triangulate the polydata without extrusion*/
  // vtkNew<vtkTriangleFilter> triangleFilter;
  // triangleFilter->SetInputData(polyData);
  // triangleFilter->Update();
  // vtkNew<vtkPolyDataMapper> mapper;
  // mapper->SetInputData(triangleFilter->GetOutput());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  //actor->GetProperty()->SetRepresentationToWireframe();
  actor->GetProperty()->SetColor(colors->GetColor3d("PaleGoldenrod").GetData());

  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("DoubleTCell");
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

  renderWindow->AddRenderer(renderer);
  renderer->AddActor(actor);

  renderer->ResetCamera();
  // Generate an interesting view
  renderer->ResetCamera();
  renderer->GetActiveCamera()->Azimuth(30);
  renderer->GetActiveCamera()->Elevation(30);
  renderer->GetActiveCamera()->Dolly(1.0);
  renderer->ResetCameraClippingRange();

  // Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}
/**
 * @brief this file works for generating double T cells in vtk
 * @author zwx3991@gmail.com
 * @date 2023-11-08
 */
#include <vtkAutoInit.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkLinearExtrusionFilter.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkTriangleFilter.h>
#include <vtkVectorText.h>

int main(int, char *[])
{
  vtkNew<vtkNamedColors> colors;
  // Create a renderer, render window, and interactor
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("DoubleTCell");
  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());

  // 创建一个vtkPoints对象，并添加一些点
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0, 0, 0); // 添加点的坐标 (x, y, z)
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


  // 创建一个vtkCellArray对象来定义多边形
  vtkNew<vtkCellArray> polygons;

  // 创建一个包含多边形顶点索引的vtkIdList对象
  vtkNew<vtkIdList> pointIds;
  pointIds->SetNumberOfIds(12); 

  // 设置多边形的顶点索引
  for (int i = 0; i < 12; i++)
  {
    pointIds->SetId(i, i);
  }

  // 将多边形添加到vtkCellArray中
  polygons->InsertNextCell(pointIds);

  // 创建一个vtkPolyData对象
  vtkNew<vtkPolyData> polyData;

  // 将vtkPoints和vtkCellArray添加到vtkPolyData中
  polyData->SetPoints(points);
  polyData->SetPolys(polygons);

  // Apply linear extrusion
  vtkNew<vtkLinearExtrusionFilter> extrude;
  extrude->SetInputData(polyData);
  extrude->SetExtrusionTypeToPointExtrusion();
  extrude->SetVector(0, 0, 10);
  extrude->SetScaleFactor(0.5);

  vtkNew<vtkTriangleFilter> triangleFilter;//
  triangleFilter->SetInputConnection(extrude->GetOutputPort());

  vtkNew<vtkDataSetMapper> mapper;
  mapper->SetInputConnection(triangleFilter->GetOutputPort());

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("PaleGoldenrod").GetData());

  renderer->SetBackground(colors->GetColor3d("SteelBlue").GetData());

  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("LinearExtrusion");

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
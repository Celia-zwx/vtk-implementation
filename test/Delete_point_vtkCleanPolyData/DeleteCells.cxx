#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>


#include <vtkStripper.h>
#include <vtkCleanPolyData.h>
#include <vtkFeatureEdges.h>
void OutputPoints(vtkSmartPointer<vtkPoints> points)
{
  for (vtkIdType i = 0; i < points->GetNumberOfPoints(); i++)
  {
    double p[3];
    points->GetPoint(i, p);
    cout << p[0] << " " << p[1] << " " << p[2] << endl;
  }
}

int main(int, char*[])
{
  vtkNew<vtkPoints> points;
  points->InsertNextPoint(0, 0, 0);
  points->InsertNextPoint(1, 0, 0);
  points->InsertNextPoint(1, 0.5, 0);
  points->InsertNextPoint(0, 1, 0);

  vtkNew<vtkLine> line0;
  line0->GetPointIds()->SetId(0, 0);
  line0->GetPointIds()->SetId(1, 1);

  vtkNew<vtkLine> line1;
  line1->GetPointIds()->SetId(0, 1);
  line1->GetPointIds()->SetId(1, 2);

  vtkNew<vtkLine> line2;
  line2->GetPointIds()->SetId(0, 2);
  line2->GetPointIds()->SetId(1, 3);

  vtkNew<vtkCellArray> lines;
  lines->InsertNextCell(line0);
  lines->InsertNextCell(line1);
  lines->InsertNextCell(line2);

  vtkNew<vtkPolyData> polydata;
  polydata->SetPoints(points);
  polydata->SetLines(lines);

  // Tell the polydata to build 'upward' links from points to cells.
  polydata->BuildLinks();
  std::cout << "Number of points: " << polydata->GetPoints()->GetNumberOfPoints() << std::endl;
  OutputPoints(polydata->GetPoints());
  // Mark a cell as deleted.
   polydata->DeleteCell(0);
  // // Remove the marked cell.
  polydata->RemoveDeletedCells();
  //points->Reset();
  // polydata->DeletePoint(0);
  polydata->Modified();
  vtkSmartPointer<vtkPolyData> polydata_clean;
  // Operations are performed to clean points from polydata
  vtkNew<vtkCleanPolyData> clean;
  clean->SetInputData(polydata);
  clean->SetTolerance(0.0);
  clean->PointMergingOn();
  vtkNew<vtkStripper> stripper;
  stripper->SetInputConnection(clean->GetOutputPort());
  stripper->Update();
  polydata_clean = stripper->GetOutput();
  std::cout << "Number of points: " << polydata_clean->GetPoints()->GetNumberOfPoints() << std::endl;
  OutputPoints(polydata_clean->GetPoints());

  // Output of not clean points
  // polydata_clean = polydata;
  // std::cout << "Number of points: " << polydata_clean->GetPoints()->GetNumberOfPoints() << std::endl;
  // OutputPoints(polydata_clean->GetPoints());


  // Visualize
  vtkNew<vtkNamedColors> colors;

  vtkNew<vtkPolyDataMapper> mapper;
  mapper->SetInputDataObject(polydata_clean);

  vtkNew<vtkActor> actor;
  actor->SetMapper(mapper);
  actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
  actor->GetProperty()->SetLineWidth(4);

  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("DeleteCells");

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);
  renderer->AddActor(actor);
  renderer->SetBackground(colors->GetColor3d("Silver").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

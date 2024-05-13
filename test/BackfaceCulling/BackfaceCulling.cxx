#include <vtkActor.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>

#include <vtkPlane.h>
#include <vtkTableBasedClipDataSet.h>
#include <vtkDataSetMapper.h>

int main(int, char*[])
{
  vtkNew<vtkNamedColors> colors;

  // Sphere
  vtkNew<vtkSphereSource> sphereSource;

  sphereSource->SetPhiResolution(30);
  sphereSource->SetThetaResolution(30);

  sphereSource->Update();

  vtkNew<vtkPolyData> polyData;
  polyData->DeepCopy(sphereSource->GetOutput()); 
  double center[3];
  polyData->GetCenter(center);
  //center[3] = {0.0,0.0,0.0};
  vtkNew<vtkPlane> plane;
  plane->SetOrigin(center);
  plane->SetNormal(0.0, 0.0, -1.0);

  // vtkNew<vtkPolyDataMapper> mapper;
  // mapper->SetInputConnection(sphereSource->GetOutputPort());

  // vtkNew<vtkActor> actor;
  // actor->SetMapper(mapper);
  // actor->GetProperty()->SetColor(colors->GetColor3d("Peacock").GetData());
    vtkNew<vtkTableBasedClipDataSet> clipper;
    clipper->SetInputDataObject(polyData);
    clipper->SetValue(0);
    clipper->SetClipFunction(plane);
    clipper->SetInsideOut(true);
    clipper->Update();

    vtkNew<vtkDataSetMapper> clipMapper;
    clipMapper->SetInputConnection(clipper->GetOutputPort());

    vtkNew<vtkActor> clipActor;
    clipActor->SetMapper(clipMapper);
    clipActor->GetProperty()->SetColor(1.0000, 0.3882, 0.2784);
    //clipActor->GetProperty()->SetInterpolationToPBR();
    clipActor->GetProperty()->SetEdgeVisibility(true);
    clipActor->GetProperty()->SetColor(1, 1, 1);

  //vtkNew<vtkProperty> backProperty; // opengl
  //backProperty->SetColor(colors->GetColor3d("Gold").GetData());
  //clipActor->SetBackfaceProperty(backProperty);
  // Since back face culling is on, when you zoom into the sphere, you won't see
  // the gold colored backface.
  clipActor->GetProperty()->BackfaceCullingOn();
   clipActor->GetProperty()->FrontfaceCullingOn();

  // Visualize
  vtkNew<vtkRenderer> renderer;
  vtkNew<vtkRenderWindow> renderWindow;
  renderWindow->AddRenderer(renderer);
  renderWindow->SetWindowName("BackfaceCulling");

  vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
  renderWindowInteractor->SetRenderWindow(renderWindow);

  renderer->AddActor(clipActor);
  renderer->SetBackground(colors->GetColor3d("MistyRose").GetData());

  renderWindow->Render();
  renderWindowInteractor->Start();

  return EXIT_SUCCESS;
}

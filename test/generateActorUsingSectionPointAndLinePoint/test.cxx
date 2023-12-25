#include <vtkActor.h>
#include <vtkAppendPolyData.h>
#include <vtkCamera.h>
#include <vtkCleanPolyData.h>
#include <vtkDiskSource.h>
#include <vtkFeatureEdges.h>
#include <vtkFrenetSerretFrame.h>
#include <vtkLineSource.h>
#include <vtkMath.h>
#include <vtkMatrix4x4.h>
#include <vtkNew.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricSpline.h>
#include <vtkPointData.h>
#include <vtkPointSource.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkRuledSurfaceFilter.h>
#include <vtkSmartPointer.h>
#include <vtkStripper.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include <vtkBYUReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>
#include <vtkPolyDataReader.h>
#include <vtkSTLReader.h>
#include <vtkXMLPolyDataReader.h>

#include <vtksys/SystemTools.hxx>

#include "GenerateActorUsingSectionPointAndLinePoint.h"
int main()
{
    // Initialize VTK components (render window, renderer, interactor, etc.)
    vtkNew<vtkRenderer> renderer;
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->AddRenderer(renderer);
    vtkNew<vtkRenderWindowInteractor> interactor;
    interactor->SetRenderWindow(renderWindow);
    renderWindow->SetSize(512, 512);
    renderer->SetBackground(.4, .5, .7);

    // Generate test data
    std::vector<std::array<double, 3>> sectionPoints = {
        {{0.0, 0.0, 0.0}},
        {{10.0, 0.0, 0.0}},
        {{8.0, 1.0, 0.0}},
        {{0.0, 10.0, 0.0}},
        {{0.0, 5, 0.0}}};
    std::vector<std::array<double, 3>> linePoints = {
        {{0.0, 0.0, 0.0}},
        {{1.0, 0.0, 0.0}},
        {{1.0, 1.0, 0.0}},
        {{0.0, 1.0, 0.0}}};

    vtkSmartPointer<vtkActor> generatedActor = generateActorUsingSectionPointAndLinePoint(sectionPoints, linePoints);
    renderer->AddActor(generatedActor);
    renderer->ResetCamera();
    renderWindow->Render();
    interactor->Start();

    return 0;
}
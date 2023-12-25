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


//vtkStandardNewMacro(generateActorUsingSectionPointAndLinePoint)

vtkSmartPointer<vtkActor> generateActorUsingSectionPointAndLinePoint(std::vector<std::array<double, 3>> SectionPoint,
                                                                     std::vector<std::array<double, 3>> LinePoint)
{
    vtkSmartPointer<vtkPolyData> polyData;
    // Create a vtkPoints object from SectionPoint and LinePoint
    vtkNew<vtkPoints> points_line; // 线
    vtkNew<vtkPoints> points_base; // 横截面
    for (size_t i = 0; i < SectionPoint.size(); ++i)
    {
        points_base->InsertNextPoint(SectionPoint[i].data());
    }
    for (size_t i = 0; i < LinePoint.size(); ++i)
    {
        points_line->InsertNextPoint(LinePoint[i].data());
    }
    // Create a vtkCellArray object to define polygons
    vtkNew<vtkCellArray> polygons;

    // Create a vtkIdList object containing indices of polygon vertices
    vtkNew<vtkIdList> pointIds;
    pointIds->SetNumberOfIds(SectionPoint.size());

    // Set indices of vertices for the polygon
    for (int i = SectionPoint.size(); i > 0; i--)
    {
        pointIds->SetId(i, i);
    }

    // Add the polygon to the vtkCellArray
    polygons->InsertNextCell(pointIds);

    // Create a vtkPolyData object
    vtkNew<vtkPolyData> basePolyData;

    // Add vtkPoints and vtkCellArray to vtkPolyData
    basePolyData->SetPoints(points_base);
    basePolyData->SetPolys(polygons);

    // Operations are performed to clean and extract edges from the base polydata
    vtkNew<vtkCleanPolyData> clean;
    clean->SetInputData(basePolyData);
    vtkNew<vtkFeatureEdges> edges;
    edges->SetInputConnection(clean->GetOutputPort());
    edges->NonManifoldEdgesOff();
    edges->ManifoldEdgesOff();
    edges->BoundaryEdgesOn();
    edges->FeatureEdgesOff();
    vtkNew<vtkStripper> stripper;
    stripper->SetInputConnection(edges->GetOutputPort());
    stripper->Update();
    polyData = stripper->GetOutput();

    int numberOfContours = polyData->GetNumberOfLines();
    // Specify points for spline
    int numberOfPoints = LinePoint.size();
    vtkNew<vtkParametricSpline> spline;
    spline->SetPoints(points_line);

    // Set the precision and smoothness by setting the resolution on uvw coordinates
    vtkNew<vtkParametricFunctionSource> functionSource; // 参数方程
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(50 * numberOfPoints);
    functionSource->SetVResolution(50 * numberOfPoints);
    functionSource->SetWResolution(50 * numberOfPoints);

    // Create the frame
    vtkNew<vtkFrenetSerretFrame> frame;
    frame->SetInputConnection(functionSource->GetOutputPort());
    frame->ConsistentNormalsOn();
    frame->Update();

    // Compute and extracts the Frenet-Serret frame properties—normals, tangents, and binormals—from the given parametric function source
    frame->GetOutput()->GetPointData()->SetActiveVectors("FSNormals");
    frame->GetOutput()->GetPointData()->SetActiveVectors("FSTangents");
    frame->GetOutput()->GetPointData()->SetActiveVectors("FSBinormals");

    vtkPoints *linePoints = frame->GetOutput()->GetPoints();

    std::vector<vtkSmartPointer<vtkAppendPolyData>> skeletons;
    for (int i = 0; i < numberOfContours; ++i)
    {
        skeletons.push_back(vtkSmartPointer<vtkAppendPolyData>::New());
    }

    for (int i = 0; i < linePoints->GetNumberOfPoints(); ++i)
    {
        vtkNew<vtkTransform> transform;

        // Compute a basis
        double normalizedX[3];
        frame->GetOutput()->GetPointData()->SetActiveVectors("FSNormals");
        frame->GetOutput()->GetPointData()->GetVectors()->GetTuple(i, normalizedX);
        double normalizedY[3];
        frame->GetOutput()->GetPointData()->SetActiveVectors("FSBinormals");
        frame->GetOutput()->GetPointData()->GetVectors()->GetTuple(i, normalizedY);
        double normalizedZ[3];
        frame->GetOutput()->GetPointData()->SetActiveVectors("FSTangents");
        frame->GetOutput()->GetPointData()->GetVectors()->GetTuple(i, normalizedZ);

        // Create the direction cosine matrix
        vtkNew<vtkMatrix4x4> matrix;
        matrix->Identity();
        for (unsigned int j = 0; j < 3; ++j)
        {
            matrix->SetElement(j, 0, normalizedX[j]);
            matrix->SetElement(j, 1, normalizedY[j]);
            matrix->SetElement(j, 2, normalizedZ[j]);
        }

        transform->Translate(linePoints->GetPoint(i)[0], linePoints->GetPoint(i)[1],
                             linePoints->GetPoint(i)[2]);
        transform->Scale(.02, .02, .02);
        transform->Concatenate(matrix);

        vtkNew<vtkTransformPolyDataFilter> transformPD;
        vtkNew<vtkPolyData> polyDataCopy;
        polyDataCopy->DeepCopy(polyData);

        transformPD->SetTransform(transform);
        transformPD->SetInputData(polyDataCopy);
        transformPD->Update();

        vtkNew<vtkPolyDataConnectivityFilter> contours;
        contours->SetInputConnection(transformPD->GetOutputPort());
        contours->Update();
        for (int r = 0; r < contours->GetNumberOfExtractedRegions(); ++r)
        {
            contours->SetExtractionModeToSpecifiedRegions();
            contours->InitializeSpecifiedRegionList();
            contours->AddSpecifiedRegion(r);
            contours->Update();
            vtkNew<vtkPolyData> skeleton;
            skeleton->DeepCopy(contours->GetOutput());
            skeletons[r]->AddInputData(skeleton);
        }
    }

    vtkNew<vtkRuledSurfaceFilter> ruled;
    ruled->SetRuledModeToPointWalk(); // 以点顺序遍历
    ruled->CloseSurfaceOff();
    ruled->SetOnRatio(1);
    ruled->SetDistanceFactor(10000000);
    ruled->SetInputConnection(skeletons[0]->GetOutputPort());

    vtkNew<vtkPolyDataNormals> normals;
    normals->SetInputConnection(ruled->GetOutputPort());
    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(normals->GetOutputPort());
    vtkNew<vtkProperty> backProp;
    backProp->SetColor(1.0, 0.3882, 0.278);
    vtkNew<vtkActor> actor;
    actor->SetBackfaceProperty(backProp);
    actor->GetProperty()->SetColor(0.8900, 0.8100, 0.3400);
    actor->SetMapper(mapper);
    return actor;
}

// int main()
// {
//     // Initialize VTK components (render window, renderer, interactor, etc.)
//     vtkNew<vtkRenderer> renderer;
//     vtkNew<vtkRenderWindow> renderWindow;
//     renderWindow->AddRenderer(renderer);
//     vtkNew<vtkRenderWindowInteractor> interactor;
//     interactor->SetRenderWindow(renderWindow);
//     renderWindow->SetSize(512, 512);
//     renderer->SetBackground(.4, .5, .7);

//     // Generate test data (replace this with your actual data)
//     std::vector<std::array<double, 3>> sectionPoints = {
//         {{0.0, 0.0, 0.0}},
//         {{1.0, 0.0, 0.0}},
//         {{1.0, 1.0, 0.0}},
//         {{0.0, 1.0, 0.0}}};
//     std::vector<std::array<double, 3>> linePoints = {
//         {{0.0, 0.0, 0.0}},
//         {{1.0, 0.0, 0.0}},
//         {{1.0, 1.0, 0.0}},
//         {{0.0, 1.0, 0.0}}};

//     // Call your function to generate the actor
//     vtkSmartPointer<vtkActor> generatedActor = generateActorUsingSectionPointAndLinePoint(sectionPoints, linePoints);

//     // Add the generated actor to the renderer
//     renderer->AddActor(generatedActor);

//     // Set up the camera and render the scene
//     renderer->ResetCamera();
//     renderWindow->Render();

//     // Start the interaction
//     interactor->Start();

//     return 0;
// }

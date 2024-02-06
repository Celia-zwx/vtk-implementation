/**
 * @file 如何移动axis actor的位置到左下角
 */
#include <vtkActor.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCaptionActor2D.h>
#include <vtkCommand.h>
#include <vtkCallbackCommand.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkTextProperty.h>
#include <vtkTransform.h>
#include <vtkTextActor.h>
#include <vtkOrientationMarkerWidget.h>

vtkSmartPointer<vtkOrientationMarkerWidget> axisWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
long pressCounts = 0;
void MyCallbackFunc(vtkObject*, unsigned long eid, void* clientdata, void *calldata)
{
	std::cout<<"You have clicked: "<<++pressCounts<<" times."<<std::endl;
}
void RenderWindowSizeCallback(vtkObject* caller, long unsigned int eid, void* clientData, void* callData)
{
    auto iren = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
    vtkRenderWindow* renderWindow = iren->GetRenderWindow();
    int width = 0;
    int height = 0;
    if (renderWindow)
    {
        // Now you can work with the render window or its properties
        int* renderWindowSize = renderWindow->GetSize();
        width = renderWindowSize[0];
        height = renderWindowSize[1];
        std::cout << "Render Window Size: " << width << " x " << height << std::endl;
    }else{
        std::cout << "Render Window does not exist" << std::endl;
    }
    // 计算新的相对位置，原窗口设定为300，300，且要保持在左下角20% * 20%
    int old_width = 300 * (0.2 -0);
    int old_height = 300 * (0.2 -0);
    double new_ymax = static_cast<double>(old_width) / 300;

    // Calculate the desired size of the widget
    int desiredWidgetWidth = 300 * (0.2 -0); // Set your desired width
    int desiredWidgetHeight = 300 * (0.2 -0); // Set your desired height

    // Calculate the normalized viewport dimensions based on desired widget size and rendering window size
    double viewportLeft = 0; // Adjust as needed
    double viewportBottom =0; // Adjust as needed
    double viewportRight = viewportLeft + static_cast<double>(desiredWidgetWidth) / renderWindow->GetSize()[0];
    double viewportTop = viewportBottom + static_cast<double>(desiredWidgetHeight) / renderWindow->GetSize()[1];

    // Set the viewport for the render window
    axisWidget->SetViewport(viewportLeft, viewportBottom, viewportRight, viewportTop);

}


int main(int, char*[])
{
    std::cout<<"out"<<std::endl;
    // ————————————————————————————————初始设置操作
    vtkNew<vtkNamedColors> colors;
    // a renderer and render window
    vtkNew<vtkRenderer> renderer;
    renderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
    vtkNew<vtkRenderWindow> renderWindow;
    renderWindow->SetWindowName("Axes");
    renderWindow->AddRenderer(renderer);
    renderWindow->SetSize(300, 300);
    // an interactor
    vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // ————————————————————————————————图显开始
    // add the actors to the scene
    vtkSmartPointer<vtkAxesActor> axis = vtkSmartPointer<vtkAxesActor>::New();
    axis->GetXAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axis->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axis->GetYAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axis->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);
    axis->GetZAxisCaptionActor2D()->GetTextActor()->SetTextScaleModeToNone();
    axis->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetFontSize(10);

    axis->SetShaftTypeToCylinder();
    axis->SetCylinderRadius(0.5 * axis->GetCylinderRadius());
    // renderer->AddActor(axis);


    axisWidget->SetOrientationMarker(axis);
    axisWidget->SetInteractor(renderWindow->GetInteractor());
    axisWidget->SetEnabled(true); // 显示与否
    axisWidget->SetInteractive(false); // 不允许拖动
    axisWidget->SetViewport(0, 0, 0.2, 0.2); // 相对位置 Coordinates are expressed as (xmin,ymin,xmax,ymax)


    vtkSmartPointer<vtkCallbackCommand> mouseCallback = 
		vtkSmartPointer<vtkCallbackCommand>::New();
	mouseCallback->SetCallback ( MyCallbackFunc );
    renderWindowInteractor->AddObserver(vtkCommand::LeftButtonPressEvent, mouseCallback);

    // Create a callback for the window frame event
    vtkSmartPointer<vtkCallbackCommand> windowFrameCallback = vtkSmartPointer<vtkCallbackCommand>::New();
    windowFrameCallback->SetCallback(RenderWindowSizeCallback);
    renderWindowInteractor->AddObserver(vtkCommand::ConfigureEvent, windowFrameCallback);

    
    // 问题：绝对位置的接口如何设置？？？

    


    // ————————————————————————————————渲染开始
    renderer->ResetCamera();
    renderWindow->Render();
    renderWindowInteractor->Start();
    return EXIT_SUCCESS;
}
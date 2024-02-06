#include <vtkActor.h>
#include <vtkSmartPointer.h>
vtkSmartPointer<vtkActor> generateActorUsingSectionPointAndLinePoint(std::vector<std::array<double, 3>> SectionPoint,
                                                                     std::vector<std::array<double, 3>> LinePoint);
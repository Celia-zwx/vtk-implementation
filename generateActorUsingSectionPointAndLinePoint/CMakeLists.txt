
cmake_minimum_required(VERSION 3.12 FATAL_ERROR)

project(ExtrudePolyDataAlongLine)

find_package(VTK COMPONENTS 
  CommonComputationalGeometry
  CommonCore
  CommonDataModel
  CommonMath
  CommonTransforms
  FiltersCore
  FiltersGeneral
  FiltersModeling
  FiltersSources
  IOGeometry
  IOLegacy
  IOPLY
  IOXML
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

if (NOT VTK_FOUND)
  message(FATAL_ERROR "GenerateActorUsingSectionPointAndLinePoint: Unable to find the VTK build folder.")
endif()
link_directories( ${PROJECT_SOURCE_DIR}/build)
# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
set(Module_SplineDrivenImageSlicer "ON")
add_executable(GenerateActorUsingSectionPointAndLinePoint MACOSX_BUNDLE GenerateActorUsingSectionPointAndLinePoint.cxx GenerateActorUsingSectionPointAndLinePoint.h test.cxx )
  target_link_libraries(GenerateActorUsingSectionPointAndLinePoint PRIVATE ${VTK_LIBRARIES} SplineDrivenImageSlicer
)
# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS GenerateActorUsingSectionPointAndLinePoint
  MODULES ${VTK_LIBRARIES}
)


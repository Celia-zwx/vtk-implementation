# "no such file or directory","vtk module not found" error

### 1.Issue:

###### In the compiling process, some vtk programs will generate "vtk_module_not _found" error

![282387beb20e99f7236e4999e7f6348](D:\vtk-implementation\study_materieal\vtk_module_not_found.assets\282387beb20e99f7236e4999e7f6348.png)

### 2. Reason

###### 咱们这个例子用到了VTK需要的外部库，但默认清空下VTK编译时没有编译这个外部库，但VTK提供了是否编译的选项，所以在用户(我们)要跑这个例子时就通过VTK官方提供的编译选项把这个外部库编译进来，编译选项就是-DVTK_MODULE_ENABLE_VTK_SplineDrivenImageSlicer:STRING=WANT，而设置编译选项的方法有：1：命令行编译时直接写这个变量；cmake_gui编译时在对话框中选中这个变量

### 3. Solution in command line

##### 1.in CmakeLists.txt:

previous:

```
add_executable(GenerateActorUsingSectionPointAndLinePoint MACOSX_BUNDLE GenerateActorUsingSectionPointAndLinePoint.cxx GenerateActorUsingSectionPointAndLinePoint.h test.cxx )
  target_link_libraries(GenerateActorUsingSectionPointAndLinePoint PRIVATE ${VTK_LIBRARIES} 
)
```

now:

```
add_executable(GenerateActorUsingSectionPointAndLinePoint MACOSX_BUNDLE GenerateActorUsingSectionPointAndLinePoint.cxx GenerateActorUsingSectionPointAndLinePoint.h test.cxx )
  target_link_libraries(GenerateActorUsingSectionPointAndLinePoint PRIVATE ${VTK_LIBRARIES} SplineDrivenImageSlicer # 这里决定了target的链接顺序，及符号表的依赖位置
)
```

Since SplineDrivenImageSlicer is a remote directory, we have to download SplineDrivenImageSlicer.dll into ${PATH}

then add

```
link_directories( ${PATH}/)
```

##### 2. Compile



```
cmake .. -DBUILD_SHARED_LIBS=ON -DVTK_VERSIONED_INSTALL=OFF -DVTK_INSTALL_SDK=ON -DVTK_GROUP_ENABLE_Qt=YES -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles" -DCMAKE_CXX_FLAGS=-fcommon -DCMAKE_C_FLAGS=-fcommon -DCMAKE_INSTALL_PREFIX=/tmp/zzy_temp -DVTK_MODULE_ENABLE_VTK_SplineDrivenImageSlicer:STRING=WANT
```

```
 cmake --build build && cmake --build build --target install

```


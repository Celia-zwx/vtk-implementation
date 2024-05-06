# VTK动画实现方式



## Animation

##### 1.实现方法（Method)：

	1. vtkCallbackCommand：vtkTimerEvent
	1. vtkAnimationScene, vtkAnimationCue

##### 2. 方案一：vtkCallbackCommand: vtkTimerEvent

###### 实现原理：

​	重写callback中的判断条件vtkCommand::TimerEvent == eventId来实现每帧的变化

###### 核心代码：

```
  // Initialize must be called prior to creating timer events.
  renderWindowInteractor->Initialize();
  // Sign up to receive TimerEvent.
  vtkNew<vtkTimerCallback> cb;
  interactor->AddObserver(vtkCommand::TimerEvent, cb);
  int timerId = interactor->CreateRepeatingTimer(500);//根据时间间隔0.5秒生成timerId
```

###### 完整代码：

​	https://examples.vtk.org/site/Cxx/Utilities/Animation/

###### 注意事项：

1. vtkTimerEvent: 必须首先initialize interacor: https://discourse.vtk.org/t/why-vtkcommand-timerevent-not-work/10113/3

   

##### 3. 方案二：vtkAnimationScene,vtkAnimationCue

###### 1.vtk 动画场景管理器定义：

​	   vtkAnimationCue和VTK vtkAnimationScene提供框架来支持动画。

- ​    vtkAnimationCue代表一个随时间变化/动画的实体

- ​    vtkAnimationScene代表动画的场景或设置，它由单个线索或其他场景组成。

     ( vtkAnimationCue represents an entity that changes/ animates with time, while vtkAnimationScene represents scene or setup for the animation, which consists of individual cues or other scenes.)

- 一个场景可以在实时模式下播放，或者作为帧1/帧率分开的时间序列。（）
- 一个提示有三种状态：未初始化、活动和非活动。  

​	（1）未初始化：表示在提示开始时间之前的某个时间点。

​	（2）活动状态：在提示的开始时间和结束时间之间的某个时间点，提示处于活动状态。

​	（3）非活动状态：而在提示结束时间之后，提示处于非活动状态。

###### 2.事件（event）

- StartAnimationCueEvent事件：当提示（cue）进入活动状态时，将触发StartAnimationCueEvent事件。可以处理此事件来初始化要进行动画处理的实体。


- EndAnimationCueEvent事件：当提示离开活动状态时，将触发EndAnimationCueEvent事件，可以处理此事件来在运行动画后进行清理工作。
- AnimationCueTickEvent事件：在活动状态下的每个渲染请求，都将触发AnimationCueTickEvent事件，必须处理此事件来执行实际的动画处理。

###### 3.流程图

![alt](.\vtk_animation_flow_chart.jpg)

###### 4. 名词解释

- **vtkAnimationCueInfo:** Structure passed on every event invocation. Depending upon the cue time mode, these times are either normalized [0,1] or relative to the scene that contains the cue. **All this information is also available by asking the cue directly for it within the handler. Thus, this information can be accessed in wrapped languages**. (From vtkAnimationCue.h)不太明白为什么

  - 定义：

     

    ```
     class AnimationCueInfo {//一个嵌套在AnimationCue里的子类
    
       public:
    
      	double StartTime;
    
      	double EndTime;
    
    	 double AnimationTime; // valid only in AnimationCueTickEvent handler
    
     	 double DeltaTime;     // valid only in AnimationCueTickEvent handler
     	 //增量时间（通常用Δt或dt表示）表示程序或模拟中两个连续帧或更新之间经过的时间。
    
    	 double ClockTime;     // valid only in AnimationCueTickEvent handler
    	 //时钟时间指的是由时钟测量的现实世界时间。这是你在挂在墙上或数字时钟上看到的时间。
    
        };
    ```
    
    

-  **vtkAnimationScene**:

  - SetStartTime()/SetEndTime() These represent the start and end times of the animation scene. This is the range that the animation time covers during playback. 设置动画开始时间/动画结束时间
  - SetPlayMode() 设置实时播放模式，设置顺序播放模式 This is used to control they playback mode i.e. how the animation time is changed. There are two modes available: 
    - Sequence Mode (PLAYMODE_SEQUENCE) In this mode, the animation time is increased by (1/frame-rate) for every frame until the EndTime is reached. Hence the number of frames rendered in a single run is fixed irrespective of how long each frame takes to render.
    -  RealTime Mode (PLAYMODE_REALTIME) In this mode, the animation runs for approximately (EndTime-StartTime) seconds, where the animation time at nth frame is given by (animation time and (n-1)th frame + time to render (n-1)th frame). Thus the number of frames rendered changes depending on how long each frame takes to render.
  -  SetFrameRate()  设置帧率,单位时间内渲染的帧数,帧率越高越慢。帧率（Frame rate）是以帧称为单位的位图图像连续出现在显示器上的频率（速率）。 Frame rate is the number of frames per unit time. This is used only in sequence playmode. 
  - AddCue(), RemoveCue(), RemoveAllCue() Methods to add/remove animation cues from the scene. 
  - SetAnimationTime() SetAnimationTime can be used to explicitly advance to a particular frame. 
  - GetAnimationTime() GetAnimationTime() can be called during playback to query the animation clock time. 
  - Play() Starts playing the animation. SetLoop() When set to True, Play() results in playing the animation in a loop.

- **vtkAnimationCue** ：vtkAnimationCue corresponds to an entity that changes in an animation. vtkAnimationCue does not know how to bring about the changes to the parameters. So the user has to either subclass vtkAnimationCue or use event observers to perform the changes as the animation progresses

  - SetTimeMode: TimeMode defines how the start and time times are specified. There are two modes available. 

    - Relative (TIMEMODE_RELATIVE) In this mode the animation cue times are specified relative to the start of the animation scene. 

    - Normalized (TIMEMODE_NORMALIZED) In this mode, the cue start and end times are always in the range [0, 1] where 0 corresponds to the start and 1 corresponds to the end of the animation scene

      ```
      `enum TimeCodes { `
      
      `TIMEMODE_NORMALIZED = 0, `
      
      `TIMEMODE_RELATIVE = 1 };`
      cue->SetTimeModeToRelative();
      cue->SetTimeModeToNormalized();
      ```

​			

```
Normalized使用案例：
	  vtkNew<vtkAnimationCue> cue1;
  	  cue1 ->SetTimeModeToNormalized();
  	  cue1->SetStartTime(0);
 	  cue1->SetEndTime(1.0);//表示cue1的start time, end time和scene一致
```

```
Relative使用案例：
  scene->SetStartTime(20);
  scene->SetEndTime(200);
  
  cue2->SetStartTime(100);
  cue2->SetEndTime(300);
  scene->AddCue(cue2);//scene开始于时间20，cue2开始于时间20+100 = 120， 但停止于scene的endtime 200
```



###### 5.注意事项

- Scene与Cue的时间关系:在relative mode中， cue的开始相对于scene的开始时间，但不会超过scene的结束时间 During playback, a cue is active when the scene's animation time is within the range specified the start and end times for the cue. When the cue is activated, it fires the vtkCommand::StartAnimationCueEvent. For every subsequent frame, it fires the vtkCommand::AnimationCueTickEvent until the end-time is reached when the vtkCommand::EndAnimationCueEvent is fired. 
- 在AnimateActors.cxx里 scene->AddObserver(vtkCommand::AnimationCueTickEvent, renWin.GetPointer(),&vtkWindow::Render); //为什么额外需要scene观察：
  - 为了确保能在:AnimationCueTickEvent中触发render()， 因为此sphere 和cone都是独立在main里初始化，然后进行渲染的，与AnimationScene.cxx中在自己定义的callback函数StartCue的startCue()里初始化物体不一样

###### 6.参考资料

- https://blog.csdn.net/qq_33598781/article/details/121588868

- The VTK User’s Guide 11th Edition

  

# Study Notes for VTK



## Animation



#### 1. vtkCallbackCommand

- vtkTimerEvent: 必须首先initialize interacor: https://discourse.vtk.org/t/why-vtkcommand-timerevent-not-work/10113/3
- 

#### 2. vtkAnimationScene,vtkAnimationCue

- vtk  动画场景管理器：

          vtkAnimationCue和VTK vtkAnimationScene提供框架来支持动画。vtkAnimationCue代表一个随时间变化/动画的实体，而vtkAnimationScene代表动画的场景或设置，它由单个线索或其他场景组成。( vtkAnimationCue represents an entity that changes/ animates with time, while vtkAnimationScene represents scene or setup for the animation, which consists of individual cues or other scenes.)
      
          一个场景可以在实时模式下播放，或者作为帧1/帧率分开的时间序列。
      
          一个提示有三种状态：未初始化、活动和非活动。

  （1）未初始化：表示在提示开始时间之前的某个时间点。

  （2）活动状态：在提示的开始时间和结束时间之间的某个时间点，提示处于活动状态。

  （3）非活动状态：而在提示结束时间之后，提示处于非活动状态。

  （4）StartAnimationCueEvent事件：当提示（cue）进入活动状态时，将触发StartAnimationCueEvent事件。可以处理此事件来初始化要进行动画处理的实体。

  （5）EndAnimationCueEvent事件：当提示离开活动状态时，将触发EndAnimationCueEvent事件，可以处理此事件来在运行动画后进行清理工作。在活动状态下的每个渲染请求，都将触发AnimationCueTickEvent事件，必须处理此事件来执行实际的动画处理。
  ————————————————
  版权声明：本文为CSDN博主「小道士写程序」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
  原文链接：https://blog.csdn.net/zhanglixin999/article/details/133576700

- A cue has three states: UNINITIALIZED, ACTIVE and INACTIVE. 
- UNINITIALIZED represents an point in time before the start time of the cue. The cue is in ACTIVE state at a point in time between start time and end time for the cue. While, beyond the end time, it is in INACTIVE state. When the cue enters the ACTIVE state, StartAnimationCueEvent is fired. This event may be handled to initialize the entity to be animated. When the cue leaves the ACTIVE state, EndAnimationCueEvent is fired, which can be handled to cleanup after having run the animation. For every request to render during the ACTIVE state, AnimationCueTickEvent is fired, which must be handled to perform the actual animation

原理，存在问题，解决方案，

实用场景

scene cue是什么


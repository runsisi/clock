clock
=====

简介
--------

提示定时休息闹钟，当预设的间隔时间到时屏幕出现一个闹钟，并锁定鼠标操作。

说明
--------

节前终于把大学期间保存资料的移动硬盘大部分数据恢复了，整理以前那些杂七杂八的\
东西的时候找到了一份闹钟源代码，正好一直觉得xx管家提供的休息闹钟不能做到强制\
休息很让人讨厌，干脆自己动手加上强制休息的功能。

代码无任何原创性，只有锁定鼠标操作（全局消息 hook）的代码是自己在学校的时候写\
的，闹钟代码应该是好多年前从 codeproject 上下载的。

编译
--------

原始的代码是 Visual Studio 工程形式的，我给改成了 Makefile 形式的，在 Clock
和 GlobalHook 两目录下分别存在一个 Makefile，可以使用 nmake 分别编译出  Cloc\
k.exe 和 GlobalHook.dll。

运行
--------

拷贝 Clock 目录下的 Clock.exe gdiplus.dll Theme 以及 GlobalHook 目录下的 G\
lobalHook.dll 到同一目录，然后运行 Clock.exe 即可，闹钟会每隔 45 分钟唤醒\
一次，在唤醒的同时会屏蔽所有的鼠标动作。

自启动
----------

Clock 目录下有一个 autostart.bat 的批处理文件，可以用于控制 Clock.exe 是否\
在系统启动时自动启动，将该批处理放在和 Clock.exe 在同一目录，然后运行批处理\
按照提示操作即可。

完整的目录结构
--------------------

如下图所示：
::

    .
    ├── Clock.exe
    ├── GlobalHook.dll
    ├── gdiplus.dll
    ├── Theme
    └── autostart.bat

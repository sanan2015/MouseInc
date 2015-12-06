--本文件只是提供引擎的文档，并不具有实际功能
local Engine = Engine or {}

--[[

--重定向为OutputDebugString，可使用Dbgview查看
print

--退出MouscInc
Engine.Exit()

--切换托盘图标显示隐藏
Engine.ToggleTrayIcon()

--获取当前区域语言
Engine.GetDefaultLanguage()

--执行一个程序
Engine.Exec(path)

--模拟发送按键
Engine.Sendkey(keys)

--注册快捷键到一个函数，只能在Init以后调用
Engine.RegisterHotKey(keys, callback)

--获得当前虚拟桌面索引，从0开始
Engine.GetCurrentDesktop()

--切换至指定虚拟桌面
Engine.SwitchVirtualDesktop(index)

--把当前窗口发送到指定虚拟桌面
--目前微软只让控制自己程序的窗口，所以这个接口并没有什么卵用
Engine.SendtoVirtualDesktop(index)

--当前运行程序，例如Explorer.exe
Engine.CurrentApp()

--写入数据到配置文件MouseInc.ini
--例如想要设置
--  [Other]
--  HideGestureTrack=0
--调用
--  Engine.SetConfig("Other", "HideGestureTrack", 0)
Engine.SetConfig(section, key, value)

--写入数据到配置文件MouseInc.ini
--例如想要追加
--  Explorer.exe
--到
--  [MouseGestureExclude]
--调用
--  Engine.SetSection("MouseGestureExclude", "Explorer.exe")
Engine.SetSection(section, value)

--窗口置顶
--此处的窗口指的是鼠标手势启动时鼠标下方的窗口
--后面提到的窗口均指这个窗口
Engine.TopWindow()

--取消窗口置顶
Engine.NoTopWindow()

--最小化窗口
Engine.MinimizeWindow()

--最大化窗口
Engine.MaximizeWindow()

--还原最大化窗口
Engine.RestoreWindow()

--关闭窗口
Engine.CloseWindow()

--关闭相似窗口
Engine.CloseSimilarWindow()

--窗口居中
Engine.CenterWindow()

]]

return Engine
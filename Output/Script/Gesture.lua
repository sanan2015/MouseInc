--内置默认动作
local Engine = require('Engine')

local Gesture = {}

function Gesture.Exit()
    Engine.Exit()
end

function Gesture.ToggleTrayIcon()
    Engine.ToggleTrayIcon()
end

function Gesture.Calc()
    Engine.Exec("calc")
end

function Gesture.TopWindow()
    Engine.TopWindow()
end

function Gesture.NoTopWindow()
    Engine.NoTopWindow()
end

function Gesture.MinimizeWindow()
    Engine.MinimizeWindow()
end

function Gesture.MaximizeWindow()
    Engine.MaximizeWindow()
end

function Gesture.RestoreWindow()
    Engine.RestoreWindow()
end

function Gesture.CloseWindow()
    Engine.CloseWindow()
end

function Gesture.CloseSimilarWindow()
    Engine.CloseSimilarWindow()
end

function Gesture.CenterWindow()
    Engine.CenterWindow()
end

function Gesture.Exclude()
    local app = Engine.CurrentApp()
    Engine.SetSection("MouseGestureExclude", app)
end

--下面是一些常用的快捷键
function Gesture.NewTab()
    Engine.Sendkey("ctrl+t")
end

function Gesture.CloseTab()
    Engine.Sendkey("ctrl+w")
end

function Gesture.RightTab()
    Engine.Sendkey("ctrl+tab")
end

function Gesture.LeftTab()
    Engine.Sendkey("ctrl+shift+tab")
end

function Gesture.Forward()
    Engine.Sendkey("alt+→")
end

function Gesture.Backward()
    Engine.Sendkey("alt+←")
end

function Gesture.Refresh()
    Engine.Sendkey("f5")
end

function Gesture.Copy()
    Engine.Sendkey("ctrl+c")
end

function Gesture.Cut()
    Engine.Sendkey("ctrl+x")
end

function Gesture.Paste()
    Engine.Sendkey("ctrl+v")
end

return Gesture
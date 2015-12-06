--脚本入口文件

local Engine = require('Engine')
local Gesture = require('Gesture')
local Language = require('Language')

print("当前语言：" .. Engine.GetDefaultLanguage())

--初始化完成
function Init()
    --注册直达虚拟桌面的快捷键
    for i=1,9 do
        Engine.RegisterHotKey("ctrl+shift+" .. i, function()
            Engine.SwitchVirtualDesktop(i - 1)
        end)
    end
end

--执行手势动作
function ExecuteAction(action)
    Gesture[action]()
end

--获取多语言
function GetLocalized(name)
    local language = Language[Engine.GetDefaultLanguage()]
    if language then
        if language[name] then
            return language[name]
        end
    end
    
    -- print("Not Localized", language, name)
    return name
end

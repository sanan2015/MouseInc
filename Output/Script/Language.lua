--多语言支持，本文件编码为UTF8
local Language = {}

--简体中文，作者shuax
Language["zh-CN"] = 
{
    --程序内置语言
    ["name"] = "MouseInc",
    ["description"] = "MouseInc 鼠标增强软件",

    ["mouse_gesture"] = "鼠标手势",
    ["natural_scroll"] = "自然滚动",
    ["through_border"] = "穿越边界",

    ["enable"] = "启用",
    ["super_drag"] = "超级拖拽",
    ["super_volume"] = "超级音量",
    ["super_key"] = "超级键",

    ["hide_mouse_track"] = "隐藏手势轨迹",
    ["hide_gesture_result"] = "隐藏手势结果",
    ["volume_sound"] = "音量调节音效",
    ["hide_icon"] = "隐藏图标",
    ["other"] = "其它设置",

    ["about"] = "关于我们",
    ["autorun"] = "开机启动",
    ["exit"] = "退出程序",
    ["autorun_tip"] = "您正在临时文件夹中运行本软件，不建议启用开机启动。\n如果您依然想要开机启动，请选择 是。",

    --鼠标手势命令
    ['None'] = "无动作",
    ['Exit'] = "退出程序",
    ['ToggleTrayIcon'] = "切换托盘图标",
    ['Calc'] = "计算器",
    ['TopWindow'] = "置顶窗口",
    ['NoTopWindow'] = "取消窗口置顶",
    ['MinimizeWindow'] = "最小化窗口",
    ['MaximizeWindow'] = "最大化窗口",
    ['RestoreWindow'] = "还原窗口",
    ['CloseWindow'] = "关闭窗口",
    ['CloseSimilarWindow'] = "关闭相似窗口",
    ['CenterWindow'] = "居中窗口",
    ['Exclude'] = "排除当前程序",

    ['NewTab'] = "新建标签页",
    ['CloseTab'] = "关闭标签页",
    ['RightTab'] = "下一个标签页 ",
    ['LeftTab'] = "上一个标签页 ",
    ['Forward'] = "前进",
    ['Backward'] = "后退",
    ['Refresh'] = "刷新页面",
}


--臺灣中文，作者ZH-TW
Language["zh-TW"] = 
{
    --程式內建語言
    ["name"] = "MouseInc",
    ["description"] = "MouseInc 滑鼠增強軟體",

    ["mouse_gesture"] = "滑鼠手勢",
    ["natural_scroll"] = "自然捲動",
    ["through_border"] = "穿越版邊",

    ["enable"] = "啟用",
    ["super_drag"] = "超級拖拽",
    ["super_volume"] = "超級音量",
    ["super_key"] = "超級鍵",

    ["hide_mouse_track"] = "隱藏手勢軌跡",
    ["hide_gesture_result"] = "隱藏手勢結果",
    ["volume_sound"] = "音量調整音效",
    ["hide_icon"] = "隱藏圖示",
    ["other"] = "其它設定",

    ["about"] = "關於我們",
    ["autorun"] = "開機啟動",
    ["exit"] = "離開程式",
    ["autorun_tip"] = "您正在暫存資料夾中執行本軟體，不建議啟用開機啟動。\n若您依然想要開機啟動，請選擇 是。",

    --滑鼠手勢命令
    ['None'] = "無動作",
    ['Exit'] = "結束程式",
    ['Calc'] = "小算盤",
    ['TopWindow'] = "視窗置頂",
    ['NoTopWindow'] = "取消視窗置頂",
    ['Minimize'] = "最小化視窗",
    ['Maximize'] = "最大化視窗",
    ['Restore'] = "還原視窗",
    ['Close'] = "關閉視窗",
    ['CloseSimilar'] = "關閉相似視窗",
    ['Center'] = "置中視窗",
}

return Language

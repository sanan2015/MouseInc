HHOOK mouse_hook;


POINT last_point = {-1, -1};

//鼠标移动
bool OnMouseMove(MSLLHOOKSTRUCT *pmouse)
{
    // 超级拖拽
    if( IsPressed(VK_LBUTTON) && IsPressed(Config::SuperKey) && Config::GetInt(L"SuperKey", L"SuperDrag", 1) && !Config::IsExclude(L"SuperKeyExclude", ::WindowFromPoint(pmouse->pt)) )
    {
        if(last_point.x != -1 || last_point.y != -1)
        {
            HWND hwnd = GetTopWnd(::WindowFromPoint(pmouse->pt));

            int posx = pmouse->pt.x - last_point.x;
            int posy = pmouse->pt.y - last_point.y;

            MoveWnd(hwnd, posx, posy);
        }

        last_point = pmouse->pt;
        return false;
    }

    last_point = pmouse->pt;

    // 穿越边界
    // TODO 多显示器支持
    if(Config::GetInt(L"Basic", L"ThroughBorder"))
    {
        int cx = ::GetSystemMetrics(SM_CXSCREEN);
        int cy = ::GetSystemMetrics(SM_CYSCREEN);

        if( pmouse->pt.x<=0 || pmouse->pt.x >= cx || pmouse->pt.y <= 0 || pmouse->pt.y >= cy )
        {
            if( pmouse->pt.x <= 0  ) pmouse->pt.x = cx - 1;//左
            if( pmouse->pt.x >= cx ) pmouse->pt.x = 1;//右
            if( pmouse->pt.y <= 0  ) pmouse->pt.y = cy - 1;//上
            if( pmouse->pt.y >= cy ) pmouse->pt.y = 1;//下

            ::SetCursorPos(pmouse->pt.x, pmouse->pt.y);
            return true;
        }
    }

    gesture_mgr.OnMouseMove(pmouse);

    return false;
}

bool OnMouseWheel(MSLLHOOKSTRUCT *pmouse)
{
    // 超级音量
    if( IsPressed(Config::SuperKey) && Config::GetInt(L"SuperKey", L"SuperVolume", 1) && !Config::IsExclude(L"SuperKeyExclude", ::WindowFromPoint(pmouse->pt)) )
    {
        short direction = GET_WHEEL_DELTA_WPARAM(pmouse->mouseData);
        if(direction<0)
        {
            //音量-
            SendOneKey(VK_VOLUME_DOWN);
        }
        else
        {
            //音量+
            SendOneKey(VK_VOLUME_UP);
        }

        //播放调节音效
        if(Config::GetInt(L"Other", L"VolumeSound", 1))
        {
            ::PlaySound(L"SOUND", ::GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_NOSTOP | SND_NODEFAULT);
        }
        return true;
    }

    // 自然滚动
    if( Config::GetInt(L"Basic", L"NaturalScroll") && !Config::IsExclude(L"NaturalScrollExclude", ::WindowFromPoint(pmouse->pt)) )
    {
        //获得方向
        short direction = GET_WHEEL_DELTA_WPARAM(pmouse->mouseData);

        return natural_scroll.Wheel(direction < 0);
    }
    return false;
}

//滚动按下
bool OnMButtonDown(MSLLHOOKSTRUCT *pmouse)
{
    // 超级音量
    if( IsPressed(Config::SuperKey) && Config::GetInt(L"SuperKey", L"SuperVolume", 1) && !Config::IsExclude(L"SuperKeyExclude", ::WindowFromPoint(pmouse->pt)) )
    {
        //禁音
        SendOneKey(VK_VOLUME_MUTE);
        return true;
    }

    return false;
}

//右键按下
bool OnRButtonDown(MSLLHOOKSTRUCT *pmouse)
{
    return gesture_mgr.OnRButtonDown(pmouse);
}

//右键弹起
bool OnRButtonUp(MSLLHOOKSTRUCT *pmouse)
{
    return gesture_mgr.OnRButtonUp(pmouse);
}

//鼠标钩子
LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    MSLLHOOKSTRUCT *pmouse = (MSLLHOOKSTRUCT *)lParam;
    if ( nCode == HC_ACTION )
    {
        bool handled = false;

        switch(wParam)
        {
        case WM_MOUSEWHEEL:
            handled = OnMouseWheel(pmouse);
            break;
        case WM_MOUSEMOVE:
            handled = OnMouseMove(pmouse);
            break;
        case WM_MBUTTONDOWN:
            handled = OnMButtonDown(pmouse);
            break;
        case WM_RBUTTONDOWN:
            handled = OnRButtonDown(pmouse);
            break;
        case WM_RBUTTONUP:
            handled = OnRButtonUp(pmouse);
            break;
        }

        if(handled)
        {
            return 1;
        }
    }
    return ::CallNextHookEx(mouse_hook, nCode, wParam, lParam);
}
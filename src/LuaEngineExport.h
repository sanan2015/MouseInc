/*
导出函数给lua使用
*/

int engine_default_language(lua_State *L)
{
    lua_pushstring(L, utf16to8(GetDefaultLanguage().c_str()).c_str());
    return 1;
}

int engine_exit(lua_State *L)
{
    ::PostMessage(main_window, WM_COMMAND, WM_APP + 13, 0);
    return 0;
}

int engine_toggle_trayicon(lua_State *L)
{
    ::PostMessage(main_window, WM_COMMAND, WM_APP + 100, 0);
    return 0;
}

int engine_exec(lua_State *L)
{
    const char* path = luaL_checkstring(L, 1);

    ShellExecute(NULL, L"open", utf8to16(path).c_str(), NULL, NULL, SW_SHOWNORMAL);
    return 0;
}

int engine_top_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::SetWindowPos(current_window, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return 0;
}

int engine_notop_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::SetWindowPos(current_window, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    return 0;
}

int engine_minimize_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::PostMessage(current_window, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    return 0;
}

int engine_maximize_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::PostMessage(current_window, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
    return 0;
}

int engine_restore_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::PostMessage(current_window, WM_SYSCOMMAND, SC_RESTORE, 0);
    return 0;
}

int engine_close_window(lua_State *L)
{
    if ( !IsDesktop(current_window) ) ::PostMessage(current_window, WM_SYSCOMMAND, SC_CLOSE, 0);
    return 0;
}

BOOL CALLBACK CloseSimilarWindows(HWND hwnd, LPARAM lParam)
{
    if (GetParent(hwnd) == NULL && IsWindowVisible(hwnd)) //可见顶层窗口
    {
        TCHAR buff[256];
        GetClassName(hwnd, buff, 255);
        if (lstrcmp(buff, (TCHAR *)lParam) == 0) PostMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
    }
    return 1;
}

int engine_close_similar_window(lua_State *L)
{
    if ( !IsDesktop(current_window) )
    {
        TCHAR buff[256];
        GetClassName(current_window, buff, 255);
        EnumWindows(CloseSimilarWindows, LPARAM(buff));
    }
    return 0;
}

void CenterWindow(HWND hwnd)
{
    RECT r;
    GetWindowRect(hwnd,&r);

    RECT w;
    SystemParametersInfo(SPI_GETWORKAREA, 0, &w, 0);
    int width = r.right - r.left;
    int height = r.bottom - r.top;
    int x = w.left + ((w.right - w.left) / 2) - (width / 2);
    int y = w.top + ((w.bottom - w.top) / 2) - (height / 2);

    SetWindowPos(hwnd, 0, x, y, width, height, SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER);
}

int engine_center_window(lua_State *L)
{
    if (!IsDesktop(current_window))
    {
        CenterWindow(current_window);
        SetForegroundWindow(current_window);
    }
    return 0;
}

int engine_sendkey(lua_State *L)
{
    const char* keys = luaL_checkstring(L, 1);
    SendKey(utf8to16(keys));
    return 0;
}

int engine_current_app(lua_State *L)
{
    std::wstring path = GetHwndProcess(current_window);
    std::wstring name = GetFileName(path);

    lua_pushstring(L, utf16to8(name.c_str()).c_str());
    return 1;
}

int engine_set_config(lua_State *L)
{
    const char* section = luaL_checkstring(L, 1);
    const char* key = luaL_checkstring(L, 2);
    const char* value = luaL_checkstring(L, 3);

    Config::SetStr(utf8to16(section).c_str(), utf8to16(key).c_str(), utf8to16(value).c_str());
    return 0;
}


int engine_set_section(lua_State *L)
{
    const char* section = luaL_checkstring(L, 1);
    const char* value = luaL_checkstring(L, 2);

    std::wstring wvalue = utf8to16(value);
    wvalue += L'\0';
    Config::SetString(utf8to16(section).c_str(), wvalue.c_str());
    return 0;
}

int engine_register_hotkey(lua_State *L)
{
    const char* keys = luaL_checkstring(L, 1);
    if(lua_isfunction(L, -1))
    {
        int ref = luaL_ref(L, LUA_REGISTRYINDEX);

        int mo, vk = 0;
        ParseHotkey(utf8to16(keys), mo, vk);
        if(RegisterHotKey(main_window, ref, mo, vk))
        {
            lua_pushboolean(L, true);
            return 1;
        }
    }
    return 0;
}

int engine_get_currentdesktop(lua_State *L)
{
	UINT num = 0;
	if (SUCCEEDED(GetCurrentDesktop(num)))
    {
        lua_pushinteger(L, num);
        return 1;
    }
    return 0;
}

int engine_switch_virtualdesktop(lua_State *L)
{
    int num = (int)luaL_checkinteger(L, 1);
    if(SUCCEEDED(SwitchVirtualDesktop(num)))
    {
        lua_pushboolean(L, true);
        return 1;
    }
    return 0;
}

int engine_sendto_virtualdesktop(lua_State *L)
{
	int num = (int)luaL_checkinteger(L, 1);
	if (SUCCEEDED(SendtoVirtualDesktop(current_window, num)))
	{
		lua_pushboolean(L, true);
		return 1;
	}
	
    return 0;
}

void LoadEngine(lua_State *L)
{
    lua_newtable(L);

    lua_pushstring(L, "GetDefaultLanguage");
    lua_pushcfunction(L, engine_default_language);
    lua_settable(L, -3);

    lua_pushstring(L, "Exit");
    lua_pushcfunction(L, engine_exit);
    lua_settable(L, -3);

    lua_pushstring(L, "Exec");
    lua_pushcfunction(L, engine_exec);
    lua_settable(L, -3);

    lua_pushstring(L, "TopWindow");
    lua_pushcfunction(L, engine_top_window);
    lua_settable(L, -3);

    lua_pushstring(L, "NoTopWindow");
    lua_pushcfunction(L, engine_notop_window);
    lua_settable(L, -3);

    lua_pushstring(L, "MinimizeWindow");
    lua_pushcfunction(L, engine_minimize_window);
    lua_settable(L, -3);

    lua_pushstring(L, "MaximizeWindow");
    lua_pushcfunction(L, engine_maximize_window);
    lua_settable(L, -3);

    lua_pushstring(L, "RestoreWindow");
    lua_pushcfunction(L, engine_restore_window);
    lua_settable(L, -3);

    lua_pushstring(L, "CloseWindow");
    lua_pushcfunction(L, engine_close_window);
    lua_settable(L, -3);

    lua_pushstring(L, "CloseSimilarWindow");
    lua_pushcfunction(L, engine_close_similar_window);
    lua_settable(L, -3);

    lua_pushstring(L, "CenterWindow");
    lua_pushcfunction(L, engine_center_window);
    lua_settable(L, -3);

    lua_pushstring(L, "Sendkey");
    lua_pushcfunction(L, engine_sendkey);
    lua_settable(L, -3);

    lua_pushstring(L, "ToggleTrayIcon");
    lua_pushcfunction(L, engine_toggle_trayicon);
    lua_settable(L, -3);

    lua_pushstring(L, "CurrentApp");
    lua_pushcfunction(L, engine_current_app);
    lua_settable(L, -3);

    lua_pushstring(L, "SetConfig");
    lua_pushcfunction(L, engine_set_config);
    lua_settable(L, -3);

    lua_pushstring(L, "SetSection");
    lua_pushcfunction(L, engine_set_section);
    lua_settable(L, -3);

    lua_pushstring(L, "RegisterHotKey");
    lua_pushcfunction(L, engine_register_hotkey);
    lua_settable(L, -3);
	
    lua_pushstring(L, "GetCurrentDesktop");
    lua_pushcfunction(L, engine_get_currentdesktop);
    lua_settable(L, -3);

    lua_pushstring(L, "SwitchVirtualDesktop");
    lua_pushcfunction(L, engine_switch_virtualdesktop);
    lua_settable(L, -3);
	
    lua_pushstring(L, "SendtoVirtualDesktop");
    lua_pushcfunction(L, engine_sendto_virtualdesktop);
    lua_settable(L, -3);

    lua_setglobal(L, "Engine");
}
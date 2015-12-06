/*
基于lua的多语言
*/

namespace i18n
{
    //多语言
    std::wstring GetString(std::wstring name)
    {
        lua_State *L = lua_engine->State();

        lua_getglobal(L, "GetLocalized");
        if (lua_isfunction(L, -1))
        {
            lua_pushstring(L, utf16to8(name.c_str()).c_str());
            if (!lua_pcall(L, 1, 1, 0))
            {
                if (lua_isstring(L, -1))
                {
                    name = utf8to16(lua_tostring(L, -1));
                }
                lua_pop(L, 1);
            }
        }

        return name;
    }
}

// 下面两个函数因为需要i18n ，所以在这里实现

int TipsBox(const wchar_t *tips, UINT type = MB_OK, HWND hwnd = NULL)
{
    return MessageBoxW(hwnd, tips, i18n::GetString(L"name").c_str(), type);
}

void switch_autorun(HWND hwnd)
{
    HKEY hKey;
    ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run"), 0, KEY_ALL_ACCESS, &hKey);
    if (!is_autorun())
    {
        TCHAR path[MAX_PATH];
        GetPrettyPath(path);
        if (::_tcsstr(path, _T("Temp")) != NULL)
        {
            if (TipsBox(i18n::GetString(L"autorun_tip").c_str(), MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2, hwnd) == IDNO) return;
        }
        ::RegSetValueEx(hKey, app_name, 0, REG_SZ, (LPBYTE)path, (DWORD)_tcslen(path)*sizeof(TCHAR));
    }
    else
    {
        ::RegDeleteValue(hKey, app_name);
    }
    ::RegCloseKey(hKey);
}
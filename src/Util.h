#include <psapi.h>

void GetPrettyPath(TCHAR *path)
{
    TCHAR temp[MAX_PATH];
    ::GetModuleFileName(NULL, temp, MAX_PATH);
    ::PathCanonicalize(path, temp);
    ::PathQuoteSpaces(path);
}

std::wstring GetPathName(std::wstring &path)
{
    auto file = PathFindFileName(path.c_str());
    PathRemoveExtension(file);
    return file;
}

std::wstring GetFileName(std::wstring &path)
{
    auto file = PathFindFileName(path.c_str());
    return file;
}

bool is_autorun()
{
    HKEY hKey;
    if (::RegOpenKeyEx(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        wchar_t buffer[MAX_PATH];
        DWORD dwLength = MAX_PATH;
        if (::RegQueryValueEx(hKey, app_name, NULL, NULL, (LPBYTE)buffer, &dwLength) == ERROR_SUCCESS)
        {
            ::RegCloseKey(hKey);

            wchar_t path[MAX_PATH];
            GetPrettyPath(path);

            if (::_tcsicmp(path, buffer) == 0)
            {
                return true;
            }
        }
        else
        {
            ::RegCloseKey(hKey);
        }
    }

    return false;
}

#define KEY_PRESSED 0x8000
bool IsPressed(int key)
{
    return key && ( ::GetAsyncKeyState(key) & KEY_PRESSED ) != 0;
}

HWND GetTopWnd(HWND hwnd)
{
    while( ::GetParent(hwnd) && ::IsWindowVisible( ::GetParent(hwnd) ) )
    {
        hwnd = ::GetParent(hwnd);
    }
    return hwnd;
}

bool IsDesktop(HWND hwnd)
{
    TCHAR buff[256];
    ::GetClassName(hwnd, buff, 256);

    //任务栏
    if ( ::_wcsicmp(buff, L"Shell_TrayWnd") == 0 )
    {
        return true;
    }

    // 桌面
    if (::_wcsicmp(buff, L"WorkerW") == 0)
    {
        return true;
    }
    if (::_wcsicmp(buff, L"Progman") == 0)
    {
        return true;
    }

    //托盘
    if (::_wcsicmp(buff, L"NotifyIconOverflowWindow") == 0)
    {
        return true;
    }

    //开始
    if (::_wcsicmp(buff, L"Windows.UI.Core.CoreWindow") == 0)
    {
        return true;
    }

    //任务切换
    if (::_wcsicmp(buff, L"MultitaskingViewFrame") == 0)
    {
        return true;
    }

    return false;
}

void MoveWnd(HWND hwnd, int posx, int posy)
{
    //不要移动桌面等窗口
    if( IsDesktop(hwnd) ) return;

    RECT rect;
    ::GetWindowRect(hwnd, &rect);

    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    rect.left += posx;
    rect.top += posy;

    ::MoveWindow(hwnd, rect.left, rect.top, width, height, true);
}

template<typename String, typename Char, typename Function>
void StringSplit(String *str, Char delim, Function f)
{
    String *ptr = str;
    while (*str)
    {
        if (*str == delim)
        {
            *str = 0;           //截断字符串

            if (str - ptr)       //非空字符串
            {
                f(ptr);
            }

            *str = delim;       //还原字符串
            ptr = str + 1;      //移动下次结果指针
        }
        str++;
    }

    if (str - ptr)  //非空字符串
    {
        f(ptr);
    }
}

void SendKey(std::wstring &keys)
{
    std::vector <INPUT> inputs;

    TCHAR *temp = _tcsdup(keys.c_str());
    StringSplit(temp, L'+', [&]
        (TCHAR *key)
    {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_EXTENDEDKEY;

        WORD vk = 0;

        //解析控制键
        if (_tcsicmp(key, _T("Shift")) == 0) vk = VK_SHIFT;
        else if (_tcsicmp(key, _T("Ctrl")) == 0) vk = VK_CONTROL;
        else if (_tcsicmp(key, _T("Alt")) == 0) vk = VK_MENU;
        else if (_tcsicmp(key, _T("Win")) == 0) vk = VK_LWIN;
        //解析方向键
        else if (_tcsicmp(key, _T("←")) == 0) vk = VK_LEFT;
        else if (_tcsicmp(key, _T("→")) == 0) vk = VK_RIGHT;
        else if (_tcsicmp(key, _T("↑")) == 0) vk = VK_UP;
        else if (_tcsicmp(key, _T("↓")) == 0) vk = VK_DOWN;
        //解析单个字符A-Z、0-9等
        else if (_tcslen(key) == 1)
        {
            if (isalnum(key[0])) vk = toupper(key[0]);
            else vk = LOWORD(VkKeyScan(key[0]));
        }
        //解析F1-F24功能键
        else if ( (key[0] == 'F' || key[0] == 'f') && isdigit(key[1]) )
        {
            int FX = _ttoi(&key[1]);
            if (FX >= 1 && FX <= 24) vk = VK_F1 + FX - 1;
        }
        // 解析其他按键
        else
        {
            if (_tcsicmp(key, _T("Left")) == 0) vk = VK_LEFT;
            else if (_tcsicmp(key, _T("Right")) == 0) vk = VK_RIGHT;
            else if (_tcsicmp(key, _T("Up")) == 0) vk = VK_UP;
            else if (_tcsicmp(key, _T("Down")) == 0) vk = VK_DOWN;

            else if (_tcsicmp(key, _T("Esc")) == 0) vk = VK_ESCAPE;
            else if (_tcsicmp(key, _T("Tab")) == 0) vk = VK_TAB;

            else if (_tcsicmp(key, _T("Backspace")) == 0) vk = VK_BACK;
            else if (_tcsicmp(key, _T("Enter")) == 0) vk = VK_RETURN;
            else if (_tcsicmp(key, _T("Space")) == 0) vk = VK_SPACE;

            else if (_tcsicmp(key, _T("PrtSc")) == 0) vk = VK_SNAPSHOT;
            else if (_tcsicmp(key, _T("Scroll")) == 0) vk = VK_SCROLL;
            else if (_tcsicmp(key, _T("Pause")) == 0) vk = VK_PAUSE;

            else if (_tcsicmp(key, _T("Insert")) == 0) vk = VK_INSERT;
            else if (_tcsicmp(key, _T("Delete")) == 0) vk = VK_DELETE;

            else if (_tcsicmp(key, _T("End")) == 0) vk = VK_END;
            else if (_tcsicmp(key, _T("Home")) == 0) vk = VK_HOME;

            else if (_tcsicmp(key, _T("PageUp")) == 0) vk = VK_PRIOR;
            else if (_tcsicmp(key, _T("PageDown")) == 0) vk = VK_NEXT;

            else if (_tcsicmp(key, _T("Back")) == 0) vk = VK_BROWSER_BACK;
            else if (_tcsicmp(key, _T("Forward")) == 0) vk = VK_BROWSER_FORWARD;
            else if (_tcsicmp(key, _T("Refresh")) == 0) vk = VK_BROWSER_REFRESH;

            else if (_tcsicmp(key, _T("VolumeMute")) == 0) vk = VK_VOLUME_MUTE;
            else if (_tcsicmp(key, _T("VolumeDown")) == 0) vk = VK_VOLUME_DOWN;
            else if (_tcsicmp(key, _T("VolumeUp")) == 0) vk = VK_VOLUME_UP;
        }

        input.ki.wVk = vk;

        inputs.push_back(input);
    });

    free(temp);

    //发起按下
    ::SendInput((UINT)inputs.size(), &inputs[0], sizeof(INPUT));

    //发送弹起
    for ( auto &input : inputs )
    {
        input.ki.dwFlags |= KEYEVENTF_KEYUP;
    }
    ::SendInput((UINT)inputs.size(), &inputs[0], sizeof(INPUT));
}

void ParseHotkey(std::wstring &keys, int &mo, int &vk)
{
    mo = 0;
    vk = 0;

    TCHAR *temp = _tcsdup(keys.c_str());
    StringSplit(temp, L'+', [&]
        (TCHAR *key)
    {
        //解析控制键
        if (_tcsicmp(key, _T("Shift")) == 0) mo |= MOD_SHIFT;
        else if (_tcsicmp(key, _T("Ctrl")) == 0) mo |= MOD_CONTROL;
        else if (_tcsicmp(key, _T("Alt")) == 0) mo |= MOD_ALT;
        else if (_tcsicmp(key, _T("Win")) == 0) mo |= MOD_WIN;
        //解析方向键
        else if (_tcsicmp(key, _T("←")) == 0) vk = VK_LEFT;
        else if (_tcsicmp(key, _T("→")) == 0) vk = VK_RIGHT;
        else if (_tcsicmp(key, _T("↑")) == 0) vk = VK_UP;
        else if (_tcsicmp(key, _T("↓")) == 0) vk = VK_DOWN;
        //解析单个字符A-Z、0-9等
        else if (_tcslen(key) == 1)
        {
            if (isalnum(key[0])) vk = toupper(key[0]);
            else vk = LOWORD(VkKeyScan(key[0]));
        }
        //解析F1-F24功能键
        else if (key[0] == 'F' || key[0] == 'f')
        {
            if (isdigit(key[1]))
            {
                int FX = _ttoi(&key[1]);
                if (FX >= 1 && FX <= 24) vk = VK_F1 + FX - 1;
            }
        }
        // 解析其他按键
        else
        {
            if (_tcsicmp(key, _T("Left")) == 0) vk = VK_LEFT;
            else if (_tcsicmp(key, _T("Right")) == 0) vk = VK_RIGHT;
            else if (_tcsicmp(key, _T("Up")) == 0) vk = VK_UP;
            else if (_tcsicmp(key, _T("Down")) == 0) vk = VK_DOWN;

            else if (_tcsicmp(key, _T("Esc")) == 0) vk = VK_ESCAPE;
            else if (_tcsicmp(key, _T("Tab")) == 0) vk = VK_TAB;

            else if (_tcsicmp(key, _T("Backspace")) == 0) vk = VK_BACK;
            else if (_tcsicmp(key, _T("Enter")) == 0) vk = VK_RETURN;
            else if (_tcsicmp(key, _T("Space")) == 0) vk = VK_SPACE;

            else if (_tcsicmp(key, _T("PrtSc")) == 0) vk = VK_SNAPSHOT;
            else if (_tcsicmp(key, _T("Scroll")) == 0) vk = VK_SCROLL;
            else if (_tcsicmp(key, _T("Pause")) == 0) vk = VK_PAUSE;

            else if (_tcsicmp(key, _T("Insert")) == 0) vk = VK_INSERT;
            else if (_tcsicmp(key, _T("Delete")) == 0) vk = VK_DELETE;

            else if (_tcsicmp(key, _T("End")) == 0) vk = VK_END;
            else if (_tcsicmp(key, _T("Home")) == 0) vk = VK_HOME;

            else if (_tcsicmp(key, _T("PageUp")) == 0) vk = VK_PRIOR;
            else if (_tcsicmp(key, _T("PageDown")) == 0) vk = VK_NEXT;
        }
    });

    free(temp);

    if (IsWindows7OrGreater())
    {
        mo |= MOD_NOREPEAT;
    }
}

void SendOneKey(int key)
{
    INPUT input[1];
    memset(input, 0, sizeof(input));

    input[0].type = INPUT_KEYBOARD;

    input[0].ki.wVk = key;

    input[0].ki.dwFlags = KEYEVENTF_UNICODE;
    ::SendInput(1, input, sizeof(INPUT));

    input[0].ki.dwFlags |= KEYEVENTF_KEYUP;
    ::SendInput(1, input, sizeof(INPUT));
}

void SendOneMouse(int mouse)
{
    INPUT input[1];
    memset(input, 0, sizeof(input));

    input[0].type = INPUT_MOUSE;

    input[0].mi.dwFlags = mouse;
    ::SendInput(1, input, sizeof(INPUT));
}

void SendWheel(int up)
{
    INPUT input[1];
    memset(input, 0, sizeof(input));

    input[0].type = INPUT_MOUSE;

    input[0].mi.mouseData = up?WHEEL_DELTA:-WHEEL_DELTA;
    input[0].mi.dwFlags = MOUSEEVENTF_WHEEL;
    ::SendInput(1, input, sizeof(INPUT));
}

// 编码转换
std::wstring utf8to16( const char* src )
{
    std::vector<wchar_t> buffer;
    buffer.resize(MultiByteToWideChar(CP_UTF8, 0, src, -1, 0, 0));
    MultiByteToWideChar(CP_UTF8, 0, src, -1, &buffer[0], (int)buffer.size());
    return std::wstring(&buffer[0]);
}

// 编码转换
std::string utf16to8( const wchar_t* src )
{
    std::vector<char> buffer;
    buffer.resize(WideCharToMultiByte(CP_UTF8, 0, src, -1,  NULL, 0, NULL, NULL));
    WideCharToMultiByte(CP_UTF8, 0, src, -1, &buffer[0], (int)buffer.size(), NULL, NULL);
    return std::string(&buffer[0]);
}

// 获得程序所在文件夹
std::wstring GetAppPath()
{
    wchar_t path[MAX_PATH];
    ::GetModuleFileName(NULL, path, MAX_PATH);
    ::PathRemoveFileSpec(path);
    return std::wstring(path);
}

// 获得系统语言
std::wstring GetDefaultLanguage()
{
    wchar_t language[MAX_PATH];
    if (!GetLocaleInfo(GetUserDefaultUILanguage(), LOCALE_SISO639LANGNAME, language, MAX_PATH))
    {
        return L"zh-CN";
    }
    wchar_t country[MAX_PATH];
    if (!GetLocaleInfo(GetUserDefaultUILanguage(), LOCALE_SISO3166CTRYNAME, country, MAX_PATH))
    {
        return std::wstring(language);
    }

    return std::wstring(language) + L"-" + country;
}

// 加载资源内容
template<typename Function>
bool LoadFromResource(const wchar_t *type, const wchar_t *name, Function f)
{
    bool result = false;
    HRSRC res = FindResource(NULL, name, type);
    if (res)
    {
        HGLOBAL header = LoadResource(NULL, res);
        if (header)
        {
            const char *data = (const char*)LockResource(header);
            DWORD size = SizeofResource(NULL, res);
            if (data)
            {
                f(data, size);
                result = true;
                UnlockResource(header);
            }
        }
        FreeResource(header);
    }

    return result;
}

bool ImageFromIDResource(const wchar_t *name, Image *&pImg)
{
    LoadFromResource(L"PNG", name, [&](const char *data, DWORD size)
    {
        HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, size);
        BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
        memcpy(pmem, data, size);
        GlobalUnlock(m_hMem);

        IStream* pstm;
        CreateStreamOnHGlobal(m_hMem, FALSE, &pstm);

        pImg = Image::FromStream(pstm);

        pstm->Release();
        GlobalFree(m_hMem);
    });
    return TRUE;
}

#define BUFSIZE 512
void DevicePathToWin32Path(wchar_t *strDevicePath)
{
    wchar_t szTemp[BUFSIZE];

    if (::GetLogicalDriveStringsW(BUFSIZE - 1, szTemp))
    {
        wchar_t szName[MAX_PATH];
        wchar_t szDrive[3] = L" :";
        wchar_t* p = szTemp;
        do
        {
            *szDrive = *p;
            if (::QueryDosDeviceW(szDrive, szName, MAX_PATH))
            {
                size_t uNameLen = ::wcslen(szName);
                if (::_wcsnicmp(strDevicePath, szName, uNameLen) == 0)
                {
                    wchar_t szTempFile[MAX_PATH];

                    ::wsprintfW(szTempFile, L"%s%s", szDrive, strDevicePath + uNameLen);
                    ::wcscpy_s(strDevicePath, MAX_PATH, szTempFile);
                }
            }
            while (*p++);
        } while (*p);
    }
}

bool isEndWith(const wchar_t *src, const wchar_t* dst)
{
    if (!src || !dst) return false;
    size_t src_len = ::wcslen(src);
    size_t dst_len = ::wcslen(dst);
    if (dst_len > src_len) return false;
    return !::_memicmp(src + src_len - dst_len, dst, dst_len * sizeof(wchar_t));
}

std::wstring GetHwndProcess(HWND hwnd)
{
    DWORD dwPID = 0;
    ::GetWindowThreadProcessId(hwnd, &dwPID);

    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
    if (hProcess)
    {
        wchar_t path[MAX_PATH];
        ::GetProcessImageFileNameW(hProcess, path, MAX_PATH);
        ::CloseHandle(hProcess);

        DevicePathToWin32Path(path);
        return path;
    }

    return L"";
}

IVirtualDesktopManagerInternal *pDesktopManagerInternal = nullptr;
IVirtualDesktopManager *pDesktopManager = nullptr;

HRESULT InitializeVirtualDesktopManagerInternal() {
    IServiceProvider* pServiceProvider = nullptr;
    if (FAILED(::CoCreateInstance(CLSID_ImmersiveShell, NULL, CLSCTX_LOCAL_SERVER, __uuidof(IServiceProvider), (PVOID*)&pServiceProvider))) {
        return E_FAIL;
    }
    if (FAILED(pServiceProvider->QueryService(CLSID_VirtualDesktopAPI_Unknown, &pDesktopManagerInternal))) {
        return E_FAIL;
    }
    if (FAILED(pServiceProvider->QueryService(__uuidof(IVirtualDesktopManager), &pDesktopManager))) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT GetCurrentDesktop(UINT &num) {
    IObjectArray *pDesktops;
    if (pDesktopManagerInternal == nullptr || FAILED(pDesktopManagerInternal->GetDesktops(&pDesktops))) {
        return E_FAIL;
    }

    HRESULT hres = E_FAIL;

    IVirtualDesktop *pCurrentDesktop;
    pDesktopManagerInternal->GetCurrentDesktop(&pCurrentDesktop);

    GUID current_guid = { 0 };
    pCurrentDesktop->GetID(&current_guid);

    UINT count;
    if (SUCCEEDED(pDesktops->GetCount(&count)))
    {
        for (UINT i = 0; i < count; i++)
        {
            IVirtualDesktop *pDesktop;
            if (SUCCEEDED(pDesktops->GetAt(i, __uuidof(IVirtualDesktop), (void**)&pDesktop))) {

                GUID guid = { 0 };
                pDesktop->GetID(&guid);
                if (guid == current_guid)
                {
                    num = i;
                    hres = S_OK;
                    break;
                }

                pDesktop->Release();
            }
        }

    }

    pCurrentDesktop->Release();

    return hres;
}

HRESULT SwitchVirtualDesktop(UINT num)
{
    IObjectArray *pDesktops;
    if (pDesktopManagerInternal == nullptr || FAILED(pDesktopManagerInternal->GetDesktops(&pDesktops))) {

        return E_FAIL;
    }

    HRESULT hres = S_OK;
    IVirtualDesktop *pDesktop;

    if (SUCCEEDED(pDesktops->GetAt(num, __uuidof(IVirtualDesktop), (void**)&pDesktop))) {
        hres = pDesktopManagerInternal->SwitchDesktop(pDesktop);
    }
    else {
        hres = E_FAIL;
    }

    return hres;
}

HRESULT SendtoVirtualDesktop(HWND hwnd, UINT num) {
    IObjectArray *pDesktops;
    if (pDesktopManagerInternal == nullptr || pDesktopManager == nullptr || FAILED(pDesktopManagerInternal->GetDesktops(&pDesktops))) {
        return E_FAIL;
    }

    HRESULT hres = S_OK;
    IVirtualDesktop *pDesktop;

    if (SUCCEEDED(pDesktops->GetAt(num, __uuidof(IVirtualDesktop), (void**)&pDesktop))) {
        GUID guid = { 0 };
        pDesktop->GetID(&guid);
        hres = pDesktopManager->MoveWindowToDesktop(hwnd, guid);
        //wchar_t xx[123];
        //wsprintf(xx, L"hres:%X", hres);
        //OutputDebugString(xx);
        pDesktop->Release();
    }
    else {
        hres = E_FAIL;
    }

    return hres;
}
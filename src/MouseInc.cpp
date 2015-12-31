#include "MouseInc.h"

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

    ImmDisableIME(-1);

    // 检查单例，并通知旧程序
    HANDLE mutex = ::CreateMutex(NULL, TRUE, repeat_run);
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        UINT WM_REPEAT_RUN = ::RegisterWindowMessage(repeat_run);
        ::PostMessage(HWND_BROADCAST, WM_REPEAT_RUN, 0, 0);
        ::CloseHandle(mutex);
        return 0;
    }

    InitializeVirtualDesktopManagerInternal();

    // 加载lua引擎
    lua_engine = new LuaEngine();

    // 加载配置
    Config::Init();

    // 启动程序
    Window wnd;
    if (!wnd.Create(NULL, CRect(0, 0, 400, 400), app_name, WS_CAPTION))
    {
        return 0;
    }
    // wnd.ShowWindow(nCmdShow);

    // 消息循环
    WTL::CMessageLoop msgLoop;
    int ret = msgLoop.Run();

    delete lua_engine;
    return ret;
}
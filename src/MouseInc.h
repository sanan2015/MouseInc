#include <windows.h>
//#include <VersionHelpers.h>

#include <vld.h>

#include <GdiPlus.h>
using namespace Gdiplus;

#pragma warning(disable: 4838)
#pragma warning(disable: 4302)
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>
#include <atlmisc.h>
#include <atlcrack.h>
#include <atlctrls.h>
#include <atlctrlx.h>

#include <string>
#include <vector>

const wchar_t repeat_run[] = L"{39E75892-612E-41A4-948A-DA1163DEA54E}";
const wchar_t app_name[] = L"MouscInc";

HWND main_window;
HWND current_window;
std::wstring current_aciton;

#include "VirtualDesktops.h"
#include "Util.h"
#include "Config.h"

#include "LuaEngine.h"
#include "LuaEngineExport.h"
#include "LuaEngineLang.h"

#include "GestureRecognition.h"
#include "GestureWindow.h"
#include "GestureMgr.h"

#include "NaturalScroll.h"

#include "MouseProc.h"
#include "KeyboardProc.h"

#include "ContextMenu.h"
#include "AboutWindow.h"
#include "Window.h"

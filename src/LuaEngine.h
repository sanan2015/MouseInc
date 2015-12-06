// 重定向lua的fopen函数，支持utf8路径
extern "C" FILE *myfopen(const char *path, const char *mode)
{
    FILE *f;
    _wfopen_s(&f, utf8to16(path).c_str(), utf8to16(mode).c_str());
    return f;
}
#include <lua.hpp>
#include <lauxlib.h>

//lua错误处理
int error_handler(lua_State *L)
{
    MessageBox(0, utf8to16(luaL_checkstring(L, -1)).c_str(), L"LuaEngine Error", 0);
    lua_pop(L, 1);
    return 0;
}

int error_handler2(lua_State *L)
{
    OutputDebugString(utf8to16(luaL_checkstring(L, -1)).c_str());
    lua_pop(L, 1);
    return 0;
}

int print(lua_State *L)
{
    int n = lua_gettop(L);  /* number of arguments */
    int i;
    lua_getglobal(L, "tostring");
    std::string buf;
    for (i = 1; i <= n; i++) {
        const char *s;
        size_t l;
        lua_pushvalue(L, -1);  /* function to be called */
        lua_pushvalue(L, i);   /* value to print */
        lua_call(L, 1, 1);
        s = lua_tolstring(L, -1, &l);  /* get result */
        if (s == NULL)
            return luaL_error(L, "'tostring' must return a string to 'print'");
        if (i > 1) buf += "\t";
        buf += s;
        lua_pop(L, 1);  /* pop result */
    }
    
    OutputDebugString(utf8to16(buf.c_str()).c_str());
    return 0;
}

extern void LoadEngine(lua_State *L);

class LuaEngine
{
public:
    LuaEngine()
    {
        L = luaL_newstate();

        luaL_openlibs(L);
        lua_atpanic(L, error_handler2);


        LoadEngine(L);


        std::wstring package_path = L"package.path=[[" + GetAppPath() + L"\\Script\\?.lua]]";

        //MessageBox(0, package_path.c_str(), package_path.c_str(),0);

        luaL_dostring(L, utf16to8(package_path.c_str()).c_str());

        lua_pushcfunction(L, print);
        lua_setglobal(L, "print");

        std::wstring path = GetAppPath() + L"\\Script\\" + L"Entry.lua";
        if (luaL_dofile(L, utf16to8(path.c_str()).c_str()))
        {
            error_handler(L);
            ExitProcess(0);
        }
    }
    
    ~LuaEngine()
    {
        lua_close(L);
    }

    lua_State *State()
    {
        return L;
    }

    void Do(const wchar_t *action)
    {
        lua_getglobal(L, "ExecuteAction");
        if (lua_isfunction(L, -1))
        {
            lua_pushstring(L, utf16to8(action).c_str());
            if (lua_pcall(L, 1, 0, 0))
            {
                error_handler2(L);
            }
        }
    }

	void Init()
	{
		lua_getglobal(L, "Init");
		if (lua_isfunction(L, -1))
		{
			if (lua_pcall(L, 0, 0, 0))
			{
				error_handler2(L);
			}
		}
	}
	void Callback(int ref)
	{
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		if (lua_isfunction(L, -1))
		{
			if (lua_pcall(L, 0, 0, 0))
			{
				error_handler2(L);
			}
		}
	}
private:
    lua_State *L;
};

LuaEngine* lua_engine = nullptr;
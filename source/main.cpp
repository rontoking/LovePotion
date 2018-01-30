extern "C" 
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>

	#include <compat-5.2.h>
	#include <luaobj.h>
}

#include <string>

#include "console.h"
#include "filesystem.h"
#include "graphics.h"
#include "love.h"

#include <stdio.h>

#include <switch.h>
#include "boot_lua.h"

#define luaL_dobuffer(L, b, n, s) \
	(luaL_loadbuffer(L, b, n, s) || lua_pcall(L, 0, LUA_MULTRET, 0))

lua_State * L;
bool ERROR = false;
bool LOVE_QUIT = false;

int main()
{
	Console::Initialize();

	Graphics::Initialize();

	Filesystem::Initialize();

	L = luaL_newstate();

	luaL_openlibs(L);
	luaL_requiref(L, "love", Love::Initialize, 1);

	std::string buff = "print('OS: ' .. love.system.getOS())";
	buff += "print(love.graphics.getWidth() .. 'x' .. love.graphics.getHeight())";

	luaL_dostring(L, buff.c_str());

	if (luaL_dobuffer(L, (char *)boot_lua, boot_lua_size, "boot"))
		Console::ThrowError(L);

	while(appletMainLoop())
	{
		if (ERROR || LOVE_QUIT)
			break;

		hidScanInput();
	
		if (hidKeysDown(CONTROLLER_P1_AUTO) & KEY_PLUS) 
			break;

		gfxFlushBuffers();
		gfxSwapBuffers();
		gfxWaitForVsync();
	}

	Love::Exit(L);

	return 0;
}
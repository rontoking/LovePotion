/*
** common/aux.h
** @brief   : Auxillary Lua functions
*/

namespace Aux
{
    int DoBuffer(lua_State * L, const char * buffer, size_t size, const char * name);
}
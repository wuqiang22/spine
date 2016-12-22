#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaValue.h"
#include "cocos2d/LuaScriptHandlerMgr.h"
#include "CCLuaEngine.h"
#include "spine/lua_cocos2dx_spine_manual_ex.h"
#include "spine/SpineCacheAnimation.h"
#include "spine/LuaSkeletonCacheAnimation.h"

static int lua_spine_SpineCacheAnimation_createWithFile(lua_State* tolua_S)
{
	if (NULL == tolua_S)
		return 0;

	

#if COCOS2D_DEBUG >= 1
	tolua_Error tolua_err;
	if (!tolua_isusertable(tolua_S, 1, "sp.SpineCacheAnimation", 0, &tolua_err)) goto tolua_lerror;
#endif

	bool ok = true;
	int argc = 0;
	argc = lua_gettop(tolua_S) - 1;

	if (argc == 2)
	{
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "sp.SpineCacheAnimation:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_spine_SpineCacheAnimation_createWithFile'", nullptr);
			return 0;
		}
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "sp.SpineCacheAnimation:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_spine_SpineCacheAnimation_createWithFile'", nullptr);
			return 0;
		}

		LuaSkeletonCacheAnimation* ret = LuaSkeletonCacheAnimation::createWithFile(arg0.c_str(), arg1.c_str());
		object_to_luaval<LuaSkeletonCacheAnimation>(tolua_S, "sp.SpineCacheAnimation", (LuaSkeletonCacheAnimation*)ret);
		return 1;
	}
	else if (argc == 3)
	{
		std::string arg0;
		ok &= luaval_to_std_string(tolua_S, 2, &arg0, "sp.SpineCacheAnimation:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_spine_SpineCacheAnimation_createWithFile'", nullptr);
			return 0;
		}
		std::string arg1;
		ok &= luaval_to_std_string(tolua_S, 3, &arg1, "sp.SpineCacheAnimation:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_spine_SpineCacheAnimation_createWithFile'", nullptr);
			return 0;
		}
		LUA_NUMBER arg2;
		ok &= luaval_to_number(tolua_S, 4, &arg2, "sp.SpineCacheAnimation:create");
		if (!ok)
		{
			tolua_error(tolua_S, "invalid arguments in function 'lua_spine_SpineCacheAnimation_createWithFile'", nullptr);
			return 0;
		}

		LuaSkeletonCacheAnimation* ret = LuaSkeletonCacheAnimation::createWithFile(arg0.c_str(), arg1.c_str(), arg2);
		object_to_luaval<LuaSkeletonCacheAnimation>(tolua_S, "sp.SpineCacheAnimation", (LuaSkeletonCacheAnimation*)ret);
		return 1;
	}

	luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n", "sp.SpineCacheAnimation:create", argc, 2);
	return 0;

#if COCOS2D_DEBUG >= 1
tolua_lerror:
	tolua_error(tolua_S, "#ferror in function 'lua_spine_SpineCacheAnimation_createWithFile'.", &tolua_err);
	return 0;
#endif
}


TOLUA_API int register_cocos2dx_spine_manual_ex(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, "sp", 0);
	tolua_beginmodule(tolua_S, "sp");

	tolua_usertype(tolua_S, "sp.SpineCacheAnimation");
	tolua_cclass(tolua_S, "SpineCacheAnimation", "sp.SpineCacheAnimation", "sp.SkeletonAnimation", nullptr);

	tolua_beginmodule(tolua_S, "SpineCacheAnimation");

	tolua_function(tolua_S, "create", lua_spine_SpineCacheAnimation_createWithFile);

	tolua_endmodule(tolua_S);
	std::string typeName = typeid(LuaSkeletonCacheAnimation).name();
	g_luaType[typeName] = "sp.SpineCacheAnimation";
	g_typeCast["SpineCacheAnimation"] = "sp.SpineCacheAnimation";
	tolua_endmodule(tolua_S);
	return 1;
}

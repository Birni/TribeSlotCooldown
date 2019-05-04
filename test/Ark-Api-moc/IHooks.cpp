#include "IHooks.h"


namespace ArkApi
{

	IHooks * IHooks::instance()
	{
		if (!_instance)
			_instance = new IHooks();
		return _instance;
	}


	bool IHooks::SetHook(const std::string& func_name, LPVOID detour, LPVOID original)
	{
		return true;
	};

	bool IHooks::DisableHook(const std::string& func_name, LPVOID detour)
	{
		return true;
	};

	IHooks IHooks::GetIHooks()
	{
		return *_instance;
	};


	IHooks GetHooks()
	{
		return IHooks::GetIHooks();
	};

	IHooks * IHooks::_instance = 0;
} // namespace ArkApi

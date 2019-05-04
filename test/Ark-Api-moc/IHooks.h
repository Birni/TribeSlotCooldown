#include "moc.h"
#include <string>
#include "moc.h"


#ifndef IHOOOKS_H
#define IHOOOKS_H

namespace ArkApi
{
	class  IHooks
	{
	private:
		static IHooks* _instance;

	public:
		static IHooks* instance();

		virtual ~IHooks() = default;

		static IHooks GetIHooks();

		 bool SetHook(const std::string& func_name, LPVOID detour, LPVOID original);
		 bool DisableHook(const std::string& func_name, LPVOID detour);


	};

	IHooks GetHooks();
} // namespace ArkApi

#endif /* IHOOOKS_H */

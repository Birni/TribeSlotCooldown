/****************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                 *
*                                                                           *
* This file is part of the TribeSlotCooldown Plugin for Ark Server API      *
*                                                                           *
*   The TribeSlotCooldown Plugin is free software: you can                  *
*	redistribute it and/or modify it under the terms of the GNU Lesser      *
*	General Public License as published by the Free Software Foundation,    *
*	either version 3 of the License, or (at your option) any later version. *
*                                                                           *
*   The TribeSlotCooldown Plugin is distributed in the hope that it         *
*   will be useful, but WITHOUT ANY WARRANTY; without even the implied      *
* 	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* 	See the GNU Lesser General Public License for more details.             *
*                                                                           *
****************************************************************************/

/**
* \file TribeSlotCooldown.cpp
* \author Matthias Birnthaler
* \date 19 March 2019
* \brief Implementation of DLLMain
*
*/

#include "TribeSlotCooldown.h"
#include "Hooks.h"
#include "SlotCooldown.h"
#include "Commands.h"



/**
* \brief Load of TribeSlotCooldown Plugin
*
* This function loads the Plugin and calls the initialisation
*
* \return void
*/
static void Load(void)
{
	Log::Get().Init("TribeSlotCooldown");

	SlotCooldown::InitSlotCooldown();
	Commands::InitCommands();
	InitHooks();
}


/**
* \brief Unload of TribeSlotCooldown Plugin
*
* This function unloads the Plugin
*
* \return void
*/
static void Unload(void)
{
	RemoveHooks();
	Commands::RemoveComands();
}


/**
* \brief DLLMain of the TribeSlotCooldown Plugin
*
* \param[in] hModule
* \param[in] ul_reason_for_call
* \param[in] lpReserved
* \return BOOL APIENTRY
*/
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		Load();
		break;
	case DLL_PROCESS_DETACH:
		Unload();
		break;
	}
	return TRUE;
}

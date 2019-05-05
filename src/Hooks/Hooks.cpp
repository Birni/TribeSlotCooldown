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
* \file Hooks.cpp
* \author Matthias Birnthaler 
* \date 19 March 2019
* \brief File containing the implementation for all needed Hooks  
*
*/

#include "Hooks.h"
#include "SlotCooldown.h"




/**
* \brief Initialisation of needed Hooks 
*
* This function initialise all needed Hooks and read the relevant information from the json config file.
*
* \return void
*/
void InitHooks(void)
{
	ArkApi::GetHooks().SetHook("AShooterPlayerState.AddToTribe", &Hook_AShooterPlayerState_AddToTribe, &AShooterPlayerState_AddToTribe_original);

	ArkApi::GetHooks().SetHook("AShooterPlayerState.ServerRequestLeaveTribe_Implementation", &Hook_AShooterPlayerState_ServerRequestLeaveTribe_Implementation, 
		&AShooterPlayerState_ServerRequestLeaveTribe_Implementation_original);

	ArkApi::GetHooks().SetHook("AShooterPlayerState.ServerRequestRemovePlayerIndexFromMyTribe_Implementation",
		&Hook_AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation,
		&AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation_original);
}


/**
* \brief Cancellation of needed Hooks
*
* This function removes all needed Hooks.
*
* \return void
*/
void RemoveHooks(void)
{
	ArkApi::GetHooks().DisableHook("AShooterPlayerState.AddToTribe", &Hook_AShooterPlayerState_AddToTribe);

	ArkApi::GetHooks().DisableHook("AShooterPlayerState.ServerRequestLeaveTribe_Implementation", &Hook_AShooterPlayerState_ServerRequestLeaveTribe_Implementation);

	ArkApi::GetHooks().DisableHook("AShooterPlayerState.ServerRequestRemovePlayerIndexFromMyTribe_Implementation",
		&Hook_AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation);
}




/**
* \brief Hook of Hook_AShooterPlayerState_AddToTribe
*
* This function ... 
*
* \return void
*/
bool  Hook_AShooterPlayerState_AddToTribe(AShooterPlayerState* _this, FTribeData* MyNewTribe, bool bMergeTribe, bool bForce, bool bIsFromInvite, APlayerController* InviterPC)
{
	bool SuppressAdToTribe = false;

	if ((nullptr != MyNewTribe) && (nullptr != _this))
	{
		AShooterPlayerController* player = ArkApi::GetApiUtils().FindPlayerFromSteamId(_this->MyPlayerDataStructField()->PlayerDataIDField());
		
	//	if (nullptr != player)
		{
		//	if (false == player->bIsAdmin().Get())
			{

				if (false == bMergeTribe) /* only on player joins tribe */
				{
					auto tribeid = MyNewTribe->TribeIDField();

					SuppressAdToTribe = SlotCooldown::SuppressPlayerJoinTribe(tribeid);

					/* notification for the player */
					if (true == SuppressAdToTribe)
					{
						AShooterPlayerController* player = ArkApi::GetApiUtils().FindPlayerFromSteamId(_this->MyPlayerDataStructField()->PlayerDataIDField());
						ArkApi::GetApiUtils().SendNotification(player, FColorList::Red, 1.4, 10, nullptr, *SlotCooldown::SuppressPlayerJoinTribeMessage);
					}


				}
				else /* tribe merge */
				{
					auto numPlayersInOldTribe = (_this->MyTribeDataField()->MembersPlayerDataIDField()).Num();
					auto numPlayersInNewTribe = (MyNewTribe->MembersPlayerDataIDField()).Num();

					SuppressAdToTribe = SlotCooldown::SuppressTribeMerge(MyNewTribe->TribeIDField(), _this->MyPlayerDataStructField()->TribeIDField(),
						numPlayersInNewTribe, numPlayersInOldTribe);

					/* notification for the player */
					if (true == SuppressAdToTribe)
					{
						AShooterPlayerController* player = ArkApi::GetApiUtils().FindPlayerFromSteamId(_this->MyPlayerDataStructField()->PlayerDataIDField());
						ArkApi::GetApiUtils().SendNotification(player, FColorList::Red, 1.4F, 10.0F, nullptr, *SlotCooldown::SuppressMergeTribeMessage);
					}
				}
			}
		}
	}

	if (true == SuppressAdToTribe)
	{
		/* the original add to tribe function call is suppressed*/
		return false;
	}
	else 
	{
		/* call the original add to tribe function */
		return AShooterPlayerState_AddToTribe_original(_this, MyNewTribe, bMergeTribe, bForce, bIsFromInvite, InviterPC);
	}
}


void  Hook_AShooterPlayerState_ServerRequestLeaveTribe_Implementation(AShooterPlayerState* _this)
{

	if (nullptr != _this)
	{
		AShooterPlayerController* player = ArkApi::GetApiUtils().FindPlayerFromSteamId(_this->MyPlayerDataStructField()->PlayerDataIDField());

		/*DEBUG */

		if (nullptr == player)
		{
			Log::GetLog()->error("SteamId: {}" , _this->MyPlayerDataStructField()->PlayerDataIDField());
		}

		/* -----*/

		//if (nullptr != player)
		{
			//if (false == player->bIsAdmin().Get())
			{
				auto tribeid = _this->MyPlayerDataStructField()->TribeIDField();
				SlotCooldown::SetTribeSlotToCooldown(tribeid);
			}
		}
	}

	AShooterPlayerState_ServerRequestLeaveTribe_Implementation_original(_this);
}


/**
* \brief Cancellation of needed Hooks
*
* This function removes all needed Hooks.
*
* \return void
*/
void  Hook_AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation(AShooterPlayerState* _this, int PlayerIndexInTribe)
{
	if (nullptr != _this)
	{
		AShooterPlayerController* player = ArkApi::GetApiUtils().FindPlayerFromSteamId(_this->MyPlayerDataStructField()->PlayerDataIDField());

	//	if (nullptr != player)
		{
		//	if (false == player->bIsAdmin().Get())
			{
				auto tribeid = _this->MyPlayerDataStructField()->TribeIDField();
				SlotCooldown::SetTribeSlotToCooldown(tribeid);
			}
		}
	}
	AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation_original(_this, PlayerIndexInTribe);
}

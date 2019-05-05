#ifndef SLOTCOOLDOWN_H
#define SLOTCOOLDOWN_H

#include "json.hpp"
#include <API/Ark/Ark.h>
#include "Tools.h"
#include "DBHandler.h"



namespace SlotCooldown
{

	inline nlohmann::json config;
	inline std::unique_ptr<DBHandler> database;

	/* Messages */
	extern FString SuppressPlayerJoinTribeMessage;
	extern FString SuppressMergeTribeMessage;
	extern FString CommandDisplaySlotsMessage;
	extern FString CommandDisplaySlotsMessageSlotCooldown;

	/* Commands */
	extern FString CommandPrefix;
	extern FString CommandDisplaySlots;



	extern void ReadConfig();
	extern bool GetFreeSlot(int * FoundSlot, std::vector<int> SlotsTimer, int ServerRunTime);
	extern void MergeTribeCooldowns(std::vector<int> *  SlotsNewTribe, std::vector<int> * SlotsOldTribe, int ServerRunTime);

	extern void InitSlotCooldown();
	extern void SetTribeSlotToCooldown(int TribeId);
	extern bool SuppressPlayerJoinTribe(int TribeId);
	extern bool SuppressTribeMerge(int TribeIdNewTribe, int TribeIdOldTribe, int NumPlayersInNewTribe, int NumPlayersInOldTribe);

}


#endif /* SLOTCOOLDOWN_H */
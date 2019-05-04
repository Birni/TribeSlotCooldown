#ifndef SLOTCOOLDOWN_H
#define SLOTCOOLDOWN_H

#include "json.hpp"
#include <API/Ark/Ark.h>
#include "Tools.h"
#include "DBHandler.h"

typedef struct 
{
	int hours;
	int minutes;
	int secounds;

}SlotCooldown_t;


namespace SlotCooldown
{

	inline nlohmann::json config;
	inline std::unique_ptr<DBHandler> database;

	extern FString SuppressPlayerJoinTribeMessage;
	extern FString SuppressMergeTribeMessage;



	extern void ReadConfig();
	extern bool GetFreeSlot(int * FoundSlot, std::vector<int> SlotsTimer, int ServerRunTime);
	extern SlotCooldown_t GetCooldownTimeOfSlot(int SlotTimer, int ServerRunTime);
	extern void MergeTribeCooldowns(std::vector<int> *  SlotsNewTribe, std::vector<int> * SlotsOldTribe, int ServerRunTime);

	extern void InitSlotCooldown();
	extern void SetTribeSlotToCooldown(int TribeId);
	extern bool SuppressPlayerJoinTribe(int TribeId);
	extern bool SuppressTribeMerge(int TribeIdNewTribe, int TribeIdOldTribe, int NumPlayersInNewTribe, int NumPlayersInOldTribe);

}


#endif /* SLOTCOOLDOWN_H */
#include "SlotCooldown.h"

#include <fstream>


namespace SlotCooldown
{
	/* Messages */
	FString SuppressPlayerJoinTribeMessage;
	FString SuppressMergeTribeMessage;
	FString CommandDisplaySlotsMessage;
	FString CommandDisplaySlotsMessageSlotCooldown;

	/* Commands */
	FString CommandPrefix;
	FString CommandDisplaySlots;

	void ReadConfig()
	{
		const std::string config_path = ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/TribeSlotCooldown/config.json";
		std::ifstream file{ config_path };
		if (!file.is_open())
		{
			throw std::runtime_error("Can't open config.json");
		}

		file >> SlotCooldown::config;

		file.close();
	}

	int CountSlotsWithCooldown(std::vector<int> SlotsTimer, int ServerRunTime)
	{
		int count = 0; 
		int tribelimit = SlotCooldown::config["General"]["Tribelimit"];

		for (int i = 0;  (i < SlotsTimer.size() && (i < (tribelimit - 2))); i++)
		{
			if (SlotsTimer[i] < ServerRunTime)
			{
				count++;
			}
		}

		return count;
	}


	bool GetFreeSlot(int * FoundSlot, std::vector<int> SlotsTimer, int ServerRunTime)
	{
		bool result = false;

		if (nullptr != FoundSlot)
		{
			if (true == SlotsTimer.empty())
			{
				*FoundSlot = 0;
				result = true;
			}
			else
			{
				int tribelimit =  SlotCooldown::config["General"]["Tribelimit"];

				for (int i = 0; (i < SlotsTimer.size() && (i < (tribelimit-2) )) ; i++)
				{
					if (SlotsTimer[i] < ServerRunTime)
					{
						*FoundSlot = i;
						result = true;
						break;
					}
				}
			}
		}

		return result;
	}

	void MergeTribeCooldowns(std::vector<int> *  SlotsNewTribe, std::vector<int> * SlotsOldTribe, int ServerRunTime)
	{
		int j = 0;
		int tribelimit = SlotCooldown::config["General"]["Tribelimit"];

		if ((nullptr != SlotsOldTribe) && (nullptr != SlotsNewTribe))
		{
			for (int i = 0; (i < (*SlotsNewTribe).size() && (i < (tribelimit - 2))); i++)
			{
				if((*SlotsNewTribe)[i] < ServerRunTime)
				{
					(*SlotsNewTribe)[i] = (*SlotsOldTribe)[j];
					j++;
				}

				if (j > (*SlotsOldTribe).size())
				{
					break;
				}
			}

		}

	}


	void InitSlotCooldown()
	{
		ReadConfig();

		const std::string db_path = SlotCooldown::config["General"]["DbPathOverride"];
		
		Log::GetLog()->error(db_path);

		database = std::make_unique<DBHandler>(db_path);

		SuppressPlayerJoinTribeMessage = FString(ArkApi::Tools::Utf8Decode(config["Messages"]["SuppressPlayerJoinTribeMessage"]).c_str());
		SuppressMergeTribeMessage = FString(ArkApi::Tools::Utf8Decode(config["Messages"]["SuppressMergeTribeMessage"]).c_str());

		CommandDisplaySlotsMessage = FString(ArkApi::Tools::Utf8Decode(config["Messages"]["CommandDisplaySlotsMessage"]).c_str());
		CommandDisplaySlotsMessageSlotCooldown = FString(ArkApi::Tools::Utf8Decode(config["Messages"]["CommandDisplaySlotsMessageSlotCooldown"]).c_str());

		CommandPrefix = FString(ArkApi::Tools::Utf8Decode(config["Commands"]["CommandPrefix"]).c_str());
		CommandDisplaySlots = FString(ArkApi::Tools::Utf8Decode(config["Commands"]["CommandDisplaySlots"]).c_str());
	
	}

	void SetTribeSlotToCooldown(int TribeId)
	{
		std::vector<int> slots;
		int freeSlot;
		long double currentServerTime;

		/* add tribe to database if not exist */
		if (false == database->IsTribeInDatabase(TribeId))
		{
			database->AddTribe(TribeId);
		}

		/* select tribe slot cooldowns from databse*/
		slots = database->GetTribeSlotsTimer(TribeId);

		/* update tribe slot cooldowns */
		currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();
		GetFreeSlot(&freeSlot, slots, currentServerTime);
		slots[freeSlot] = currentServerTime + config["General"]["SlotCooldown"] * 3600;

		/* rewrite  tribe slots to database */
		database->UpdateSlotTimer(TribeId, slots);
	}


	bool SuppressPlayerJoinTribe(int TribeId)
	{
		bool result;
		int freeSlot;
		long double currentServerTime;

		if (true == database->IsTribeInDatabase(TribeId))
		{
			std::vector<int> slots = database->GetTribeSlotsTimer(TribeId);
			currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();

			if (true == GetFreeSlot(&freeSlot, slots, currentServerTime))
			{
				result = false;
			}

		}
		else /* never removed a player from tribe */
		{
			result = true;
		}

		return result;
	}


	bool SuppressTribeMerge(int TribeIdNewTribe, int TribeIdOldTribe, int NumPlayersInNewTribe, int NumPlayersInOldTribe)
	{
		bool result = true;
		bool isNewTribeInDb;
		bool isOldTribeInDb;
		int NumOfNeededTribeSlots = 0;
		int NumOfSlotsWithCooldownOldTribe = 0;
		int NumOfSlotsWithCooldownNewTribe = 0;
		std::vector<int> slotsOfNewTribe;
		std::vector<int> slotsOfOldTribe;
		int tribelimit = SlotCooldown::config["General"]["Tribelimit"];
		long double currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();


		isOldTribeInDb = database->IsTribeInDatabase(TribeIdOldTribe);
		if (true == isOldTribeInDb)
		{
			slotsOfOldTribe = database->GetTribeSlotsTimer(TribeIdOldTribe);
			NumOfSlotsWithCooldownOldTribe = CountSlotsWithCooldown(slotsOfOldTribe, currentServerTime);
		}

		isNewTribeInDb = database->IsTribeInDatabase(TribeIdNewTribe);
		if (true == isNewTribeInDb)
		{
			slotsOfNewTribe = database->GetTribeSlotsTimer(TribeIdOldTribe);
			NumOfSlotsWithCooldownNewTribe = CountSlotsWithCooldown(slotsOfNewTribe, currentServerTime);
		}

		if ((tribelimit - (NumPlayersInOldTribe + NumOfSlotsWithCooldownNewTribe))
			<= (NumPlayersInNewTribe + NumOfSlotsWithCooldownOldTribe))
		{
			MergeTribeCooldowns(&slotsOfOldTribe, &slotsOfNewTribe, currentServerTime);
			result = false;
		}

		if (false == isNewTribeInDb)
		{
			database->AddTribe(TribeIdNewTribe);
		}
		database->UpdateSlotTimer(TribeIdNewTribe, slotsOfNewTribe);

		if (true == isOldTribeInDb)
		{
			database->DeleteTribe(TribeIdOldTribe);
		}

		return result;
	}

}
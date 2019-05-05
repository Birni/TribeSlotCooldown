#include "Commands.h"
#include "SlotCooldown.h"
#include <sstream>
#include <string>




namespace Commands
{
	typedef struct
	{
		int hours;
		int minutes;
		int secounds;

	}SlotCooldown_t;


	static FString DisplaySlots;


	static FString GetStringWithTribeSlotCooldowns(int TribeId);
	SlotCooldown_t GetCooldownTimeOfSlot(int SlotTimer, int ServerRunTime);
	static void NormalizeSlots(std::vector<int>* slots, long double ServerRunTime);


	SlotCooldown_t GetCooldownTimeOfSlot(int SlotTimer, int ServerRunTime)
	{
		SlotCooldown_t result = { 0 };

		int timediff = SlotTimer - ServerRunTime;

		if (0 < timediff)
		{
			result.hours = (int)(timediff / 3600);
			result.minutes = (int)(((float)(timediff % 3600)) / 60);
			result.secounds = (int)((float)((timediff % 3600) % 60));
		}

		return result;
	}


	static void NormalizeSlots(std::vector<int>* slots, long double ServerRunTime)
	{
		if (nullptr != slots)
		{
			for (std::vector<int>::iterator it = slots->begin(); it != slots->end(); ++it)
			{
				if (*it < ServerRunTime)
				{
					slots->erase(it);
				}
			}

			std::sort(slots->begin(), slots->end());
		}
	}


	static FString GetStringWithTribeSlotCooldowns(int TribeId)
	{
		FString displayString = "";
		std::vector<int> slots;
		auto currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();

		if (true == SlotCooldown::database->IsTribeInDatabase(TribeId))
		{
			slots = SlotCooldown::database->GetTribeSlotsTimer(TribeId);

			NormalizeSlots(&slots, currentServerTime);
		}

		displayString = FString::Format(*SlotCooldown::CommandDisplaySlotsMessage, slots.size());

		displayString += "\n";
		displayString += "\n";

		for (int i = 0; i < slots.size(); i++)
		{
			SlotCooldown_t timediff = GetCooldownTimeOfSlot(slots[i], (int)currentServerTime);

			FString str = FString::Format(*SlotCooldown::CommandDisplaySlotsMessageSlotCooldown, i + 1, timediff.hours, timediff.minutes, timediff.secounds);
			displayString += "\n";
			displayString += str;

		}

		return displayString;
	}



	static void ChatCommandDisplaySlots(AShooterPlayerController* player, FString* message, int mode)
	{
		if (nullptr != player)
		{
			FString displayString = GetStringWithTribeSlotCooldowns(player->TargetingTeamField());
			ArkApi::GetApiUtils().SendNotification(player, FColorList::White, 1.4F, 10.0F, nullptr, *displayString);
		}
	}


	static void RconPlayerTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
	{
		if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
		{
			TArray<FString> parsed;
			rcon_packet->Body.ParseIntoArray(parsed, L" ", true);

			AShooterPlayerController* searchedplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(std::stoi(parsed[1].ToString()));
			FString displayString = GetStringWithTribeSlotCooldowns(searchedplayer->TargetingTeamField());
			rcon_connection->SendMessageW(rcon_packet->Id, 0, &displayString);
		}
	}


	static void ConsolePlayerTribeCooldownSlots(APlayerController* player, FString* cmd, bool boolean)
	{
		if ((nullptr != player) && (nullptr != cmd))
		{
			if (true == player->bIsAdmin().Get())
			{
				TArray<FString> parsed;
				cmd->ParseIntoArray(parsed, L" ", true);

				if (true == parsed.IsValidIndex(1))
				{
					AShooterPlayerController* searchedplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(std::stoi(parsed[1].ToString()));
					FString displayString = GetStringWithTribeSlotCooldowns(searchedplayer->TargetingTeamField());
					const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
				    ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, *displayString);

				}
			}
		}
	}


	static void RconListTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
	{
		if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
		{
			TArray<FString> parsed;
			rcon_packet->Body.ParseIntoArray(parsed, L" ", true);

			if (true == parsed.IsValidIndex(1))
			{
				FString displayString = GetStringWithTribeSlotCooldowns(std::stoi(parsed[1].ToString()));
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &displayString);
			}
			else
			{
				FString reply = "Couldn't reseted slot!\n";
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
			}
		}
	}


	static void ConsoleListTribeCooldownSlots(APlayerController* player, FString* cmd, bool boolean)
	{
		if ((nullptr != player) && nullptr != cmd)
		{
			if (true == player->bIsAdmin().Get())
			{
				TArray<FString> parsed;
				cmd->ParseIntoArray(parsed, L" ", true);

				if (true == parsed.IsValidIndex(1))
				{
					FString displayString = GetStringWithTribeSlotCooldowns(std::stoi(parsed[1].ToString()));
					const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
					ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, *displayString);
				}
			}
		}
	}

	static bool ResetSlotOfTribe(const FString& cmd)
	{
		TArray<FString> parsed;
		cmd.ParseIntoArray(parsed, L" ", true);
		bool result = false;

		if ((true == parsed.IsValidIndex(1))
			&& (true == parsed.IsValidIndex(2)))
		{
			std::vector<int> slots;
			int tribeId = std::stoi(parsed[1].ToString());
			auto currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();

			if (true == SlotCooldown::database->IsTribeInDatabase(tribeId))
			{
				slots = SlotCooldown::database->GetTribeSlotsTimer(tribeId);

				NormalizeSlots(&slots, currentServerTime);

				int selectedNr = parsed.IsValidIndex(2);

				if ((false == slots.empty())
					&& (0 < selectedNr)
					&& (slots.size() + 1 <= selectedNr))
				{
					slots[selectedNr - 1] = 0;

					SlotCooldown::database->UpdateSlotTimer(tribeId, slots);
					result = true;
				}
			}
		}
		return result;
	}


	static void ConsoleResetSlotOfTribe(APlayerController* player, FString* cmd, bool boolean)
	{
		if ((nullptr != player) && (nullptr != cmd))
		{
			if (true == player->bIsAdmin().Get())
			{
				const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
				if (true == ResetSlotOfTribe(*cmd))
				{
					ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, "Successfully reseted slot!");
				}
				else
				{
					ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, "Couldn't reseted slot!");
				}
			}
		}
	}


	void RconResetSlotOfTribe(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
	{
		if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
		{
			if (true == ResetSlotOfTribe(rcon_packet->Body))
			{
				FString reply = "Successfully reseted slot!\n";
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
			}
			else
			{
				FString reply = "Couldn't reseted slot!\n";
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
			}
		}
	}



	bool GetTribeIdOfPlayer(const FString& cmd, int * TribeId)
	{
		TArray<FString> parsed;
		cmd.ParseIntoArray(parsed, L" ", true);
		bool result = false;

		if (nullptr != TribeId)
		{

			if (true == parsed.IsValidIndex(1))
			{
				AShooterPlayerController* givenplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(parsed.IsValidIndex(1));

				if (nullptr != givenplayer)
				{
					*TribeId = givenplayer->TargetingTeamField();
					result = true;
				}
			}
		}
		return result;
	}



	static void ConsoleGetTribeIdOfPlayer(APlayerController* player, FString* cmd, bool boolean)
	{
		if ((nullptr != player) && (nullptr != cmd))
		{
			if (true == player->bIsAdmin().Get())
			{
				int tribeId;
				const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
				if (true == GetTribeIdOfPlayer(*cmd, &tribeId))
				{
					ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, "TribeId is: {}", tribeId);
				}
				else
				{
					ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, "Couldn't determine TribeId!");
				}
			}
		}
	}


	void RconGetTribeIdOfPlayer(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* )
	{
		if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
		{
			int tribeId;
			if (true == GetTribeIdOfPlayer(rcon_packet->Body, &tribeId))
			{
				FString reply = FString::Format("TribeId is: {}\n", tribeId);
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
			}
			else
			{
				FString reply = "Couldn't determine TribeId!\n";
				rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
			}
		}
	}


	static void InitRconCommands()
	{
		ArkApi::GetCommands().AddRconCommand("ListTribeCooldownSlots", &RconListTribeCooldownSlots);
		ArkApi::GetCommands().AddRconCommand("ListPlayerTribeCooldownSlots", &RconPlayerTribeCooldownSlots);
		ArkApi::GetCommands().AddRconCommand("ResetSlotOfTribe", &RconResetSlotOfTribe);
		ArkApi::GetCommands().AddRconCommand("GetTribeIdOfPlayer", &RconGetTribeIdOfPlayer);
	}

	static void RemoveRconCommands()
	{
		ArkApi::GetCommands().RemoveRconCommand("ListTribeCooldownSlots");
		ArkApi::GetCommands().RemoveRconCommand("ListPlayerTribeCooldownSlots");
		ArkApi::GetCommands().RemoveRconCommand("ResetSlotOfTribe");
		ArkApi::GetCommands().RemoveRconCommand("GetTribeIdOfPlayer");
	}

	static void InitConsoleCommands()
	{
		ArkApi::GetCommands().AddConsoleCommand("ListTribeCooldownSlots", &ConsoleListTribeCooldownSlots);
		ArkApi::GetCommands().AddConsoleCommand("ListPlayerTribeCooldownSlots", &ConsolePlayerTribeCooldownSlots);
		ArkApi::GetCommands().AddConsoleCommand("ResetSlotOfTribe", &ConsoleResetSlotOfTribe);
		ArkApi::GetCommands().AddConsoleCommand("GetTribeIdOfPlayer", &ConsoleGetTribeIdOfPlayer);
	}

	static void RemoveConsoleCommands()
	{
		ArkApi::GetCommands().RemoveConsoleCommand("ListTribeCooldownSlots");
		ArkApi::GetCommands().RemoveConsoleCommand("ListPlayerTribeCooldownSlots");
		ArkApi::GetCommands().RemoveConsoleCommand("ResetSlotOfTribe");
		ArkApi::GetCommands().RemoveConsoleCommand("GetTribeIdOfPlayer");
	}


	static void InitChatCommands()
	{
		DisplaySlots = SlotCooldown::CommandPrefix;
		DisplaySlots = DisplaySlots.Append(SlotCooldown::CommandDisplaySlots);
		ArkApi::GetCommands().AddChatCommand(DisplaySlots, &ChatCommandDisplaySlots);
		
		Log::GetLog()->error(FString(DisplaySlots).ToString());

	}

	static void RemoveChatCommands()
	{
		ArkApi::GetCommands().RemoveChatCommand(DisplaySlots);
	}


	void InitCommands()
	{
		InitChatCommands();
		InitConsoleCommands();
		InitRconCommands();
	}

	void RemoveComands()
	{
		RemoveChatCommands();
		RemoveConsoleCommands();
		RemoveRconCommands();
	}
}
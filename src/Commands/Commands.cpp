/*****************************************************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                                                      *
*                                                                                                                *
* This file is part of the TribeSlotCooldown Plugin for Ark Server API                                           *
*                                                                                                                *
*    This program is free software : you can redistribute it and/or modify                                       *
*    it under the terms of the GNU General Public License as published by                                        *
*    the Free Software Foundation, either version 3 of the License, or                                           *
*    (at your option) any later version.                                                                         *
*                                                                                                                *
*    This program is distributed in the hope that it will be useful,                                             *
*    but WITHOUT ANY WARRANTY; without even the implied warranty of                                              *
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the                                                 *
*    GNU General Public License for more details.                                                                *
*                                                                                                                *
*    You should have received a copy of the GNU General Public License                                           *
*    along with this program.If not, see <https://www.gnu.org/licenses/>.                                        *
*                                                                                                                *
*****************************************************************************************************************/

/**
* \file Commands.cpp
* \author Matthias Birnthaler Matthias.Birnthaler@outlook.com
* \date 15 May 2019
* \brief Implementation of all Commands 
*
*/

/* ================================================ [includes] ================================================ */

#include "Commands.h"
#include "SlotCooldown.h"
#include <sstream>
#include <string>




namespace Commands
{
    /* ============================================ [type definitions] ============================================ */

    /**
    *  \brief time struct
    */
    typedef struct
    {
        int hours;       /**< \brief value for hours */
        int minutes;     /**< \brief value for minutes */
        int secounds;    /**< \brief value for secounds */

    }SlotCooldown_t;


    /* =============================================== [local data] =============================================== */


    /* \brief string for the ChatCommandDisplaySlots chat command */
    static FString StringChatCommandDisplaySlots;


    /* ===================================== [prototype of local functions] ======================================= */

    static SlotCooldown_t GetCooldownTimeOfSlot(int SlotTimer, int ServerRunTime);
    static FString GetStringWithTribeSlotCooldowns(int TribeId);
    static void ChatCommandDisplaySlots(AShooterPlayerController* player, FString* message, int mode);
    static void RconPlayerTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*);
    static void ConsolePlayerTribeCooldownSlots(APlayerController* player, FString* cmd, bool boolean);
    static void RconListTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*);
    static void ConsoleListTribeCooldownSlots(APlayerController* player, FString* cmd, bool boolean);
    static bool ResetSlotOfTribe(const FString& cmd);
    static void ConsoleResetSlotOfTribe(APlayerController* player, FString* cmd, bool boolean);
    static void RconResetSlotOfTribe(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*);
    static bool GetTribeIdOfPlayer(const FString& cmd, int * TribeId);
    static void ConsoleGetTribeIdOfPlayer(APlayerController* player, FString* cmd, bool boolean);
    static void RconGetTribeIdOfPlayer(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*);


    static void InitRconCommands(void);
    static void RemoveRconCommands(void);
    static void InitConsoleCommands(void);
    static void RemoveConsoleCommands(void);
    static void InitChatCommands(void);
    static void RemoveChatCommands(void);

    /* ===================================== [definition of local functions] ====================================== */

    /**
    * \brief Gets a string with the slot cooldowns
    *
    * This functions returns a string with the player slot cooldowns of a given tribe
    *
    * \param[in] SlotTimer the cooldown time of the slot 
    * \param[in] ServerRunTime the current server runntime
    * \return SlotCooldown_t struct with the times 
    */
    static SlotCooldown_t GetCooldownTimeOfSlot(int SlotTimer, int ServerRunTime)
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


    /**
    * \brief Gets a string with the slot cooldowns
    *
    * This functions returns a string with the player slot cooldowns of a given tribe
    *
    * \param[in] TribeId the given tribe
    * \return FString the string with the cooldowns
    */
    static FString GetStringWithTribeSlotCooldowns(int TribeId)
    {
        FString displayString = "";
        std::vector<int> slots;
        auto currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();

        if (true == SlotCooldown::database->IsTribeInDatabase(TribeId))
        {
            slots = SlotCooldown::database->GetTribeSlotsTimer(TribeId);

            SlotCooldown::NormalizeSlots(&slots, currentServerTime);
        }

        displayString = FString::Format(*SlotCooldown::CommandDisplaySlotsMessage, slots.size());
        displayString += "\n";


        for (int i = 0; i < slots.size(); i++)
        {
            SlotCooldown_t timediff = GetCooldownTimeOfSlot(slots[i], (int)currentServerTime);

            FString str = FString::Format(*SlotCooldown::CommandDisplaySlotsMessageSlotCooldown, i + 1, timediff.hours, timediff.minutes, timediff.secounds);
            displayString += "\n";
            displayString += str;

        }
        displayString += "\n";

        return displayString;
    }


    /**
    * \brief Chat command: ChatCommandDisplaySlots
    *
    * This command returns the slots that are on cooldown of a given player(steamId)
    *
    * \param[in] player caller of the command
    * \param[in] message command data
    * \param[in] mode, variable not used
    * \return void
    */
    static void ChatCommandDisplaySlots(AShooterPlayerController* player, FString* message, int mode)
    {
        if (nullptr != player)
        {
            FString displayString = GetStringWithTribeSlotCooldowns(player->TargetingTeamField());
            ArkApi::GetApiUtils().SendNotification(player, FColorList::White, SlotCooldown::MessageDisplaySize, 
				SlotCooldown::MessageDisplayTime, nullptr, *displayString);
        }
    }


    /**
    * \brief Rcon command: RconPlayerTribeCooldownSlots
    *
    * This command returns the slots that are on cooldown of a given player(steamId)
    *
    * \param[in] rcon_connection caller of the command
    * \param[in] rcon_packet rcon data
    * \param[in] UWorld, variable not used
    * \return void
    */
    static void RconPlayerTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
    {
        if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
        {
            TArray<FString> parsed;
            rcon_packet->Body.ParseIntoArray(parsed, L" ", true);

            AShooterPlayerController* searchedplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(std::stoull(*parsed[1]));

            if (nullptr != searchedplayer)
            {
                FString displayString = GetStringWithTribeSlotCooldowns(searchedplayer->TargetingTeamField());
                rcon_connection->SendMessageW(rcon_packet->Id, 0, &displayString);
            }
        }
    }


    /**
    * \brief Console command: ConsolePlayerTribeCooldownSlots
    *
    * This command returns the slots that are on cooldown of a given player(steamId)
    *
    * \param[in] player caller of the command
    * \param[in] cmd the commands data
    * \param[in] boolean, variable not used
    * \return void
    */
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
                    AShooterPlayerController* searchedplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(std::stoull(*parsed[1]));

                    if (nullptr != searchedplayer)
                    {

                        FString displayString = GetStringWithTribeSlotCooldowns(searchedplayer->TargetingTeamField());
                        const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
                        ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, *displayString);
                    }
                }
            }
        }
    }


    /**
    * \brief Rcon command: RconResetSlotOfTribe
    *
    * This command returns the slots that are on cooldown of a given tribe
    *
    * \param[in] rcon_connection caller of the command
    * \param[in] rcon_packet rcon data
    * \param[in] UWorld, variable not used
    * \return void
    */
    static void RconListTribeCooldownSlots(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
    {
        if ((nullptr != rcon_connection) && (nullptr != rcon_packet))
        {
            TArray<FString> parsed;
            rcon_packet->Body.ParseIntoArray(parsed, L" ", true);

            if (true == parsed.IsValidIndex(1))
            {
                FString displayString = GetStringWithTribeSlotCooldowns((int)(std::stoull(*parsed[1])));
                rcon_connection->SendMessageW(rcon_packet->Id, 0, &displayString);
            }
            else
            {
                FString reply = "Couldn't reseted slot!\n";
                rcon_connection->SendMessageW(rcon_packet->Id, 0, &reply);
            }
        }
    }


    /**
    * \brief Console command: ConsoleListTribeCooldownSlots
    *
    * This command returns the slots that are on cooldown of a given tribe(tribeId)
    *
    * \param[in] player caller of the command
    * \param[in] cmd the commands data
    * \param[in] boolean, variable not used
    * \return void
    */
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
                    FString displayString = GetStringWithTribeSlotCooldowns((int)(std::stoull(*parsed[1])));
                    const auto shooter_controller = static_cast<AShooterPlayerController*>(player);
                    ArkApi::GetApiUtils().SendServerMessage(shooter_controller, FColorList::White, *displayString);
                }
            }
        }
    }


    /**
    * \brief Resets a given slot of a given tribe
    *
    * This function resets a given slot that is on cooldown of a given tribe
    *
    * \param[in] cmd the data (tribeid and slotnumber)
    * \return true if the slot can get reseted, otherwise false
    */
    static bool ResetSlotOfTribe(const FString& cmd)
    {
        TArray<FString> parsed;
        cmd.ParseIntoArray(parsed, L" ", true);
        bool result = false;
        std::vector<int> slots;

        if ((true == parsed.IsValidIndex(1))
            && (true == parsed.IsValidIndex(2)))
        {
            int tribeId = (int)(std::stoull(*parsed[1]));
            auto currentServerTime = ArkApi::GetApiUtils().GetWorld()->TimeSecondsField();

            if (true == SlotCooldown::database->IsTribeInDatabase(tribeId))
            {
                slots = SlotCooldown::database->GetTribeSlotsTimer(tribeId);

                /* data needs to be normalized to map it to the users input*/
                SlotCooldown::NormalizeSlots(&slots, currentServerTime);

                auto selectedNr = std::stoull(*parsed[2]);

                Log::GetLog()->error("Tribe: {}, selectedNr: {} ", tribeId, selectedNr);

                /* out of bounds check for the given slot number*/
                if ((false == slots.empty()) && (0 < selectedNr) && (slots.size() + 1 >= selectedNr))
                {
                    /* normalize number to vector index*/
                    slots[selectedNr - 1] = 0;

                    SlotCooldown::database->UpdateSlotTimer(tribeId, slots);
                    result = true;
                }
            }
        }
        return result;
    }


    /**
    * \brief Console command: ConsoleResetSlotOfTribe
    *
    * This command is to reset a player slot of a tribe that is on cooldown
    *
    * \param[in] player caller of the command
    * \param[in] cmd the commands data
    * \param[in] boolean, variable not used
    * \return void
    */
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


    /**
    * \brief Rcon command: RconResetSlotOfTribe
    *
    * This command is to reset a player slot of a tribe that is on cooldown 
    *
    * \param[in] rcon_connection caller of the command
    * \param[in] rcon_packet rcon data
    * \param[in] UWorld, variable not used
    * \return void
    */
    static void RconResetSlotOfTribe(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld*)
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


    /**
    * \brief Gets the tribe id of a given player
    *
    * This function returns the tribe id of a given player(steamid)
    *
    * \param[in] cmd the given steamid
    * \param[out] TribeId the found tribe id
    * \return true if the tribe id can determine otherwise false 
    */
    static bool GetTribeIdOfPlayer(const FString& cmd, int * TribeId)
    {
        TArray<FString> parsed;
        cmd.ParseIntoArray(parsed, L" ", true);
        bool result = false;

        if (nullptr != TribeId)
        {

            if (true == parsed.IsValidIndex(1))
            {
                AShooterPlayerController* givenplayer = ArkApi::GetApiUtils().FindPlayerFromSteamId(std::stoull(*parsed[1]));

                if (nullptr != givenplayer)
                {
                    *TribeId = givenplayer->TargetingTeamField();
                    result = true;
                }
            }
        }
        return result;
    }


    /**
    * \brief Console command: GetTribeIdOfPlayer
    *
    * This command returns the tribe id of a given player(steamid)
    *
    * \param[in] player caller of the command
    * \param[in] cmd the commands data
    * \param[in] boolean, variable not used
    * \return void
    */
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


    /**
    * \brief Rcon command: GetTribeIdOfPlayer
    *
    * This command returns the tribe id of a given player(steamid)
    *
    * \param[in] rcon_connection caller of the command
    * \param[in] rcon_packet rcon data
    * \param[in] UWorld, variable not used
    * \return void
    */
    static void RconGetTribeIdOfPlayer(RCONClientConnection* rcon_connection, RCONPacket* rcon_packet, UWorld* )
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


    /**
    * \brief Initialisation of rcon commands
    *
    * This function initializes all rcon commands
    *
    * \return void
    */
    static void InitRconCommands(void)
    {
        ArkApi::GetCommands().AddRconCommand("ListTribeCooldownSlots", &RconListTribeCooldownSlots);
        ArkApi::GetCommands().AddRconCommand("ListPlayerTribeCooldownSlots", &RconPlayerTribeCooldownSlots);
        ArkApi::GetCommands().AddRconCommand("ResetSlotOfTribe", &RconResetSlotOfTribe);
        ArkApi::GetCommands().AddRconCommand("GetTribeIdOfPlayer", &RconGetTribeIdOfPlayer);
    }


    /**
    * \brief Remove of rcon commands
    *
    * This function removes all rcon commands
    *
    * \return void
    */
    static void RemoveRconCommands(void)
    {
        ArkApi::GetCommands().RemoveRconCommand("ListTribeCooldownSlots");
        ArkApi::GetCommands().RemoveRconCommand("ListPlayerTribeCooldownSlots");
        ArkApi::GetCommands().RemoveRconCommand("ResetSlotOfTribe");
        ArkApi::GetCommands().RemoveRconCommand("GetTribeIdOfPlayer");
    }


    /**
    * \brief Initialisation of console commands
    *
    * This function initializes all console commands
    *
    * \return void
    */
    static void InitConsoleCommands(void)
    {
        ArkApi::GetCommands().AddConsoleCommand("ListTribeCooldownSlots", &ConsoleListTribeCooldownSlots);
        ArkApi::GetCommands().AddConsoleCommand("ListPlayerTribeCooldownSlots", &ConsolePlayerTribeCooldownSlots);
        ArkApi::GetCommands().AddConsoleCommand("ResetSlotOfTribe", &ConsoleResetSlotOfTribe);
        ArkApi::GetCommands().AddConsoleCommand("GetTribeIdOfPlayer", &ConsoleGetTribeIdOfPlayer);
    }


    /**
    * \brief Remove of console commands
    *
    * This function removes all console commands
    *
    * \return void
    */
    static void RemoveConsoleCommands(void)
    {
        ArkApi::GetCommands().RemoveConsoleCommand("ListTribeCooldownSlots");
        ArkApi::GetCommands().RemoveConsoleCommand("ListPlayerTribeCooldownSlots");
        ArkApi::GetCommands().RemoveConsoleCommand("ResetSlotOfTribe");
        ArkApi::GetCommands().RemoveConsoleCommand("GetTribeIdOfPlayer");
    }


    /**
    * \brief Initialisation of chat commands
    *
    * This function initializes all chat commands
    *
    * \return void
    */
    static void InitChatCommands(void)
    {
        StringChatCommandDisplaySlots = SlotCooldown::CommandPrefix;
        StringChatCommandDisplaySlots = StringChatCommandDisplaySlots.Append(SlotCooldown::CommandDisplaySlots);
        ArkApi::GetCommands().AddChatCommand(StringChatCommandDisplaySlots, &ChatCommandDisplaySlots);
        
    }


    /**
    * \brief Remove of chat commands
    *
    * This function removes all chat commands
    *
    * \return void
    */
    static void RemoveChatCommands(void)
    {
        ArkApi::GetCommands().RemoveChatCommand(StringChatCommandDisplaySlots);
    }


    /* ===================================== [definition of global functions] ===================================== */

    /**
    * \brief Initialisation of commands
    *
    * This function initializes all commands
    *
    * \return void
    */
    void InitCommands(void)
    {
        InitChatCommands();
        InitConsoleCommands();
        InitRconCommands();
    }


    /**
    * \brief Remove of commands
    *
    * This function removes all commands
    *
    * \return void
    */
    void RemoveComands(void)
    {
        RemoveChatCommands();
        RemoveConsoleCommands();
        RemoveRconCommands();
    }
}

/* =================================================[end of file]================================================= */

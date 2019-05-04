#include "test.h"

#include <iostream>
#include <filesystem>
#include <chrono>

#include "json.hpp"
#include <iostream>
#include "gtest/gtest.h"
#include "DBHandler.h"		
#include "moc.h"
#include "Tools.h"

#include "SlotCooldown.h"
					    
TEST(Database, CreateDatabase)
{
	Log::ResetLogCount();

	std::string current_path = std::filesystem::current_path().u8string();

	std::unique_ptr<DBHandler> database;
	const std::string db_path = current_path +"\\database.db";
	database = std::make_unique<DBHandler>(db_path);
	
	/*database created*/
	ASSERT_TRUE(std::filesystem::exists(db_path));

	/*a error occurred exception throwed, etc.*/
	ASSERT_EQ(0 , Log::GetLogCount());
}																										    
																										    

TEST(Database, AddTribe)
{
	Log::ResetLogCount();
	std::string current_path = std::filesystem::current_path().u8string();

	/*Create Database*/
	std::unique_ptr<DBHandler> database;
	const std::string db_path = current_path + "\\database.db";
	database = std::make_unique<DBHandler>(db_path);

	database->AddTribe(123456);

	/* no error should get called */
	ASSERT_EQ(0, Log::GetLogCount());
}



TEST(Database, UpdateSlotTimer)
{
	Log::ResetLogCount();
	std::string current_path = std::filesystem::current_path().u8string();

	/*Create Database*/
	std::unique_ptr<DBHandler> database;
	const std::string db_path = current_path + "\\database.db";
	database = std::make_unique<DBHandler>(db_path);

	std::vector<int> slotTimer = {10, 20 ,30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140 ,150};

	database->UpdateSlotTimer(123456, slotTimer);

	/* no error should get called */
	ASSERT_EQ(0, Log::GetLogCount());
}


TEST(Database, IsTribeInDatabase)
{
	Log::ResetLogCount();
	std::string current_path = std::filesystem::current_path().u8string();

	/*Create Database*/
	std::unique_ptr<DBHandler> database;
	const std::string db_path = current_path + "\\database.db";
	database = std::make_unique<DBHandler>(db_path);

	bool IsTribeinDb0 = false;
	bool IsTribeinDb1 = true;


	IsTribeinDb0 = database->IsTribeInDatabase(123456);
	IsTribeinDb1 = database->IsTribeInDatabase(654321);


	/* no error should get called */
	ASSERT_EQ(0, Log::GetLogCount());
	ASSERT_EQ(IsTribeinDb0, true);
	ASSERT_EQ(IsTribeinDb1, false);
}


TEST(Database, GetTribeSlotsTimer)
{
	Log::ResetLogCount();
	std::string current_path = std::filesystem::current_path().u8string();

	/*Create Database*/
	std::unique_ptr<DBHandler> database;
	const std::string db_path = current_path + "\\database.db";
	database = std::make_unique<DBHandler>(db_path);

	std::vector<int> slotTimer = { 10, 20 ,30, 40, 50};
	database->UpdateSlotTimer(123456, slotTimer);

	std::vector<int> slotTimerfromDB;

	slotTimerfromDB = database->GetTribeSlotsTimer(123456);

	/* no error should get called */
	ASSERT_EQ(0, Log::GetLogCount());
	ASSERT_EQ(slotTimer[0], slotTimerfromDB[0]);
	ASSERT_EQ(slotTimer[1], slotTimerfromDB[1]);
	ASSERT_EQ(slotTimer[2], slotTimerfromDB[2]);
	ASSERT_EQ(slotTimer[3], slotTimerfromDB[3]);
	ASSERT_EQ(slotTimer[4], slotTimerfromDB[4]);
	ASSERT_EQ(slotTimer.size(), slotTimerfromDB.size());

}

TEST(SlotCooldown, GetFreeSlot1)
{
	std::vector<int> TestSlotsTimer;
	int TestServerRunTime = 5000;
	bool RetValue1 = true;
	int FoundSlot1 = -1; 

	bool RetValue2 = true;
	int FoundSlot2 = -1;

	bool RetValue3 = true;
	int FoundSlot3 = -1;

	nlohmann::json jsonconfig; 
	jsonconfig["General"]["Tribelimit"] = 12;

	SlotCooldown::config = jsonconfig;


	RetValue1 = SlotCooldown::GetFreeSlot(&FoundSlot1, TestSlotsTimer, 5000);

	TestSlotsTimer = { 1500, 2000, 15000  ,254016000 };

	RetValue2 = SlotCooldown::GetFreeSlot(&FoundSlot2, TestSlotsTimer, 5000);

	TestSlotsTimer = { 10, 20, 30, 40, 50, 60, 70, 80, 90 ,100, 110}; /* full slot -> for 12 man tribe limit*/

	RetValue3 = SlotCooldown::GetFreeSlot(&FoundSlot3, TestSlotsTimer, 1);

	ASSERT_EQ(RetValue1, true);  /* no slot found TestSlotsTimer is empty -> tribe has no slots on cooldown */
	ASSERT_EQ(FoundSlot1, 0);  /* found slot is 0*/

	ASSERT_EQ(RetValue2, true);  /* no slot found TestSlotsTimer is empty */
	ASSERT_EQ(FoundSlot2, 0); /* first slot */


	ASSERT_EQ(RetValue3, false);  /* no slot found TestSlotsTimer is empty */
	ASSERT_EQ(FoundSlot3, -1); /* first slot */
}


TEST(SlotCooldown, GetCooldownTimeOfSlot)
{
	SlotCooldown_t RetValue0;
	SlotCooldown_t RetValue1;
	SlotCooldown_t RetValue2;
	SlotCooldown_t RetValue3;
	SlotCooldown_t RetValue4;

	RetValue0 = SlotCooldown::GetCooldownTimeOfSlot(3661, 0);
	RetValue1 = SlotCooldown::GetCooldownTimeOfSlot(100, 200);
	RetValue2 = SlotCooldown::GetCooldownTimeOfSlot(100, 100);
	RetValue3 = SlotCooldown::GetCooldownTimeOfSlot(101, 100);
	RetValue4 = SlotCooldown::GetCooldownTimeOfSlot(21955, 1000);

	ASSERT_EQ(RetValue0.hours, 1);
	ASSERT_EQ(RetValue0.minutes, 1);
	ASSERT_EQ(RetValue0.secounds, 1);

	ASSERT_EQ(RetValue1.hours, 0);
	ASSERT_EQ(RetValue1.minutes, 0);
	ASSERT_EQ(RetValue1.secounds, 0);

	ASSERT_EQ(RetValue2.hours, 0);
	ASSERT_EQ(RetValue2.minutes, 0);
	ASSERT_EQ(RetValue2.secounds, 0);

	ASSERT_EQ(RetValue3.hours, 0);
	ASSERT_EQ(RetValue3.minutes, 0);
	ASSERT_EQ(RetValue3.secounds, 1);

	ASSERT_EQ(RetValue4.hours, 5);
	ASSERT_EQ(RetValue4.minutes, 49);
	ASSERT_EQ(RetValue4.secounds, 15);

}


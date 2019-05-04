#include "DBHandler.h"
#include "Tools.h"
#include "API/ARK/Ark.h"


DBHandler::DBHandler(const std::string& path) : mdb(path.empty() ? ArkApi::Tools::GetCurrentDir() + "/ArkApi/Plugins/ArkShop/ArkShop.db" : path)
{		

	try
	{
		mdb << "PRAGMA journal_mode=WAL;";

		mdb << "create table if not exists TribeSlots ("
			   "TribeId integer primary key autoincrement not null,"
			   "SlotsTimer BLOB"
			   ");";
	}
	catch (const std::exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}
};



void DBHandler::AddTribe(const int TribeId)
{
	try
	{
		mdb << "INSERT INTO TribeSlots VALUES (?,?)" << TribeId << 0;
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}
};


std::vector<int> DBHandler::GetTribeSlotsTimer(const int TribeId)
{

	std::vector<int> slots;

	try
	{
		mdb << "SELECT SlotsTimer from TribeSlots where TribeId = ?;" << TribeId >> slots;
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
	}

	return slots;
}

bool DBHandler::UpdateSlotTimer(const int TribeId, const std::vector<int> SlotTimer)
{
	try
	{
		mdb << "UPDATE TribeSlots SET SlotsTimer = ? WHERE TribeId = ?;" << SlotTimer << TribeId;
		return true;
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
		return false;
	}
}

bool  DBHandler::IsTribeInDatabase(int TribeId)
{
	int count = 0;
	bool result = false;

	try 
	{
		mdb << "SELECT count(1) FROM TribeSlots WHERE TribeId = ?;" << TribeId >> count;
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());
		return false;
	}

	if (count != 0)
	{
		result = true;
	}


	return result;
}


void  DBHandler::DeleteTribe(int TribeId)
{
	try
	{
		mdb << "DELETE FROM TribeSlots WHERE TribeId = ?;" << TribeId;
	}
	catch (const sqlite::sqlite_exception& exception)
	{
		Log::GetLog()->error("({} {}) Unexpected DB error {}", __FILE__, __FUNCTION__, exception.what());

	}
}

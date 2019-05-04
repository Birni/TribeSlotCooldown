#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <sqlite_modern_cpp.h>

#include "Tools.h"
#include "Requests.h"
#include "ICommands.h"
#include "IHooks.h"
#include "API/ARK/Ark.h"

class DBHandler
{
private:
	sqlite::database mdb;

public:
	virtual ~DBHandler() = default;
	explicit DBHandler(const std::string& path);


	void AddTribe(const int TribeId);

	std::vector<int> GetTribeSlotsTimer(const int TribeId);
	bool UpdateSlotTimer(const int TribeId, const std::vector<int> SlotTimer);
	bool IsTribeInDatabase(int TribeId);
	void DeleteTribe(int TribeId);

};



#endif /* DBHANDLER_H */
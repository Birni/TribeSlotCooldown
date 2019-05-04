#include <string>
#include "Tools.h"
#include "moc.h"

namespace ArkApi
{

	Tools * Tools::instance()
	{
		if (!_instance)
			_instance = new Tools();
		return _instance;
	}

	std::string Tools::GetCurrentDir()
	{
		return "";
	};

	std::string Tools::Utf8Decode(std::string string)
	{
		return string;
	};

	Tools * Tools::_instance = 0;


	//ApiUtils
	ApiUtils * ApiUtils::instance()
	{
		if (!_instance)
			_instance = new ApiUtils();
		return _instance;
	}

	World_t* ApiUtils::GetWorld()
	{
		return &mWorld;
	};

	ApiUtils * ApiUtils::_instance = 0;


	ApiUtils GetApiUtils()
	{
		ApiUtils* _ApiUtils = new ApiUtils();

		return *_ApiUtils;
	};

	AShooterPlayerController* ApiUtils::FindPlayerFromSteamId(int PlayerID)
	{
		return AShooterPlayerController::instance();
	};

}


Log * Log::instance()
{
	if (!_instance)
	{
		_instance = new Log();
		_instance->logcount = 0;
	}

	return _instance;

}

void Log::ResetLogCount()
{
	Log::instance()->logcount = 0;
};

int Log::GetLogCount()
{
	return instance()->logcount;
};

Log * Log::GetLog()
{
	return Log::instance();
};


Log Log::Get()
{
	return *_instance;
};

void Log::Init(std::string log)
{
	instance()->logcount++;
};

void Log::error(std::string log)
{
	instance()->logcount++;
};
void Log::error(std::string log1, std::string log2)
{
	instance()->logcount++;
};
void Log::error(std::string log1, std::string log2, int i)
{
	instance()->logcount++;
};
void Log::error(std::string log1, std::string log2, std::string log3, std::string log4)
{
	instance()->logcount++;
};


Log * Log::_instance = 0;



#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include "moc.h"

namespace ArkApi
{
	typedef struct
	{
		int TimeSecondsField() { return 1500; };
	}World_t;

	static class  Tools
	{
	private:
		static Tools* _instance;


	public:
		static Tools* instance();
		static std::string GetCurrentDir();
		static std::string Utf8Decode(std::string string);

	
	};

	static class  ApiUtils
	{
	private:
		World_t mWorld;
		static ApiUtils* _instance;


	public:
		static ApiUtils* instance();

		AShooterPlayerController* FindPlayerFromSteamId(int PlayerID);

		//template <typename T>
		void SendNotification(AShooterPlayerController * player, int color, int textsize, int duration, int *ptr, std::wstring msg) { return; };

		World_t* GetWorld();
	};

	ApiUtils GetApiUtils();

}




class Log
{
private: 
	static Log* _instance;
    int logcount=0;


public:
	static Log* instance();
	static void ResetLogCount();
	static int GetLogCount();

	static Log * GetLog();
	static Log  Get();

	void Init(std::string log);
	void error(std::string log);
	void error(std::string log1 , std::string log2);
	void error(std::string log1, std::string log2, int i);
	void error(std::string log1, std::string log2, std::string log3, std::string log4);
};




#endif

#ifndef MOC_H
#define MOC_H

#include <string>



#define DLL_PROCESS_ATTACH 1 
#define DLL_PROCESS_DETACH 2 

#define APIENTRY
#define BOOL bool

#define TRUE true
#define FALSE false 

typedef char HMODULE;
typedef unsigned long       DWORD;

typedef void* LPVOID;


#define DECLARE_HOOK(x) 



typedef unsigned __int64 DWORD64, *PDWORD64;



class FString
{
private: 
	std::wstring mstring;

public:

	FString(const wchar_t* string);

	std::string ToString();

	FString(const char* string);
	FString() = default;
	FString(FString&&) = default;
	FString(const FString&) = default;
	FString& operator=(FString&&) = default;
	FString& operator=(const FString&) = default;

	typedef wchar_t TCHAR;
	const TCHAR* operator*() const { return (wchar_t const) 0; };

};



class UClass 
{

};


class UObject
{
public:
	static UClass* GetPrivateStaticClass();

};


typedef char UPrimalItem;

template <typename T>
struct TSubclassOf
{
	TSubclassOf()
		: uClass(nullptr)
	{
	}

	TSubclassOf(UClass* uClass)
		: uClass(uClass)
	{
	}

	UClass* uClass;
};

typedef struct Globals
{
	static UObject* StaticLoadObject(UClass* ObjectClass, UObject* InOuter, const wchar_t* InName, const wchar_t* Filename,
		unsigned int LoadFlags, DWORD64 Sandbox, bool bAllowObjectReconciliation)
	{
		return nullptr;
	}
};


struct FPrimalPlayerDataStruct
{
	FString tribe;

	FString PlayerNameField() { return tribe; };
	int TribeIDField() { return 12345; };
	int PlayerDataIDField() { return 987123; };


};




class AShooterPlayerController
{

private:
	static AShooterPlayerController* _instance;

public:
	static AShooterPlayerController* instance();

};


class Container
{
private:
	static Container* _instance;
	static int _num;

public:
	static Container* instance();
	static int Num();

};


class FTribeData
{
private:
	static FTribeData* _instance;
	Container _container;

public:

	static FTribeData* instance();
	Container MembersPlayerDataIDField();
	int TribeIDField();
};


class AShooterPlayerState
{
private:
	static AShooterPlayerState* _instance;
	FPrimalPlayerDataStruct* _FPrimalPlayerDataStruct;
	FTribeData* _FTribeData;

public:
	static AShooterPlayerState* instance();
	FTribeData* MyTribeDataField();
	FPrimalPlayerDataStruct* MyPlayerDataStructField();

};

class APlayerController
{
private:
	static APlayerController* _instance;


public:

	static APlayerController* instance();

};


class FColorList
{
public:

	static const int Red =1;
};



extern void AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation_original(AShooterPlayerState* AShooterPlayerState, int PlayerIndexInTribe);
extern bool AShooterPlayerState_AddToTribe_original(AShooterPlayerState* AShooterPlayerState, FTribeData * MyNewTribe, bool bMergeTribe, bool bForce, bool bIsFromInvite, APlayerController *InviterPC);
extern void AShooterPlayerState_SendTribeInviteData_Implementation_original(AShooterPlayerState * AShooterPlayerState, FTribeData TribeInviteData);
extern void AShooterPlayerState_ServerRequestLeaveTribe_Implementation_original(AShooterPlayerState *_this);

extern void Hook_AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation(AShooterPlayerState* AShooterPlayerState, int PlayerIndexInTribe);
extern bool Hook_AShooterPlayerState_AddToTribe(AShooterPlayerState* AShooterPlayerState, FTribeData * MyNewTribe, bool bMergeTribe, bool bForce, bool bIsFromInvite, APlayerController *InviterPC);
extern void Hook_AShooterPlayerState_SendTribeInviteData_Implementation(AShooterPlayerState * AShooterPlayerState, FTribeData TribeInviteData);
extern void Hook_AShooterPlayerState_ServerRequestLeaveTribe_Implementation(AShooterPlayerState *_this);

#endif 
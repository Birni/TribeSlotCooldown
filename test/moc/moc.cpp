#include "moc.h"



FString::FString(const wchar_t* string)
{
	mstring = std::wstring(string);
};

FString::FString(const char * string)
{
	
};

std::string FString::ToString() 
{ 
	return "test"; 
};






AShooterPlayerState * AShooterPlayerState::instance()
{
	if (!_instance)
		_instance = new AShooterPlayerState();
	return _instance;
}
FPrimalPlayerDataStruct* AShooterPlayerState::MyPlayerDataStructField()
{
	return this->_FPrimalPlayerDataStruct;
};

FTribeData* AShooterPlayerState::MyTribeDataField()
{
	return _FTribeData;
};


AShooterPlayerState * AShooterPlayerState::_instance = 0;



AShooterPlayerController * AShooterPlayerController::instance()
{
	if (!_instance)
		_instance = new AShooterPlayerController();
	return _instance;
}

AShooterPlayerController * AShooterPlayerController::_instance = 0;



FTribeData * FTribeData::instance()
{
	if (!_instance)
		_instance = new FTribeData();
	return _instance;
}

int FTribeData::TribeIDField()
{
	return 123456;
};

Container  FTribeData::MembersPlayerDataIDField()
{
	return _container;
};

FTribeData * FTribeData::_instance = 0;


APlayerController * APlayerController::instance()
{
	if (!_instance)
		_instance = new APlayerController();
	return _instance;
}

APlayerController * APlayerController::_instance = 0;


Container * Container::instance()
{
	if (!_instance)
		_instance = new Container();
	return _instance;
}

int Container::Num()
{
	return 5;
}

Container * Container::_instance = 0;





void AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation_original(AShooterPlayerState* AShooterPlayerState, int PlayerIndexInTribe){};
bool AShooterPlayerState_AddToTribe_original(AShooterPlayerState* AShooterPlayerState, FTribeData * MyNewTribe, bool bMergeTribe, bool bForce, bool bIsFromInvite, APlayerController *InviterPC) { return false; };
void AShooterPlayerState_SendTribeInviteData_Implementation_original(AShooterPlayerState * AShooterPlayerState, FTribeData TribeInviteData) {};
void AShooterPlayerState_ServerRequestLeaveTribe_Implementation_original(AShooterPlayerState *_this) {};
 

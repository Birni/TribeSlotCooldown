/****************************************************************************
* Copyright (C) 2019 by Matthias Birnthaler                                 *
*                                                                           *
* This file is part of the TribeSlotCooldown Plugin for Ark Server API *
*                                                                           *
*   The TribeSlotCooldown Plugin is free software: you can             *
*	redistribute it and/or modify it under the terms of the GNU Lesser      *
*	General Public License as published by the Free Software Foundation,    *
*	either version 3 of the License, or (at your option) any later version. *
*                                                                           *
*   The TribeSlotCooldown Plugin is distributed in the hope that it    *
*   will be useful, but WITHOUT ANY WARRANTY; without even the implied      *
* 	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.        *
* 	See the GNU Lesser General Public License for more details.             *
*                                                                           *
****************************************************************************/

/**
* \file Hooks.h
* \author Matthias Birnthaler
* \date 19 March 2019
* \brief Header file of declared Hooks
*
*/

#ifndef HOOKS_H
#define HOOKS_H

#include "Tools.h"
#include "Requests.h"
#include "ICommands.h"
#include "IHooks.h"
#include "API/ARK/Ark.h"

/**< \brief declare of AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation Hook */
DECLARE_HOOK(AShooterPlayerState_AddToTribe, bool, AShooterPlayerState*, FTribeData *, bool, bool, bool, APlayerController *);

DECLARE_HOOK(AShooterPlayerState_ServerRequestLeaveTribe_Implementation, void, AShooterPlayerState*);

DECLARE_HOOK(AShooterPlayerState_ServerRequestRemovePlayerIndexFromMyTribe_Implementation, void, AShooterPlayerState*, int);



/**
* \brief Initialisation of needed Hooks
*
* This function initialise all needed Hooks and read the relevant information from the json config file.
*
* \return void
*/
extern void InitHooks(void);


/**
* \brief Cancellation of needed Hooks
*
* This function removes all needed Hooks.
*
* \return void
*/
extern void RemoveHooks(void);

#endif /* HOOKS_H */
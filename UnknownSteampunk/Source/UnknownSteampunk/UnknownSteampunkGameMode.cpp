// Copyright Epic Games, Inc. All Rights Reserved.

#include "UnknownSteampunkGameMode.h"
#include "UnknownSteampunkCharacter.h"

AUnknownSteampunkGameMode::AUnknownSteampunkGameMode()
{
	// Set default pawn class to our character
	DefaultPawnClass = AUnknownSteampunkCharacter::StaticClass();	
}

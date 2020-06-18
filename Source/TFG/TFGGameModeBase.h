// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TFGGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TFG_API ATFGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATFGGameModeBase();						//	Constructor

	UFUNCTION(BlueprintCallable)
		void Connect();						//	Function to connect to IP
	UFUNCTION(BlueprintCallable)
		FString GetUrl();					//	Get URL to connect to
	UFUNCTION(BlueprintCallable)
		void SetUrl(FString mUrl);			//	Set new URL to connect to

		FString url;						//	Composer IP/url

};

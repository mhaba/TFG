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
		void Connect(FString url);			//	Function to connect to IP

	UPROPERTY(BlueprintReadWrite)
		FString url;						//	Composer IP/url
	
	FString GetUrl();
	void SetUrl(FString mUrl);

};

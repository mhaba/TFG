// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Bg2DownloadParser.generated.h"

/**
 * 
 */
UCLASS()
class BG2FILEDOWNLOADER_API UBg2DownloadParser : public UObject
{
	GENERATED_BODY()
	

public:

	static bool SceneParser(const FString& ScenePath, TArray<FString>& Result);

	static bool ModelParser(const FString& ModelPath, TArray<FString>& Result);

	static bool MaterialParser(const TSharedPtr<FJsonObject>& JsonObject, TArray<FString>& Result);
	
private:


	
};

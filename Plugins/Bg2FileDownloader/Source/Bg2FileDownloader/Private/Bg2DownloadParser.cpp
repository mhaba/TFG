// Fill out your copyright notice in the Description page of Project Settings.


#include "Bg2DownloadParser.h"

#include "..\..\..\..\Bg2UnrealTools\Source\Bg2UnrealTools\Public\Bg2Scene.h"
#include "..\..\..\..\Bg2UnrealTools\Source\Bg2UnrealTools\Public\Bg2Model.h"
#include "..\..\..\..\Bg2UnrealTools\Source\Bg2UnrealTools\Public\Bg2Material.h"

bool UBg2DownloadParser::SceneParser(const FString& ScenePath, TArray<FString>& Result) {
	bool bIsParsed = false;

	UBg2Scene::GetExternalResources(ScenePath, Result);

	return bIsParsed;
}

bool UBg2DownloadParser::ModelParser(const FString& ModelPath, TArray<FString>& Result) {
	bool bIsParsed = false;

	UBg2Model::GetExternalResources(ModelPath, Result);

	return bIsParsed;
}

bool UBg2DownloadParser::MaterialParser(const TSharedPtr<FJsonObject>& JsonObject, TArray<FString>& Result) {
	bool bIsParsed = false;

	UBg2Material::GetExternalResources(JsonObject, Result);

	return bIsParsed;
}


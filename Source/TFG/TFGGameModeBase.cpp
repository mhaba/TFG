// Copyright Epic Games, Inc. All Rights Reserved.


#include "TFGGameModeBase.h"

ATFGGameModeBase::ATFGGameModeBase() {
	FString mUrl = GetUrl();
}

void ATFGGameModeBase::Connect(FString mUrl) {

}

FString ATFGGameModeBase::GetUrl() {
	this->url;
}

void ATFGGameModeBase::SetUrl(FString mUrl) {
	this->url = mUrl;
}
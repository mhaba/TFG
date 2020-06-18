// Copyright Epic Games, Inc. All Rights Reserved.


#include "TFGGameModeBase.h"

#include "Runtime/Online/HTTP/Public/HttpManager.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Online/HTTP/Public/HttpRetrySystem.h"
#include "Runtime/Online/HTTP/Public/Interfaces/IHttpResponse.h"

#include "Bg2SceneLoader.h"
#include "Bg2Scene.h"

using namespace FHttpRetrySystem;

ATFGGameModeBase::ATFGGameModeBase() {
	ATFGGameModeBase::url = "192.168.0.18::8080";
}

void ATFGGameModeBase::Connect() {
	// Create the IHttpRequest object from FHttpModule singleton interface.
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	FHttpRequestCompleteDelegate& delegate = request->OnProcessRequestComplete();

	delegate.BindLambda(
		[](FHttpRequestPtr request, FHttpResponsePtr response, bool success) -> void {
			UE_LOG(LogTemp, Warning, TEXT("Http Response: %d, %s"),
				request->GetResponse()->GetResponseCode(),
				*request->GetResponse()->GetContentAsString());
		}
	);

	request->SetURL(GetUrl());
	request->SetVerb(TEXT("GET"));
	request->ProcessRequest();
}

FString ATFGGameModeBase::GetUrl() {
	return this->url;
}

void ATFGGameModeBase::SetUrl(FString mUrl) {
	this->url = mUrl;
}


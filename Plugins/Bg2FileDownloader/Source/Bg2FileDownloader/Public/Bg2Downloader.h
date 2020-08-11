// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "UObject/NoExportTypes.h"
#include "Interfaces/IHttpRequest.h"

#include "Bg2Downloader.generated.h"

UCLASS(Blueprintable)
class BG2FILEDOWNLOADER_API UBg2Downloader : public UObject
{
	GENERATED_BODY()

public:
	//bool* bAndroidReady;

public:
	/**
	* Downloads a file and saves it.
	*
	* @param URL		URL
	* @param Bg2Thing	Object requested to download.
	* @return			Returns itself.
	*/
	UFUNCTION(BlueprintCallable, Category = "Bg2Downloader")
	static UBg2Downloader* Download(FString URL);

	/** Checks if Android device is ready to work with */
	/*UFUNCTION(BlueprintCallable, Category = "Bg2Downloader")
	static bool CheckAndroidReadiness();
	*/

	FString GetBaseURL();

	FString GetActualURL();

private:
	void Start(FString URL);

	/** Handles requests coming from the URL */
	void HandleRequest(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded);

	/** Shows download progress */
	void OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved);

	void SetBaseURL(FString URL);

	void SetActualURL(FString URL);

};

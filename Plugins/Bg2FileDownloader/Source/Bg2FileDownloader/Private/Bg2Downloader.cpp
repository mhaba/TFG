// Fill out your copyright notice in the Description page of Project Settings.


#include "Bg2Downloader.h"

#include "Modules/ModuleManager.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"

#include "Bg2DownloadParser.h"


FString mScene = "/Test.txt";
FString mActualURL = "http://192.168.0.18:8080";
FString mBaseURL = "http://192.168.0.18:8080";
FString mItem;

bool bIsReady = false;
bool bSceneParsed = false;

//UBg2DownloadParser* DownloadPars = NewObject<UBg2DownloadParser>();

UBg2Downloader* UBg2Downloader::Download(FString URL) {
	UBg2Downloader* DownloadTask = NewObject<UBg2Downloader>();
	DownloadTask->Start(URL);

	return DownloadTask;
}

void UBg2Downloader::Start(FString URL) {
	SetActualURL(URL);

	if (GetActualURL().Equals(GetBaseURL())) {
		URL += mScene;
		SetActualURL(URL);

	}

	// Create the IHttpRequest object from FHttpModule singleton interface.
	TSharedRef<IHttpRequest> request = FHttpModule::Get().CreateRequest();
	request->OnProcessRequestComplete().BindUObject(this, &UBg2Downloader::HandleRequest);
	request->SetURL(URL);
	request->SetVerb(TEXT("GET"));

	//	Start Processing the request.
	request->ProcessRequest();
	//	Prevent the object to be deleted during garbage collection.
	AddToRoot();
}

void UBg2Downloader::HandleRequest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess) {

	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	FString mURL = GetActualURL();

	if (bSuccess && Response.IsValid() && Response->GetContentLength() > 0) {

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileManager* FileManager = &IFileManager::Get();

		//	Create save directory path
		FString savePath = FPaths::ProjectSavedDir();
		FString filename = savePath;

		if (mURL.Contains(mScene)) {
			filename += mScene;
			TArray<FString> sceneResources;


		}
		else {

		}

		if (!PlatformFile.DirectoryExists(*savePath) || !FileManager->DirectoryExists(*savePath)) {
			//	Create directory
			PlatformFile.CreateDirectoryTree(*savePath);
		}

		//	Create the file
		IFileHandle* fileHandler = PlatformFile.OpenWrite(*filename);
		if (fileHandler) {
			//	Write the new file from the response
			fileHandler->Write(Response->GetContent().GetData(), Response->GetContentLength());
			//	Close and finish rhe operation
			delete fileHandler;
		}
	}
}

void UBg2Downloader::DoLoadResources(const FString& Path, TArray<FString>& Result) {
	//DownloadPars->SceneParser(Path, Result);
	UBg2DownloadParser::SceneParser(Path, Result);

	FString mURL;

	for (int32 i = 0; i < Result.Num(); ++i)
	{
		//UE_LOG(Bg2Tools, Display, TEXT("Scene external resource: %s"), *Result[i]);
		mURL = GetBaseURL() + *Result[i];
		Start(mURL);
	}
}

void UBg2Downloader::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved)
{
	//	TODO:	USARLO EN UNA BARRA DE CARGA ?
	int32 size = HttpRequest->GetContentLength();
}

FString UBg2Downloader::GetBaseURL()
{
	return mBaseURL;
}

FString UBg2Downloader::GetActualURL()
{
	return mActualURL;
}

void UBg2Downloader::SetBaseURL(FString URL)
{
	mBaseURL = URL;
}

void UBg2Downloader::SetActualURL(FString URL)
{
	mActualURL = URL;
}
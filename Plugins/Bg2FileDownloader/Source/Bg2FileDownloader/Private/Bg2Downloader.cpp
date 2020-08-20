// Fill out your copyright notice in the Description page of Project Settings.


#include "Bg2Downloader.h"

#include "Modules/ModuleManager.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"
#include "AndroidPermissionFunctionLibrary.h"
#include "AndroidPermissionCallbackProxy.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/GameplayStatics.h"
#include "Internationalization/Regex.h"

#include "Bg2DownloadParser.h"


const FString mSceneName = "/ExampleScene.vitscnj";

FString mActualURL = "http://192.168.0.18:8080";
FString mBaseURL = "http://192.168.0.18:8080";
TArray<FString> mResources;

bool bIsReady = false;
bool bSceneParsed = false;

//UBg2DownloadParser* DownloadPars = NewObject<UBg2DownloadParser>();

UBg2Downloader* UBg2Downloader::Download(FString URL) {
	Start(URL, [&]() {
		// Se han descargado todos los recursos, lanzamos un evento para informar de que se ha terminado la descarga
		this->OnDownloadFinished.Broadcast();
		// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Download completed"));
	});


	//	Maybe Start() should return a bool, so we could see if the funcition was
	//	a success or a failure.
	return this;
}

void UBg2Downloader::Start(FString URL, std::function<void ()> onComplete) {
	mOnComplete = onComplete;

	SetActualURL(URL);

	//	If these two URLs are equal then this is the scene's request.
	//if (GetActualURL().Equals(GetBaseURL())) {
	
	const FRegexPattern containsFilePattern(TEXT(".*[^/]/{1}([a-zA-Z0-9\\-_\\%]+\\.[a-zA-Z0-9\\-_]+)$"));
	FString ActualUrl = GetActualURL();
	FRegexMatcher fileMatcher(containsFilePattern, ActualUrl);
	
	if (!fileMatcher.FindNext()) {
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::White, TEXT("EEEEEY"));
		SetBaseURL(URL);
		URL += mSceneName;
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

	if (GEngine) {
		FString log1, log2, log3 = "";
		if (bSuccess) log1 = "I'm in, but bSuccess = true";
		if (Response.IsValid()) log2 = ", Response = true";
		if (Response->GetContentLength() > 0) log3 = ", ResponseContent = true";
		FString log = log1 + log2 + log3;
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("REQUEST: ") + log);
	}

	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	// GetActualURL(); 
	FString mURL = Request->GetURL();	

	if (bSuccess && Response.IsValid() && Response->GetContentLength() > 0) {

		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
		IFileManager* FileManager = &IFileManager::Get();

		//	Create save directory path
		FString savePath = FPaths::ProjectSavedDir();

		FString fileSavePath = savePath;
		fileSavePath += FGenericPlatformHttp::UrlDecode(FPaths::GetCleanFilename(mURL));



		if (!PlatformFile.DirectoryExists(*savePath) || !FileManager->DirectoryExists(*savePath)) {
			//	Create directory
			PlatformFile.CreateDirectoryTree(*savePath);
		}

		//	Create the file
		IFileHandle* fileHandler = PlatformFile.OpenWrite(*fileSavePath);
		if (fileHandler) {
			//	Write the new file from the response
			fileHandler->Write(Response->GetContent().GetData(), Response->GetContentLength());
			//	Close and finish rhe operation
			delete fileHandler;
		}

		DoLoadResources(fileSavePath, mResources);
	}

}

bool UBg2Downloader::DoLoadResources(const FString& Path, TArray<FString>& Result) {
	FString mURL = GetActualURL();

	//	Obtain the scene's objects to download.
	if (mURL.Contains(mSceneName)) {
		// End of main scene file download
		ScenePath = Path;
		UBg2DownloadParser::SceneParser(Path, Result);
		if (GEngine) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("Number of things to download " + FString::FromInt(Result.Num())));
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("PATH: " + Path));
		}
	}
	else {
		// End of resource file download
		mOnComplete();
		return true;
	}

	// Main scene download: check how many resource files 
	mNumResources = Result.Num();
	mDownloadedResources = 0;
	for (int32 i = 0; i < Result.Num(); ++i)
	{
		FString URL = GetBaseURL() + "/" + FGenericPlatformHttp::UrlEncode(*Result[i]);
		/*if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("LOOP: ") + URL);
		*/
		UBg2Downloader* DownloadTask = NewObject<UBg2Downloader>();
		DownloadTask->Start(URL, [&]() {
			mDownloadedResources++;
			if (mDownloadedResources == mNumResources) {
				mOnComplete();
			}
		});
	}
	return true;
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
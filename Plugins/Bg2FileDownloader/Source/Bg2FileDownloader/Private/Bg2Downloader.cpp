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


FString mDownloadPath = "Path";
FString mBg2Thing = "FirstThing";

bool bIsReady = false;

UBg2Downloader* UBg2Downloader::Download(FString URL) {
	UBg2Downloader* DownloadTask = NewObject<UBg2Downloader>();
	DownloadTask->Start(URL);

	return DownloadTask;
}

void UBg2Downloader::Start(FString URL) {
	FString defaultURL = "http://192.168.0.18:8080";
	if (URL.IsEmpty()){
		URL = defaultURL;
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

	if (bSuccess && Response.IsValid() && Response->GetContentLength() > 0) {
		//if (CheckAndroidReadiness()) {
			IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
			IFileManager* FileManager = &IFileManager::Get();

			//	Create save directory path
			FString savePath = FPaths::ProjectSavedDir();
			SetDownloadPath(savePath);

			FString filename = savePath;
			if (mBg2Thing.Equals("FirstThing")) {
				filename += FString("Test.txt");
			}
			else {
				filename += mBg2Thing;
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
		//}
	}
}

void UBg2Downloader::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved)
{
	//	TODO:	USARLO EN UNA BARRA DE CARGA ?
	int32 size = HttpRequest->GetContentLength();
}

/*void UBg2Downloader::AndroidReadiness() {
	//bool bIsReady = false;
	TArray<FString> mAndroidPermArr;
	mAndroidPermArr.Add(TEXT("android.permission.WRITE_EXTERNAL_STORAGE"));

	//bIsReady = UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.WRITE_EXTERNAL_STORAGE"));
	bIsReady = UAndroidPermissionFunctionLibrary::CheckPermission(mAndroidPermArr[0]);

	if (!bIsReady) {
		UAndroidPermissionCallbackProxy* mCallback = UAndroidPermissionFunctionLibrary::AcquirePermissions(mAndroidPermArr);
		
		mCallback->OnPermissionsGrantedDelegate.BindLambda([this](const TArray<FString>& Permissions, const TArray<bool>& GrantResults) {
			if (GrantResults.Num() > 0) {
				if (GrantResults[0]) {
					//bAndroidReady = true;
					bIsReady = true;
				}
				else {
					bIsReady = false;
				}
			}
			
		});
	}
	
	if (mBg2Thing.Equals("FirstThing")) {
		*UBg2Downloader::bAndroidReady = false;
	}

	if (!*UBg2Downloader::bAndroidReady) {
		TArray<FString> mAndroidPermArr;
		mAndroidPermArr.Add(TEXT("android.permission.WRITE_EXTERNAL_STORAGE"));

		*UBg2Downloader::bAndroidReady = UAndroidPermissionFunctionLibrary::CheckPermission(mAndroidPermArr[0]);
		if (!*UBg2Downloader::bAndroidReady) {
			UAndroidPermissionCallbackProxy* mCallback = UAndroidPermissionFunctionLibrary::AcquirePermissions(mAndroidPermArr);

			mCallback->OnPermissionsGrantedDelegate.BindLambda([this](const TArray<FString>& Permissions, const TArray<bool>& GrantResults) {
				if (GrantResults.Num() > 0) {
					if (GrantResults[0]) {
						*UBg2Downloader::bAndroidReady = true;
					}
					else {
						*UBg2Downloader::bAndroidReady = false;
					}
				}
				});
		}

	}

}*/

/*bool UBg2Downloader::CheckAndroidReadiness() {
	UBg2Downloader bg2dwn;
	bg2dwn.AndroidReadiness();					//	We are in a static function, we must access this way.
	return *bg2dwn.bAndroidReady;				//	""
	
}*/

bool UBg2Downloader::CheckAndroidReadiness()
{
	TArray<FString> mAndroidPermArr;
	mAndroidPermArr.Add(TEXT("android.permission.WRITE_EXTERNAL_STORAGE"));

	//bIsReady = UAndroidPermissionFunctionLibrary::CheckPermission(TEXT("android.permission.WRITE_EXTERNAL_STORAGE"));
	bIsReady = UAndroidPermissionFunctionLibrary::CheckPermission(mAndroidPermArr[0]);

	if (!bIsReady) {
		UAndroidPermissionCallbackProxy* mCallback = UAndroidPermissionFunctionLibrary::AcquirePermissions(mAndroidPermArr);

		mCallback->OnPermissionsGrantedDelegate.BindLambda([this](const TArray<FString>& Permissions, const TArray<bool>& GrantResults) {
			if (GrantResults.Num() > 0) {
				if (GrantResults[0]) {
					bIsReady = true;
				}
				else {
					bIsReady = false;
				}
			}

			});
	}
	return bIsReady;
}

void UBg2Downloader::SetDownloadPath(FString DownloadPath)
{
	mDownloadPath = DownloadPath;
}

FString UBg2Downloader::GetDownloadPath() {
	if (mDownloadPath.Equals("Path")) {
		return FPaths::ProjectSavedDir();	//	Default
	}
	return mDownloadPath;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Bg2Downloader.h"

#include "Modules/ModuleManager.h"
#include "Interfaces/IHttpResponse.h"
#include "HttpModule.h"
#include "HAL/PlatformFilemanager.h"
#include "HAL/FileManager.h"

FString mDownloadPath = "Path";
FString mBg2Thing = "FirstThing";

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
	//	TODO:	PROCESAR UN JSON TAMBIÉN -> JSONParser

	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	if (bSuccess && Response.IsValid() && Response->GetContentLength() > 0) {
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

	}
}

void UBg2Downloader::OnRequestProgress(FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesRecieved)
{
	//	TODO:	USARLO EN UNA BARRA DE CARGA ?
	int32 size = HttpRequest->GetContentLength();
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

FString UBg2Downloader::JSONParser(FString JSONFilePath)
{
	//	TODO:	COMENZAR A PARSEAR EL JSON, DEVOLVER NOMBRE DEL FICHERO A DESCARGAR
	FString mFilename = "";

	JSONFilePath;

	return mFilename;
}
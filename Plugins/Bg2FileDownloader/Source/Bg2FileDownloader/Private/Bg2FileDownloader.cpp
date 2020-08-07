// Copyright Epic Games, Inc. All Rights Reserved.

#include "Bg2FileDownloader.h"

#define LOCTEXT_NAMESPACE "FBg2FileDownloaderModule"

void FBg2FileDownloaderModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FBg2FileDownloaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FBg2FileDownloaderModule, Bg2FileDownloader)
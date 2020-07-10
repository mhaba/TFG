// Fill out your copyright notice in the Description page of Project Settings.


#include "Bg2SceneLoader.h"
#include "Bg2Scene.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine.h"

// Sets default values
ABg2SceneLoader::ABg2SceneLoader()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneRoot = nullptr;

	ConstructorHelpers::FObjectFinder<UMaterial> MaterialFinder(TEXT("/Bg2UnrealTools/Materials/TestMaterial"));
	if (MaterialFinder.Succeeded())
	{
		BaseMaterial = MaterialFinder.Object;
	}
	else
	{
		BaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("InvalidBaseMaterial"));
	}
}

// Called when the game starts or when spawned
void ABg2SceneLoader::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the scene node at the origin
	SetActorLocation(FVector(0, 0, 0));
}

// Called every frame
void ABg2SceneLoader::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABg2SceneLoader::LoadScene(FString Path, float Scale)
{
	if (BaseMaterial)
	{
		UBg2Scene::Load(this, BaseMaterial, Path, Scale);
	}
	else
	{
		UE_LOG(Bg2Tools, Error, TEXT("Could not load scene. The base material is not configured."));
	}
}

FString ABg2SceneLoader::LoadSceneFromFilesystem(float Scale)
{
	//FString testPath = "F:/Unreal Projects/Bg2UnrealToolsExample/sample_scene/sample_scene.vitscnj";
	//LoadScene(testPath, Scale);
	if (GEngine)
	{
		if (GEngine->GameViewport)
		{
			FString DialogTitle = "Open file";
			FString DefaultPath = FPaths::ProjectContentDir();
			FString FileTypes = "bg2 scene files|*.vitscnj;";
			TArray<FString> OutFileNames;
			void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform)
			{
				uint32 SelectionFlag = 0;
				DesktopPlatform->OpenFileDialog(ParentWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutFileNames);
				if (OutFileNames.Num() > 0)
				{
					LoadScene(OutFileNames[0], Scale);
					return OutFileNames[0];
				}
			}
		}
	}
	return "";
}

void ABg2SceneLoader::GetExternalResources(FString Path, TArray<FString>& Result)
{
	UBg2Scene::GetExternalResources(Path, Result);
}

void ABg2SceneLoader::CloseScene()
{
	if (SceneRoot)
	{

	}
}

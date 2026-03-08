#include "BuildTool.h"

#include "DrawDebugHelpers.h"
#include "Engine/StaticMeshActor.h"

#include "Editor.h"
#include "Subsystems/EditorActorSubsystem.h"

FBuildTool::FBuildTool()
{
	
}

FBuildTool::~FBuildTool()
{
}

void FBuildTool::OnClick(const struct FBuildClickedContext& context)
{
	switch (context.BuildMode)
	{
		case EBuildEditMode::Add:
			if (context.Key == EKeys::LeftMouseButton and context.bHit)
			{
				CreateMeshAtLocation(context.World, context.HitResult, context.BuildAsset.Get(), context.AssetType,context.IgnoreActors,context.IgnoreComponents);
			}
			break;
		case EBuildEditMode::Remove:
			if (context.Key == EKeys::LeftMouseButton and context.bHit)
			{
				DeleteMeshAtLocation(context.HitResult.GetActor());
			}
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Current Mode is not exist is this BuildTool"));
			break;
	}
}

void FBuildTool::CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FHitResult& HitResult, UObject* BuildAsset, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent)
{
	if (BuildAsset == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation BuildAsset is nullptr"));
		return;
	}

	if (type == EBuildAssetType::Actor)
	{
		AActor* Actor = Cast<AActor>(BuildAsset);
		const UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
		if (!PrimComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation Actor has no valid root component"));
			return;
		}

		FPlacementResult Result = CanPlaceActorAtLocation(ViewPortClientWorld, HitResult, PrimComp->Bounds, type, IgnoreActors, IgnoreStaticMeshComponent);

		if (!Result.bCanPlace)
		{
			UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation Cannot place actor: %s"), *Result.FailMessage);
			return;
		}

		const FScopedTransaction Transaction(
			NSLOCTEXT("BuildTool", "CreateActor", "Create Actor")
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		AActor* MeshActor =
			ViewPortClientWorld->SpawnActor<AActor>(
				Actor->GetClass(),
				Result.FinalTransform.GetLocation(),
				FRotator::ZeroRotator,
				SpawnParams
			);

		if (!MeshActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn AActor"));
			return;
		}

		MeshActor->SetFlags(RF_Transactional);
		MeshActor->Modify();
	}
	else if (type == EBuildAssetType::StaticMesh)
	{
		UStaticMesh* ProduceStaticMesh = Cast<UStaticMesh>(BuildAsset);
		FPlacementResult Result = CanPlaceActorAtLocation(ViewPortClientWorld, HitResult, ProduceStaticMesh->GetBounds(), type, IgnoreActors, IgnoreStaticMeshComponent);

		if (!Result.bCanPlace)
		{
			UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation Cannot place actor: %s"), *Result.FailMessage);
			return;
		}

		const FScopedTransaction Transaction(
			NSLOCTEXT("BuildTool", "CreateActor", "Create Actor")
		);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

		AStaticMeshActor* MeshActor =
			ViewPortClientWorld->SpawnActor<AStaticMeshActor>(
				Result.FinalTransform.GetLocation(),
				FRotator::ZeroRotator,
				SpawnParams
			);

		if (!MeshActor)
		{
			UE_LOG(LogTemp, Warning,TEXT("Failed to spawn AStaticMeshActor"));
			return;
		}

		MeshActor->SetFlags(RF_Transactional);
		MeshActor->Modify();

		UStaticMeshComponent* MeshComp = MeshActor->GetStaticMeshComponent();
		if (MeshComp)
		{
			MeshComp->SetStaticMesh(ProduceStaticMesh);
			MeshComp->SetMobility(EComponentMobility::Movable);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::CreateMeshAtLocation Unknown EBuildAssetType"));
	}
}

void FBuildTool::DeleteMeshAtLocation(AActor* DeleActor)
{
	if (!DeleActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::DeleteMeshAtLocation DeleActor is nullptr"));
		return;
	}

	UEditorActorSubsystem* Subsystem = GEditor->GetEditorSubsystem<UEditorActorSubsystem>();
	
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("FBuildTool::DeleteMeshAtLocation UEditorActorSubsystem is nullptr"));
		return;
	}

	const FScopedTransaction Transaction(
		NSLOCTEXT("BuildTool", "DeleteActor", "Delete Actor")
	);

	DeleActor->Modify();
	Subsystem->DestroyActor(DeleActor);
}

FPlacementResult FBuildTool::CanPlaceActorAtLocation(UWorld* ViewPortClientWorld, const FHitResult& HitResult, FBoxSphereBounds BuildBound, EBuildAssetType type, TArray<AActor*> IgnoreActors, TArray<UStaticMeshComponent*> IgnoreStaticMeshComponent)
{
	FPlacementResult retResult;

	const FVector HalfExtent = BuildBound.BoxExtent;

	const FVector SurfaceNormal = HitResult.ImpactNormal.GetSafeNormal();

	constexpr float PlacementEpsilon = 0.1f;

	FVector HitHalfExtent = FVector::ZeroVector;

	if (const UPrimitiveComponent* HitComp = HitResult.GetComponent())
	{
		HitHalfExtent = HitComp->Bounds.BoxExtent;
	}

	const float NewMeshExtentAlongNormal =
		HalfExtent.ProjectOnToNormal(SurfaceNormal).Size();

	const float HitMeshExtentAlongNormal =
		HitHalfExtent.ProjectOnToNormal(SurfaceNormal).Size();

	const FVector CandidateLocation =
		HitResult.ImpactPoint +
		SurfaceNormal * (NewMeshExtentAlongNormal + PlacementEpsilon);

	// 设置最终位置和旋转
	retResult.FinalTransform = FTransform(FRotator::ZeroRotator, CandidateLocation);

	FCollisionShape CollisionShape =
		FCollisionShape::MakeBox(HalfExtent);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	if (HitResult.GetComponent())
	{
		QueryParams.AddIgnoredComponent(HitResult.GetComponent());
		QueryParams.AddIgnoredActor(HitResult.GetActor());
	}
	QueryParams.AddIgnoredActors(IgnoreActors);
	for (UStaticMeshComponent* comp : IgnoreStaticMeshComponent)
	{
		if (comp)
		{
			QueryParams.AddIgnoredComponent(comp);
		}
	}

	const bool bBlocked =
		ViewPortClientWorld->OverlapBlockingTestByChannel(
			CandidateLocation,
			FQuat::Identity,
			ECC_WorldStatic,
			CollisionShape,
			QueryParams
		);

	if (bBlocked)
	{
		retResult.bCanPlace = false;
		retResult.FailReason = EPlacementFailReason::BlockedByCollision;
		retResult.FailMessage = TEXT("Placement blocked");
		return retResult;
	}

	retResult.bCanPlace = true;
	retResult.FailReason = EPlacementFailReason::None;
	retResult.FailMessage = TEXT("");

	return retResult;
}
#pragma once

#include "CoreMinimal.h"

struct FBuildPreviewInstance
{
	TWeakObjectPtr<AActor> Actor;
	TWeakObjectPtr<UStaticMeshComponent> MeshComponent;

	bool IsValid() const
	{
		return Actor.IsValid() && MeshComponent.IsValid();
	}

	void Destroy()
	{
		if (Actor.IsValid())
		{
			Actor->Destroy();
		}
		Actor.Reset();
		MeshComponent.Reset();
	}
};

class FBuildPreviewSystem
{
public:
	void StartPreview(UWorld* ViewPortClientWorld, UStaticMesh* InMesh);
    void UpdatePreviewTransform(const FTransform& InTransform);
    void StopPreview();

    bool HasValidPreview() const
    {
        return Preview.IsValid();
    }

	AActor* GetPreviewActor() const
	{
		return Preview.Actor.Get();
	}

	UStaticMeshComponent* GetPreviewMeshComponent() const
	{
		return Preview.MeshComponent.Get();
	}

private:
    FBuildPreviewInstance Preview;
};
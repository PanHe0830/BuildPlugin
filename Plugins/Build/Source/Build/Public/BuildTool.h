#pragma once

#include "CoreMinimal.h"

class FBuildTool
{
public:
	FBuildTool();
	~FBuildTool();

	void OnClick(UWorld* ViewPortClientWorld, const FVector& ClickLocation);

	// 设置生成物体的网格资源
	void SetProduceStaticMesh(class UStaticMesh* InStaticMesh) { ProduceStaticMesh = InStaticMesh; }
	void SetProduceActorMesh(TSubclassOf<class AActor> InActorClass) { ProduceActorMesh = InActorClass; }

private:
	// 功能实现
	// 在指定位置创建网格体
	void CreateMeshAtLocation(UWorld* ViewPortClientWorld, const FVector& Location);

private:
	// 生成物体的网格资源
	UPROPERTY(Transient) //Transient 作用是不被序列化
	class UStaticMesh* ProduceStaticMesh;

	// 生成物体的Actor类型(这个主要用于蓝图)
	UPROPERTY(Transient) //Transient 作用是不被序列化
	class TSubclassOf<class AActor> ProduceActorMesh;
};
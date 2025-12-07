#include "BuildToolkitWidget.h"

// Slate控件头文件
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"
#include "PropertyCustomizationHelpers.h"
// Slate控件头文件

void SBuildToolkitWidget::Construct(const FArguments& InArgs)
{
    BuildTool = InArgs._BuildTool;

    ChildSlot
    [
        SNew(SVerticalBox)
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(STextBlock).Text(FText::FromString("Build Mode Settings"))
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SNumericEntryBox<float>)
                    .LabelVAlign(VAlign_Center)
                    .AllowSpin(true)
                    .Value_Lambda([]() { return 100.0f; })
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SButton)
                    .Text(FText::FromString("Test Button"))
                    .OnClicked_Lambda([]() { UE_LOG(LogTemp, Warning, TEXT("Button Clicked")); return FReply::Handled(); })
            ]
            + SVerticalBox::Slot().AutoHeight().Padding(5)
            [
                SNew(SObjectPropertyEntryBox)
                    .AllowedClass(UObject::StaticClass()) // 可以改成你想允许的类型
                    .AllowClear(true)
                    .OnObjectChanged(this, &SBuildToolkitWidget::OnResourceSelected)
            ]
    ];
}

void SBuildToolkitWidget::OnResourceSelected(const FAssetData& AssetData)
{
    if (!BuildTool.IsValid())
        return;

    UObject* SelectedObject = AssetData.GetAsset();

    if (UStaticMesh* Mesh = Cast<UStaticMesh>(SelectedObject))
    {
        //BuildTool->ProduceStaticMesh = Mesh;
        //BuildTool->ProduceActorMesh = nullptr;
        UE_LOG(LogTemp, Warning, TEXT("Selected StaticMesh: %s"), *Mesh->GetName());
    }
    else if (UBlueprint* Blueprint = Cast<UBlueprint>(SelectedObject))
    {
        if (Blueprint->GeneratedClass->IsChildOf(AActor::StaticClass()))
        {
            //BuildTool->ProduceActorMesh = Blueprint->GeneratedClass;
            //BuildTool->ProduceStaticMesh = nullptr;
            UE_LOG(LogTemp, Warning, TEXT("Selected Blueprint Actor: %s"), *Blueprint->GetName());
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Selected Blueprint is not Actor"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected object is not valid"));
    }
}

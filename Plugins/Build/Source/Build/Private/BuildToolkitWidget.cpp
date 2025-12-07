#include "BuildToolkitWidget.h"

#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/SBoxPanel.h"

void SBuildToolkitWidget::Construct(const FArguments& InArgs)
{
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
    ];
}

#include "BuildModeUIWidget.h"

#include "Widgets/Input/SButton.h"

void SBuildModeUIWidget::Construct(const FArguments& InArgs)
{
    BuildModeChange = InArgs._BuildModeChange;
    ChildSlot
        [
            SNew(SVerticalBox)
                + SVerticalBox::Slot().AutoHeight().Padding(5)
                [
                    SNew(SHorizontalBox)
                        + SHorizontalBox::Slot().AutoWidth().Padding(2.5)
                        [
                            SNew(SButton).Text(FText::FromString("Add Mode"))
                                .OnClicked(this, &SBuildModeUIWidget::OnAddModeClicked)
                        ]
                        + SHorizontalBox::Slot().AutoWidth().Padding(2.5)
                        [
                            SNew(SButton).Text(FText::FromString("Delete Mode"))
                                .OnClicked(this, &SBuildModeUIWidget::OnRemoveModeClicked)
                        ]
                ]
        ];
}

FReply SBuildModeUIWidget::OnAddModeClicked()
{
    BuildModeChange.ExecuteIfBound(EBuildEditMode::Add);
    return FReply::Handled();
}

FReply SBuildModeUIWidget::OnRemoveModeClicked()
{
    BuildModeChange.ExecuteIfBound(EBuildEditMode::Remove);
    return FReply::Handled();
}

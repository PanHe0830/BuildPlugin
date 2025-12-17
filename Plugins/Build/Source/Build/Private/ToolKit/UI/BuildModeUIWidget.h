#pragma once

#include "CoreMinimal.h"
#include "Types/BuildModeTypes.h"
#include "Widgets/SCompoundWidget.h"

class SBuildModeUIWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SBuildModeUIWidget) {}
        SLATE_EVENT(FOnBuildModeChanged, BuildModeChange)
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

private:
    FReply OnAddModeClicked();
    FReply OnRemoveModeClicked();
private:
	FOnBuildModeChanged BuildModeChange;
};
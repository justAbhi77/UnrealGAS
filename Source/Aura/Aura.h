

#pragma once

#include "CoreMinimal.h"

/**
 Enum for visualling highlighting actors on mouse hover.
 */
UENUM(BlueprintType)
enum class CustomDepthHighlight : uint8
{
    Empty = 0 UMETA(DisplayName = "Empty"),
    Red = 250 UMETA(DisplayName = "Red Highlight"),
    Blue = 251 UMETA(DisplayName = "Blue Highlight"),
    Tan = 252 UMETA(DisplayName = "Tan Highlight")
};

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3


#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class CustomDepthHighlight : uint8
{
    Empty = 0 UMETA(DisplayName = "Empty"),
    Red = 250 UMETA(DisplayName = "Red Highlight"),
    Blue = 251 UMETA(DisplayName = "Blue Highlight"),
    Tan = 252 UMETA(DisplayName = "Tan Highlight")
};

#define CUSTOM_DEPTH_RED 250
#define CUSTOM_DEPTH_BLUE 251
#define CUSTOM_DEPTH_TAN 252

#define ECC_Projectile ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Target ECollisionChannel::ECC_GameTraceChannel2
#define ECC_ExcludePlayers ECollisionChannel::ECC_GameTraceChannel3


#pragma once

#include "CoreMinimal.h"

/**
 Enum for highlighting actors on mouse hover.
 */
UENUM(BlueprintType)
enum class ECustomDepthHighlight : uint8
{
	Empty = 0 UMETA(DisplayName = "Empty"),
	Red = 250 UMETA(DisplayName = "Red Highlight"),
	Blue = 251 UMETA(DisplayName = "Blue Highlight"),
	Tan = 252 UMETA(DisplayName = "Tan Highlight")
};

#define ECC_PROJECTILE ECollisionChannel::ECC_GameTraceChannel1
#define ECC_TARGET ECollisionChannel::ECC_GameTraceChannel2
#define ECC_EXCLUDE_PLAYERS ECollisionChannel::ECC_GameTraceChannel3

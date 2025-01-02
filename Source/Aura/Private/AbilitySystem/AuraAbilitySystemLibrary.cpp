// 


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	// Validate input
	if(!WorldContextObject) return nullptr;

	// Retrieve the player controller
	if(APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD())) // Retrieve the Aura HUD
		{
			// Retrieve the player state and associated components
			if(AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributeSet();

				// Create widget controller parameters
				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

				// Retrieve and return the Overlay Widget Controller
				return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
			}
		}

	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	// Validate input
	if(!WorldContextObject) return nullptr;

	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			if(AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
			{
				UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
				UAttributeSet* AS = PS->GetAttributeSet();

				const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);

				return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
			}
		}

	return nullptr;
}

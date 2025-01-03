// 

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "AuraInputConfig.h"
#include "AuraInputComponent.generated.h"

/**
 * Custom input component class for handling ability input bindings.
 */
UCLASS()
class AURA_API UAuraInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	/**
	 * Binds input actions for abilities defined in the input configuration.
	 * 
	 * @tparam UserClass         The class type of the object receiving the bindings.
	 * @tparam PressedFuncType   The function type for the action pressed event.
	 * @tparam ReleasedFuncType  The function type for the action released event.
	 * @tparam HeldFuncType      The function type for the action held event.
	 * @param InputConfig        Pointer to the input configuration containing actions and tags.
	 * @param Object             Pointer to the object where the events are bound.
	 * @param PressedFunc        Function called when an input action is pressed.
	 * @param ReleasedFunc       Function called when an input action is released.
	 * @param HeldFunc           Function called when an input action is held.
	 */
	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
	void BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object, 
		PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc);
};

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType, typename HeldFuncType>
void UAuraInputComponent::BindAbilityActions(const UAuraInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, HeldFuncType HeldFunc)
{
	// Ensure the input configuration is valid
	check(InputConfig);

	// Iterate over ability input actions in the configuration
	for(const auto& Action : InputConfig->AbilityInputActions)
	{
		// Validate that the input action and tag are correctly set
		if(Action.InputAction && Action.InputTag.IsValid())
		{
			// Bind the Pressed event if the function is provided
			if(PressedFunc)
				BindAction(Action.InputAction, ETriggerEvent::Started, Object, PressedFunc, Action.InputTag);

			// Bind the Released event if the function is provided
			if(ReleasedFunc)
				BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag);

			// Bind the Held event if the function is provided
			if(HeldFunc)
				BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, HeldFunc, Action.InputTag);
		}
	}
}

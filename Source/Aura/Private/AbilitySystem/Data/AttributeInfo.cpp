// 


#include "AbilitySystem/Data/AttributeInfo.h"

/**
 * Searches the attributes list for an entry matching the specified tag.
 * Logs if not found and bLogNotFound is true.
 */
FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for(const auto& Info : AttributeInformation)
		if(Info.AttributeTag.MatchesTagExact(AttributeTag))
			return Info;

	if(bLogNotFound)
		UE_LOG(LogTemp, Error, TEXT("Can't find Info for AttributeTag [%s] on AttributeInfo [%s]."),
			*AttributeTag.ToString(), *GetNameSafe(this));

	return FAuraAttributeInfo();
}

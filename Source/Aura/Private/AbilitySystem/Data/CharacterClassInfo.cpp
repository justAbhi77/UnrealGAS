// 


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	// CharacterClassInformation.FindChecked(CharacterClass);
	return *CharacterClassInformation.Find(CharacterClass);
}

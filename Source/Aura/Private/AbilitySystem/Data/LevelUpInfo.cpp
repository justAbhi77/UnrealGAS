//


#include "AbilitySystem/Data/LevelUpInfo.h"

int32 ULevelUpInfo::FindLevelForXP(int32 XP) const
{
	int32 Level = 1;
	bool bSearching = true;

	// LevelUpInformation[1] = Level 1 Information
	// LevelUpInformation[2] = Level 2 Information

	while(bSearching)
	{
		// Prevent out-of-bounds access
		if(LevelUpInformation.Num() - 1 <= Level) return Level;

		// Check if XP meets requirement for next level
		if(XP >= LevelUpInformation[Level].LevelUpRequirement)
			Level++;
		else
			bSearching = false;
	}

	return Level;
}

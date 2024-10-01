// Copyright AM Company


#include "AbilitySystem/Data/LevelUpInfo.h"

FAuraLevelUpInfo ULevelUpInfo::FindLevelUpInformationForXP(const float XP) const
{
	for (const FAuraLevelUpInfo& LevelInfo : LevelUpInformation)
	{
		if (XP <= LevelInfo.LevelUpRequirement)
		{
			return LevelInfo;
		}
	}

	return FAuraLevelUpInfo();
}

int32 ULevelUpInfo::FindLevelForXP(int32 XP)
{
	int32 Level = 1;
	bool bSearching = true;
	while (bSearching)
	{
		// LevelUpInformation[1] = Level 1 Information
		// LevelUpInformation[2] = Level 1 Information
		if (LevelUpInformation.Num() - 1 <= Level) return Level;

		if (XP >= LevelUpInformation[Level].LevelUpRequirement)
		{
			++Level;
		}
		else
		{
			bSearching = false;
		}
	}
	return Level;
}

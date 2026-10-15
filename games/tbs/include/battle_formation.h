#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "types.h"

struct BattleFormationRecord
{
	u8 battleType;
	u8 memberIds[5];
	u8 minimumCounts[5];
	u8 maximumCounts[5];
};

extern struct BattleFormationRecord BattleFormation_Records[];

#endif

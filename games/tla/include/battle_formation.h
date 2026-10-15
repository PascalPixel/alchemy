#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "types.h"

struct BattleFormationRecord
{
    u16 memberIds[5];
    u8  minimumCounts[5];
    u8  maximumCounts[5];
    u8  unk14[4];
};

extern struct BattleFormationRecord gBattleFormation_Records[];

#endif

#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "types.h"

struct BattleFormationRecord {
    u16 member_ids[5];
    u8 minimum_counts[5];
    u8 maximum_counts[5];
    u8 unknown_14[4];
};

extern struct BattleFormationRecord BattleFormation_Records[];

#endif

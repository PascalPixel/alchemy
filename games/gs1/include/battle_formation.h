#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "types.h"

struct BattleFormationRecord {
    u8 battle_type;
    u8 member_ids[5];
    u8 minimum_counts[5];
    u8 maximum_counts[5];
};

extern struct BattleFormationRecord Data_080c5c38[];

#endif

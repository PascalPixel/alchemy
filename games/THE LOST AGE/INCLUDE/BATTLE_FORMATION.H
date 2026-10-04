#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "TYPES.H"

/* An enemy formation: up to five member kinds with the least and most of
   each. The Lost Age's ids are halfwords. */
struct BattleFormationRecord {
    u16 member_ids[5];
    u8 minimum_counts[5];
    u8 maximum_counts[5];
    u8 unknown_14[4];
};

extern struct BattleFormationRecord BattleFormation_Records[];

#endif

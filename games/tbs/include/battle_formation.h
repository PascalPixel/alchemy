#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

/// @file
/// The enemy formation records.

#include "types.h"

/// An enemy formation: its battle type and up to five kinds of enemy, each
/// with the fewest and most of it that appear.
struct BattleFormationRecord {
  u8 battleType;
  u8 memberIds[5];
  u8 minimumCounts[5];
  u8 maximumCounts[5];
};

extern struct BattleFormationRecord gBattleFormation_Records[];

#endif

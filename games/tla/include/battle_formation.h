#ifndef ALCHEMY_BATTLE_FORMATION_H
#define ALCHEMY_BATTLE_FORMATION_H

#include "types.h"

/// An enemy formation: up to five kinds of enemy, each with the fewest and
/// most of it that appear. A formation's number is a battle's, and a Djinni
/// battle's is DJINNI_BATTLE_EVENT_FIRST plus the Djinni's.
struct BattleFormationRecord {
  u16 memberIds[5];
  u8 minimumCounts[5];
  u8 maximumCounts[5];
  u8 unk14[4];
};

extern struct BattleFormationRecord gBattleFormation_Records[];

#endif

/// @file
/// Picking a unit on one side: a random living target, or the living unit with
/// the highest Attack.
// Part of battle.c.

#include "../battle_internal.h"

/// Picks a random living target on one side: the party (target codes 0x100 +
/// slot) when party is set, otherwise the enemies (0x180 + slot); 0 when none
/// lives.
s32 BattleAI_ChooseRandomTarget(s32 party) {
  struct BattleWork *work;
  u16 targets[6];
  s32 count;
  s32 i;

  count = 0;
  work = Ram_HeapSlots->battleWork;

  if (party != 0) {
    for (i = 0; work->partyUnits[i] != BATTLE_UNIT_LIST_END; i++) {
      if (work->partyUnits[i] != BATTLE_UNIT_REMOVED &&
          ((struct Unit *)Unit_Get(work->partyUnits[i]))->hp != 0)
        targets[count++] = i | 0x100;
    }
  } else {
    for (i = 0; work->enemyIds[i] != BATTLE_UNIT_LIST_END; i++) {
      if (work->enemyIds[i] != BATTLE_UNIT_REMOVED &&
          ((struct Unit *)Unit_Get(work->enemyIds[i]))->hp != 0)
        targets[count++] = i | 0x180;
    }
  }
  if (count == 0)
    return 0;
  return targets[(u32)(count * Battle_RandomFar()) >> 16];
}

/// Finds the living unit with the highest Attack on unitId's side (the party
/// for ids 0 to 7, else the enemies), other than unitId itself; unitId when no
/// other lives.
s32 BattleUnit_FindStrongestAlly(u32 unitId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u32 best = unitId;
  s32 bestAttack = ((struct Unit *)Unit_Get(unitId))->atk;
  s32 i;
  s32 id;

  if (unitId <= ALLY_MAX) {
    for (i = 0; (id = work->partyUnits[i]) != BATTLE_UNIT_LIST_END; i++) {
      if (id != BATTLE_UNIT_REMOVED && ((struct Unit *)Unit_Get(id))->hp != 0) {
        if (best == unitId ||
            (id != unitId && ((struct Unit *)Unit_Get(id))->atk > bestAttack)) {
          best = id;
          bestAttack = ((struct Unit *)Unit_Get(best))->atk;
        }
      }
    }
  } else {
    for (i = 0; (id = work->enemyIds[i]) != BATTLE_UNIT_LIST_END; i++) {
      if (id != BATTLE_UNIT_REMOVED && ((struct Unit *)Unit_Get(id))->hp != 0) {
        if (best == unitId ||
            (id != unitId && ((struct Unit *)Unit_Get(id))->atk > bestAttack)) {
          best = id;
          bestAttack = ((struct Unit *)Unit_Get(best))->atk;
        }
      }
    }
  }
  return best;
}

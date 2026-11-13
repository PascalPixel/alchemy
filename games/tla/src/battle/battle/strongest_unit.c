/// @file
/// Picking a unit on one side: a random living target, or the living unit with
/// the highest Attack.
// Part of battle.c.

#include "../battle_internal.h"

/* VIEW(strongest_unit): routine Unit_Get as struct BattleUnit *Unit_Get(s32
 * unitId); owner_state.h has void *Unit_Get(s32 owner); tried as the
 * definition's void *Unit_Get(s32 owner), which the object already declares, it
 * does not compile in tla-ja: games/tla/src/battle/battle.c:6206: request for
 * member `hp' in something not a structure or union. */
extern struct BattleUnit *
Unit_Get__strongest_unit(s32 unitId) __asm__("Unit_Get");
/* VIEW(strongest_unit): routine Battle_RandomFar as u32 Battle_RandomFar(void);
 * battle_system.h has s32 Battle_RandomFar(void); tried as the 2 declarations'
 * u32 Battle_RandomFar(void), it does not compile in tla-ja:
 * games/tla/src/battle/battle.c:6190: conflicting types for `Battle_RandomFar'.
 */
extern u32 Battle_RandomFar__strongest_unit(void) __asm__("Battle_RandomFar");

/// Picks a random living target on one side: the party (target codes 0x100 +
/// slot) when party is set, otherwise the enemies (0x180 + slot); 0 when none
/// lives.
// No ☀️ twin found.
s32 BattleAI_ChooseRandomTarget(s32 party) {
  struct BattleWork *work;
  u16 targets[6];
  s32 count;
  s32 i;

  count = 0;
  work = Ram_HeapSlots->battleWork;

  if (party != 0) {
    for (i = 0; work->partyUnits[i] != 255; i++) {
      if (work->partyUnits[i] != 254 &&
          Unit_Get__strongest_unit(work->partyUnits[i])->hp != 0)
        targets[count++] = i | 0x100;
    }
  } else {
    for (i = 0; work->enemyIds[i] != 255; i++) {
      if (work->enemyIds[i] != 254 &&
          Unit_Get__strongest_unit(work->enemyIds[i])->hp != 0)
        targets[count++] = i | 0x180;
    }
  }
  if (count == 0)
    return 0;
  return targets[(count * Battle_RandomFar__strongest_unit()) >> 16];
}

/// Finds the living unit with the highest Attack on unitId's side (the party
/// for ids 0 to 7, else the enemies), other than unitId itself; unitId when no
/// other lives.
// No ☀️ twin found.
s32 BattleUnit_FindStrongestAlly(u32 unitId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u32 best = unitId;
  s32 bestAttack = Unit_Get__strongest_unit(unitId)->atk;
  s32 i;
  s32 id;

  if (unitId <= 7) {
    for (i = 0; (id = work->partyUnits[i]) != 255; i++) {
      if (id != 254 && Unit_Get__strongest_unit(id)->hp != 0) {
        if (best == unitId ||
            (id != unitId && Unit_Get__strongest_unit(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get__strongest_unit(best)->atk;
        }
      }
    }
  } else {
    for (i = 0; (id = work->enemyIds[i]) != 255; i++) {
      if (id != 254 && Unit_Get__strongest_unit(id)->hp != 0) {
        if (best == unitId ||
            (id != unitId && Unit_Get__strongest_unit(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get__strongest_unit(best)->atk;
        }
      }
    }
  }
  return best;
}

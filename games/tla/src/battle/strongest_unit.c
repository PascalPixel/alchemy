#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "ram_buffer.h"

struct BattleUnit *Unit_Get(s32 unitId);
u32 BattleRandomFar(void);

/// Picks a random living target on one side: the party (target codes 0x100 +
/// slot) when party is set, otherwise the enemies (0x180 + slot); 0 when none
/// lives.
// No ☀️ twin found.
s32 Func_0811f4d4(s32 party) {
  struct BattleWork *work;
  u16 targets[6];
  s32 count;
  s32 i;

  count = 0;
  work = Ram_HeapSlots->battleWork;

  if (party != 0) {
    for (i = 0; work->partyUnits[i] != 255; i++) {
      if (work->partyUnits[i] != 254 && Unit_Get(work->partyUnits[i])->hp != 0)
        targets[count++] = i | 0x100;
    }
  } else {
    for (i = 0; work->enemyIds[i] != 255; i++) {
      if (work->enemyIds[i] != 254 && Unit_Get(work->enemyIds[i])->hp != 0)
        targets[count++] = i | 0x180;
    }
  }
  if (count == 0)
    return 0;
  return targets[(count * BattleRandomFar()) >> 16];
}

/// Finds the living unit with the highest Attack on unitId's side (the party
/// for ids 0 to 7, else the enemies), other than unitId itself; unitId when no
/// other lives.
// No ☀️ twin found.
s32 Func_0811f5a4(u32 unitId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u32 best = unitId;
  s32 bestAttack = Unit_Get(unitId)->atk;
  s32 i;
  s32 id;

  if (unitId <= 7) {
    for (i = 0; (id = work->partyUnits[i]) != 255; i++) {
      if (id != 254 && Unit_Get(id)->hp != 0) {
        if (best == unitId || (id != unitId && Unit_Get(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get(best)->atk;
        }
      }
    }
  } else {
    for (i = 0; (id = work->enemyIds[i]) != 255; i++) {
      if (id != 254 && Unit_Get(id)->hp != 0) {
        if (best == unitId || (id != unitId && Unit_Get(id)->atk > bestAttack)) {
          best = id;
          bestAttack = Unit_Get(best)->atk;
        }
      }
    }
  }
  return best;
}

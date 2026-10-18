#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "ram_buffer.h"

struct BattleUnit *UnitGet(s32 unitId);

// Finds the living unit with the highest Attack on unitId's side (the party
// for ids 0 to 7, else the enemies), other than unitId itself; unitId when no
// other lives. No ☀️ twin found.
s32 Func_0811f5a4(u32 unitId) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u32 best = unitId;
  s32 bestAttack = UnitGet(unitId)->atk;
  s32 i;
  s32 id;

  if (unitId <= 7) {
    for (i = 0; (id = work->partyUnits[i]) != 255; i++) {
      if (id != 254 && UnitGet(id)->hp != 0) {
        if (best == unitId || (id != unitId && UnitGet(id)->atk > bestAttack)) {
          best = id;
          bestAttack = UnitGet(best)->atk;
        }
      }
    }
  } else {
    for (i = 0; (id = work->enemyIds[i]) != 255; i++) {
      if (id != 254 && UnitGet(id)->hp != 0) {
        if (best == unitId || (id != unitId && UnitGet(id)->atk > bestAttack)) {
          best = id;
          bestAttack = UnitGet(best)->atk;
        }
      }
    }
  }
  return best;
}

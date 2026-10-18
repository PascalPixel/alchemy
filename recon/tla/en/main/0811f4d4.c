// Draft, differ 4 of 95: picks a random living target on one side: the party
// (target codes 0x100 + slot) when party is set, otherwise the enemies
// (0x180 + slot); 0 when none lives. No ☀️ twin found. Left: instruction
// order only, in the prologue: the heap address's lsls and the battle work's
// load each sit one slot before the count's zero (movs r1 / mov r8) here,
// after it natively.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_work.h"
#include "ram_buffer.h"

struct BattleUnit *UnitGet(s32 unitId);
u32 BattleRandomFar(void);


s32 Func_0811f4d4(s32 party) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 targets[6];
  s32 count = 0;
  s32 i;

  if (party != 0) {
    for (i = 0; work->partyUnits[i] != 255; i++) {
      if (work->partyUnits[i] != 254 && UnitGet(work->partyUnits[i])->hp != 0)
        targets[count++] = i | 0x100;
    }
  } else {
    for (i = 0; work->enemyIds[i] != 255; i++) {
      if (work->enemyIds[i] != 254 && UnitGet(work->enemyIds[i])->hp != 0)
        targets[count++] = i | 0x180;
    }
  }
  if (count == 0)
    return 0;
  return targets[(count * BattleRandomFar()) >> 16];
}

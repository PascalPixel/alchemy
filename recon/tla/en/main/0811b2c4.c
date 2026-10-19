// Near miss: differ 2 of 89 (one reordered row). The native reads the first
// enemy slot before it builds the -1 (ldrsh r3, [r2, r3]; movs r1, #1; negs r1,
// r1); the draft builds it first. This game skips leading empty slots (254) and
// lays out up to the last live one. Swapping the two initialisers does not move
// the load.
#include "types.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_session.h"

void Summon_LayoutPositions(u16 *unit_ids, s32 count, s32 *x, s32 *z);
struct BattleObj *BattleObjGet(s32 id);

void EnemyUpdate(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 unitIds[14];
  s32 x[6];
  s32 z[6];
  s32 last = -1;
  s32 count = 0;
  s32 i;
  s32 total;

  for (i = 0; i < 6 && work->enemyIds[i] != 0xff; i++) {
    if (work->enemyIds[i] == 0xfe) {
      if (count == 0)
        continue;
    } else {
      last = count;
    }
    unitIds[count] = work->enemyIds[i];
    count++;
  }
  total = last + 1;
  Summon_LayoutPositions(unitIds, total, x, z);
  for (i = 0; i < total; i++) {
    s32 id = unitIds[i];

    if (id != 0xfe) {
      struct BattleObj *object = BattleObjGet(id);

      object->anchorX = x[i] << 16;
      object->anchorZ = z[i] << 16;
    }
  }
}

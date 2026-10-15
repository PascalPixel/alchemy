/* Near miss: differ 2 of 89 (one reordered row). The native reads the first enemy slot before it
 * builds the -1 (ldrsh r3, [r2, r3]; movs r1, #1; negs r1, r1); the draft builds it first. This game skips
 * leading empty slots (254) and lays out up to the last live one. Swapping the two initialisers does not move the load. */
#include "types.h"
#include "ram_buffer.h"
#include "battle_work.h"
#include "battle_session.h"

void Func_0811b180(u16 *unit_ids, s32 count, s32 *x, s32 *z);
struct BattleObj *BattleObjGet(s32 id);

void EnemyUpdate(void)
{
    struct BattleWork *work = Ram_HeapSlots->battle_work;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 last = -1;
    s32 count = 0;
    s32 i;
    s32 total;

    for (i = 0; i < 6 && work->enemy_ids[i] != 0xff; i++) {
        if (work->enemy_ids[i] == 0xfe) {
            if (count == 0)
                continue;
        } else {
            last = count;
        }
        unit_ids[count] = work->enemy_ids[i];
        count++;
    }
    total = last + 1;
    Func_0811b180(unit_ids, total, x, z);
    for (i = 0; i < total; i++) {
        s32 id = unit_ids[i];

        if (id != 0xfe) {
            struct BattleObj *object = BattleObjGet(id);

            object->anchor_x = x[i] << 16;
            object->anchor_z = z[i] << 16;
        }
    }
}

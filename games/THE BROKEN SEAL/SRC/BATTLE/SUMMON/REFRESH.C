#include "TYPES.H"
#include "BATTLE_WORK.H"
#include "MOTION_OBJECT.H"

void Summon_LayoutPositions(u16 *unit_ids, s32 count, s32 *x, s32 *z);

/*
 * Lays out the battle's enemy list again: collects up to six ids,
 * computes their standing positions and moves every unit still present
 * there.
 */
s32 Summon_Refresh(void)
{
    struct BattleSession *work = gBattleWork;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;

    for (i = 0; i < 6 && work->enemy_units[i] != 0xff; i++)
        unit_ids[i] = work->enemy_units[i];
    count = i;
    Summon_LayoutPositions(unit_ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = work->enemy_units[i];

        if (id != 0xfe) {
            struct BattleObjectSlot *object = GetBattleObjectSlot(id);

            object->anchor_x = x[i] << 16;
            object->anchor_z = z[i] << 16;
        }
    }
}

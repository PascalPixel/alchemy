#include "TYPES.H"

struct BattleUnitObject {
    u8 reserved_00[12];
    s32 x;
    s32 z;
};

struct BattleUnitLists {
    u8 reserved_000[0x66];
    s16 unit_ids[8];                /* 0x66; 0xff ends, 0xfe marks a removed unit */
};

extern struct BattleUnitLists *gBattleWork;

void Summon_LayoutPositions(u16 *unit_ids, s32 count, s32 *x, s32 *z);
struct BattleUnitObject *GetBattleObjectSlot(s32 unit_id);

/*
 * Lays out the battle's second unit list again: collects up to six ids,
 * computes their standing positions and moves every unit still present
 * there.
 */
s32 Summon_Refresh(void)
{
    struct BattleUnitLists *work = gBattleWork;
    u16 unit_ids[14];
    s32 x[6];
    s32 z[6];
    s32 count;
    s32 i;

    for (i = 0; i < 6 && work->unit_ids[i] != 0xff; i++)
        unit_ids[i] = work->unit_ids[i];
    count = i;
    Summon_LayoutPositions(unit_ids, count, x, z);
    for (i = 0; i < count; i++) {
        s32 id = work->unit_ids[i];

        if (id != 0xfe) {
            struct BattleUnitObject *object = GetBattleObjectSlot(id);

            object->x = x[i] << 16;
            object->z = z[i] << 16;
        }
    }
}

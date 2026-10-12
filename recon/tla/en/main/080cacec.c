#include "types.h"

struct SceneRegionEntry {
    s16 id;
    s16 flag;
    u8 unknown_04[4];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[4];
};

#include "ram_buffer.h"

struct MapViewBounds {
    u8 unknown_000[0xec];
    s32 min_x;
    s32 min_y;
    s32 max_x;
    s32 max_y;
};

void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);

void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
    struct MapViewBounds *view = (struct MapViewBounds *)Ram_HeapSlots->map_work;

    GameFlag_ClearBit(0x164);
    GameFlag_SetBit(0x165);
    while (entry->id != -1) {
        /* FAKEMATCH: reading the flag through a u16 adds one loop
           instruction, which keeps loop-invariant motion to the reference's
           single hoisted bound address. */
        u16 raw = entry->flag;
        s16 flag = raw;

        if (flag == 0) {
            s32 x = entry->x;
            s32 z = entry->z;

            if (view->min_x <= x && x <= view->max_x
                && view->min_y <= z && z <= view->max_y)
                entry->flag = 0x164;
            else
                entry->flag = 0x165;
        }
        entry++;
    }
}

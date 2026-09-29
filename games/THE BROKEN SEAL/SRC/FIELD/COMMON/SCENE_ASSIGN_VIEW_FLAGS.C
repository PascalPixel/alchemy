#include "TYPES.H"
#include "MAP_SCROLL.H"

/* One entry of a scene's region table, which ends at id -1. */
struct SceneRegionEntry {
    s16 id;
    s16 flag;
    u8 unknown_04[4];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[4];
};

void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

/* Give every entry without a flag of its own one of two fixed flags: 0x164,
   kept clear, when its position lies inside the camera's bounds, and 0x165,
   kept set, when it lies outside. */
void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
    struct MapScrollWork *view = Data_03001e70;

    GameFlag_ClearBitFar(0x164);
    GameFlag_SetBitFar(0x165);
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

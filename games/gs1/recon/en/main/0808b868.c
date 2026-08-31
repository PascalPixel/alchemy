#include "types.h"

struct SceneRegionEntry {
    s16 id;
    s16 flag;
    u8 padding04[4];
    s32 x;
    u8 padding0c[4];
    s32 z;
    u8 padding14[4];
};

struct SceneRegionBounds {
    u8 padding00[0xec];
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 maximum_z;
};

extern struct SceneRegionBounds *Data_03001e70;

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);

void Func_0808b868(struct SceneRegionEntry *entry)
{
    struct SceneRegionBounds *bounds = Data_03001e70;

    GameFlag_Clear(0x164);
    GameFlag_Set(0x165);
    while (entry->id != -1) {
        if (entry->flag == 0) {
            s32 x = entry->x;
            s32 z = entry->z;

            if (bounds->minimum_x <= x
                && x <= bounds->maximum_x
                && bounds->minimum_z <= z
                && z <= bounds->maximum_z)
                entry->flag = 0x164;
            else
                entry->flag = 0x165;
        }
        entry++;
    }
}

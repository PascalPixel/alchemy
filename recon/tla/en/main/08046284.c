#include "battle_system.h"

struct DjinnSlot {
    u16 id;
    u16 state;
};

s32 DjinnMenu_ListChangedDjinn(struct DjinnSlot *oldGrid, struct DjinnSlot *newGrid, u16 *out, s32 *gained, s32 *lost)
{
    s32 i;
    s32 j;
    s32 count;
    s32 added;
    s32 removed;

    added = 0;
    removed = 0;
    count = 0;
    for (i = 0; i < 32 && newGrid[i].id != 0; i++) {
        if (BattleActGet(newGrid[i].id)->targetFlag & 0x80) {
            out[count] = newGrid[i].id & 0x3fff;
            count++;
            for (j = 0; j < 32; j++) {
                if (((newGrid[i].id ^ oldGrid[j].id) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                added++;
                out[count - 1] |= 0x8000;
            }
        }
    }
    for (i = 0; i < 32 && oldGrid[i].id != 0; i++) {
        if (BattleActGet(oldGrid[i].id)->targetFlag & 0x80) {
            for (j = 0; j < 32; j++) {
                if (((oldGrid[i].id ^ newGrid[j].id) & 0x3fff) == 0)
                    break;
            }
            if (j == 32) {
                removed++;
                out[count] = (oldGrid[i].id & 0x3fff) | 0x4000;
                count++;
            }
        }
    }
    *gained = added;
    *lost = removed;
    return count;
}

#include "types.h"

#define SceneActor_FindSlotAtTile Func_020007c4

extern u8 *Data_03001ebc;

s32 *SceneActor_FindSlotAtTile(s32 x, s32 z) {
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

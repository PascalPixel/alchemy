#include "types.h"

extern u8 *Data_03001ebc;

s32 *SceneActor_FindAtTileXZ(s32 x, s32 z) {
    s32 **tbl = (s32 **)(Data_03001ebc + 0x14);
    u32 i;

    for (i = 8; i <= 65; i++) {
        s32 *p = tbl[i];

        if (x == (p[2] >> 20) && z == (p[4] >> 20)) {
            return p;
        }
    }
    return 0;
}

#include "types.h"
#include "map.h"

u8 Func_08012038(s32 arg0, s32 arg1, s32 arg2) {
    struct MapState *temp_r0;
    s32 var_r2;
    s32 idx;

    temp_r0 = *(struct MapState **)0x03001E70;
    arg1 >>= 0x14;
    arg2 >>= 0x14;
    var_r2 = 0x02010000;
    if (temp_r0 != 0) {
        idx = ((arg0 & 3) * 0x30) + 0x130;
        var_r2 = *(s32 *)((u8 *)temp_r0 + idx);
    }
    var_r2 += (arg1 + (arg2 << 7)) * 4;
    return *(u8 *)(var_r2 + 2);
}

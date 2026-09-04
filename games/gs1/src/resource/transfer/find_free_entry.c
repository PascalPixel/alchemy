#include "types.h"
#include "global_cells.h"

#define Resource_FindFreeTransferEntry Func_0801a910

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Resource_FindFreeTransferEntry(s32 kind) {
    s32 z0;
    s32 s;
    s32 z1;
    s32 state;
    s32 off;
    s32 i;
    s32 j;
    u32 ret;
    u16 *q;
    u16 *p;
    u32 v;

    (s32) s = (*(s32 *)ADDR_03001E98); state = s;
    if (kind != 0) {
        (s32) i = 0;
        p = state + 0x1DE;
        off = 0;
loop_2:
        if (*p == 0) {
            return state + off + 0x1D4;
        }
        z0 = 0; i = i + 1;
        (s32) p += 0x34;
        off = off + 0x34;
        if (i == 5) {
            goto block_10;
        }
        goto loop_2;
    }
    j = 0;
    ret = state + 0x68;
    q = state + 0x72;
loop_7:
    v = *q;
    (s32) q += 0x34;
    if (v == 0) {
        return ret;
    }
    z1 = 0; ret += 0x34;
    j += 1;
    if (j == 7) {
block_10:
        return 0;
    }
    goto loop_7;
}

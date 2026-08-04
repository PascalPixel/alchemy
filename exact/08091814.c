#include "types.h"

s32 Func_08077090(s32 group, s32 entry);
s32 Func_080770c0(s32 group);

s32 Func_08091814(u32 packed_id)
{
    s32 group = (packed_id >> 10) & 0xf;
    s32 entry = packed_id & 0x3ff;

    if (group > 7)
        return -1;
    if (Func_080770c0(group) == 0)
        return -2;
    if (Func_08077090(group, entry) == 0)
        return -3;
    return 0;
}

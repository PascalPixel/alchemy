#include "types.h"

u32 Func_08005b24(s32);
s32 Func_08005b64(s32);

u32 Func_08005ac0(s32 record_id)
{
    s32 index;
    s32 temp_r0;

    index = Func_08005b24(record_id);
    if (index > 0xFU) {
        return 1U;
    }
    temp_r0 = Func_08005b64(index);
    return (u32) ((0 - temp_r0) | temp_r0) >> 0x1F;
}

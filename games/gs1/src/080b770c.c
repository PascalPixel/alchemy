#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080b770c(s16 *entries, s32 id) {
    s16 *entry;
    s32 target;
    u16 value;
    u32 index;

    entry = entries;
    target = id;
    if (target > 7) {
        target += 0x78;
    }
    index = 0;
loop_3:
    value = (u16) *entry;
    entry += 1;
    if (value != 0xFF) {
        if (value == target) {
            return 1;
        }
        index += 1;
        if (index > 0xDU) {
            goto block_7;
        }
        goto loop_3;
    }
block_7:
    return 0;
}

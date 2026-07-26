#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_08077080();

u32 Func_080bd3c8(s32 arg0) {
    u8 temp_r3;

    if (arg0 == 0x7E) {
        return 1U;
    }
    temp_r3 = M2C_FIELD(Func_08077080(), u8 *, 9);
    return (u32) ((0 - temp_r3) | temp_r3) >> 0x1F;
}

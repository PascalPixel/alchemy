#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void *Func_08077008();
s32 Func_080771a0();

s32 Func_080bf208(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_r5;
    void *temp_r0;

    temp_r0 = Func_08077008();
    if (arg1 <= 5) {
        temp_r5 = (((M2C_FIELD(temp_r0, u8 *, 0x42) * 3) - (arg1 * 5)) + arg2) * 0x28F;
        if (temp_r5 >= (Func_080771a0() & 0xFFFF)) {
            return 1;
        }
    }
    return 0;
}

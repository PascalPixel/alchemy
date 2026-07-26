#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080043e0();
s32 Func_08009298();
s32 Func_080770d0(s32);

s32 Func_080a1050(void) {
    Func_080043e0();
    Func_08009298();
    Func_080770d0(0x166);
    return Func_080770d0(0x152);
}

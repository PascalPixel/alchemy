#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_0800430c();
s32 Func_08009290();
s32 Func_080770c8(s32);

s32 Func_080a1070(void) {
    Func_080770c8(0x152);
    Func_080770c8(0x166);
    Func_08009290();
    return Func_0800430c();
}

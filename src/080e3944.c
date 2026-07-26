#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08005268();

s32 Func_080e3944(s32 arg0, void *arg1) {
    Func_08005268();
    M2C_FIELD(arg1, s32 *, 4) = (s32) (M2C_FIELD(arg1, s32 *, 4) - 0x10);
}

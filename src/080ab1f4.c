#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080153c8(s32, s32, s32, s32, s32);

s32 Func_080ab1f4(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Func_080153c8(M2C_FIELD(arg0, u16 *, 0xC) + arg1 + 1, M2C_FIELD(arg0, u16 *, 0xE) + arg2 + 1, arg3, arg4, arg5);
}

#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080b50b0();

void Func_080e396c(s32 arg0, void *arg1) {
    Func_080b50b0();
    M2C_FIELD(arg1, s32 *, 4) = (s32) (M2C_FIELD(arg1, s32 *, 4) - 0x10);
}

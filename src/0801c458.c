#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08077048(s32);

s32 Func_0801c458(s32 arg0, s32 arg1) {
    Func_08077048(arg1);
    return 0;
}

#include "far_runtime.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_08015080(s32, s32, s32, s32);

void Func_080a3cf8(s32 arg0, s32 arg1) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_08015270(M2C_FIELD(temp_r5, s32 *, 0x10C));
    Func_08015080(arg1, M2C_FIELD(temp_r5, s32 *, 0x10C), 0, 0);
}

#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080a23f4(struct Fields_080a23f4 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void Func_080a4e20(void) {
    Func_080a23f4(M2C_FIELD(*(void **)0x03001F2C, s32 *, 0x20), 0xD, 5, 0x11, 0xA);
}

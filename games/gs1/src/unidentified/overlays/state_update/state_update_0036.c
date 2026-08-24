#include "types.h"
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s16 Func_0200150e(s32, s32);
void *Func_02001564(s32);

s32 Func_02000054(void *arg0) {
    void *temp_r0;

    temp_r0 = Func_02001564(0xA);
    M2C_FIELD(arg0, s16 *, 6) = Func_0200150e(M2C_FIELD(temp_r0, s32 *, 0x10) - M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(temp_r0, s32 *, 8) - M2C_FIELD(arg0, s32 *, 8));
    return 0;
}

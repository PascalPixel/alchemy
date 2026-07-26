#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s16 Func_080044d0(s32, s32);

s32 Func_0800da78(void *arg0) {
    void *temp_r2;

    temp_r2 = M2C_FIELD(arg0, void **, 0x68);
    M2C_FIELD(arg0, s16 *, 6) = Func_080044d0(M2C_FIELD(temp_r2, s32 *, 0x10) - M2C_FIELD(arg0, s32 *, 0x10), M2C_FIELD(temp_r2, s32 *, 8) - M2C_FIELD(arg0, s32 *, 8));
    M2C_FIELD(arg0, u16 *, 4) = (u16) (M2C_FIELD(arg0, u16 *, 4) + 1);
    return 1;
}

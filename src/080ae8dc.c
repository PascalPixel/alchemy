#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08003f3c(u16);

void Func_080ae8dc(void) {
    void *temp_r5;

    temp_r5 = *(void **)0x03001F2C;
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x392));
    Func_08003f3c(M2C_FIELD(temp_r5, u16 *, 0x394));
}

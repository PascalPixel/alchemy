#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080a1814(void *);
s32 Func_080a1870(s32, s32, s32, s32, s32);

void Func_080a8034(void) {
    void *temp_r6;

    temp_r6 = *(void **)0x03001F2C;
    Func_080a1870(Func_080a1814(temp_r6), 2, 2, 8, 0);
    M2C_FIELD(temp_r6, s32 *, 0x28) = 0;
    M2C_FIELD(temp_r6, s32 *, 0x24) = 0;
    M2C_FIELD(temp_r6, s32 *, 0x2C) = 0;
    M2C_FIELD(temp_r6, s32 *, 0x20) = 0;
    M2C_FIELD(temp_r6, s8 *, 0x110) = 0;
    M2C_FIELD(temp_r6, s8 *, 0x111) = 0;
    M2C_FIELD(temp_r6, s8 *, 0x112) = 8;
    M2C_FIELD(temp_r6, s8 *, 0x113) = 2;
}

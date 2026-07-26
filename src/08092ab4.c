#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08009080(void *, s32);
s32 Func_08009140();
void *Func_0808ba1c();

void Func_08092ab4(void) {
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s32 *, 0x38) = 0x80000000;
        M2C_FIELD(temp_r0, s32 *, 0x3C) = 0x80000000;
        M2C_FIELD(temp_r0, s32 *, 0x40) = 0x80000000;
        Func_08009140();
        Func_08009080(temp_r0, 1);
    }
}

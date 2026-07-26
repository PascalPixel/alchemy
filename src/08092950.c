#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_0808ba1c();
s32 Func_080929d8(void *, s32);

void Func_08092950(s32 arg0, s32 arg1) {
    s32 temp_r3;
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        temp_r3 = 0x100 & arg1;
        if (temp_r3 != 0) {
            M2C_FIELD(temp_r0, s32 *, 0x6C) = 0x08092981;
            return;
        }
        M2C_FIELD(temp_r0, s32 *, 0x6C) = temp_r3;
        Func_080929d8(temp_r0, arg1);
    }
}

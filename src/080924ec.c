#include "types.h"

typedef int bool;
#define NULL ((void *)0)
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_08009088(s32, s32);
s32 Func_0808ba1c();

void Func_080924ec(s32 arg0, s32 arg1) {
    s32 temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != 0) {
        Func_08009088(temp_r0, arg1);
    }
}

#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void *Func_0808ba1c();
s32 Func_08092054(u32);
s32 Func_08093a6c(void *, s32);

void Func_080920c0(s32 arg0, s32 arg1) {
    void *temp_r0;

    temp_r0 = Func_0808ba1c();
    if (temp_r0 != NULL) {
        M2C_FIELD(temp_r0, s32 *, 0x68) = Func_08092054(arg1);
        Func_08093a6c(temp_r0, 0x0809FF40);
    }
}

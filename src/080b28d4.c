#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_0808a540(u16);
void Func_08015140(void);
s32 Func_080b2884(s32);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_080030f8(u32);

extern void *Data_03001f2c;

void Func_080b28d4(s32 arg0) {
    s32 var_r5;

    var_r5 = Func_0808a540(M2C_FIELD(Data_03001f2c, u16, 0x3A4));
    Func_08015140();
    arg0 = Func_080b2884(arg0);
    Func_08015038(arg0, 5, 0, (var_r5 << 0x10) | 0x22);
    while (Func_08015048() == 0) {
        Func_080030f8(1U);
    }
    Func_080030f8(1U);
}

#include "types.h"

struct Work_080b28d4 {
    u8 unknown_000[0x3a4];
    u16 value_3a4;
};

s32 Func_0808a540(u16);
void Func_08015140(void);
s32 Func_080b2884(s32 value);
void Func_08015038(s32, s32, s32, s32);
s32 Func_08015048(void);
void Func_080030f8(u32);

extern struct Work_080b28d4 *Data_03001f2c;

void Func_080b28d4(s32 value)
{
    s32 param;

    param = Func_0808a540(Data_03001f2c->value_3a4);
    Func_08015140();
    value = Func_080b2884(value);
    Func_08015038(value, 5, 0, (param << 0x10) | 0x22);
    while (Func_08015048() == 0) {
        Func_080030f8(1U);
    }
    Func_080030f8(1U);
}

#include "types.h"

extern u8 *Data_03001ebc;
extern s16 Data_02000240[];

s32 Func_0808a5d0(s16, s16);
s32 Func_080187ac(s32, s32, s32, s32, s32);
s32 Func_080162d4(s32, s32, s32, s32, s32);
void Func_0801e74c(s32, s32, s32, s32);
s32 Func_080041d8(const void *, s32);

void Func_0801c34c(void)
{
    register u8 *base;
    s32 fourth;
    s32 third;
    s32 y;
    s32 x;
    s32 resource;
    s32 object;
    u16 *field;

    base = Data_03001ebc;
    x = 8;
    y = 8;
    resource = Func_0808a5d0(Data_02000240[224], Data_02000240[225]) + 0x99b;
    Func_080187ac(resource, (s32)&x, (s32)&y, (s32)&third, (s32)&fourth);
    x = (30 - third) >> 1;
    y = (10 - fourth) >> 1;
    object = Func_080162d4(x, y, third, fourth, 2);
    *(s32 *)(base + 0x230) = object;
    Func_0801e74c(resource, object, 0, 0);
    field = (u16 *)(base + 0x234);
    {
        register s32 ninety = 90;
        *field = ninety;
    }
    Func_080041d8((void *)0x0801c3e9, 0xc80);
}

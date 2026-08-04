#include "types.h"

extern void Func_02008934(void);
extern void Func_02008938(s32);
extern s32 Func_02008aa0(s32, s32);
extern void Func_02008948(s32);
extern void Func_0200896c(void);

void Func_02004234(void)
{
    u8 *state;

    Func_02008934();
    state = *(u8 **)0x03001ebc;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    Func_02008938(0x87c);
    Func_02008aa0(12, 2);
    Func_02008948(0x900);
    Func_0200896c();
}

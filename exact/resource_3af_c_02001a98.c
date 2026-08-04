#include "types.h"

extern void Func_02005d88(void);
extern void Func_02005ec2(s32, s32, s32, s32);
extern void Func_02005d10(s32);
extern void Func_02005e2a(s32, s32, s32);
extern void Func_02005e34(s32, s32, s32);
extern void Func_02005e3e(s32, s32, s32);
extern void Func_02005e48(s32, s32, s32);
extern void Func_02005e52(s32, s32, s32);
extern void Func_02005e5c(s32, s32, s32);
extern void Func_02005e66(s32, s32, s32);
extern void Func_02005e70(s32, s32, s32);
extern u8 *Func_02005e16(s32);
extern void Func_02005e88(s32, s32, s32);
extern u8 *Func_02005e2e(s32);
extern void Func_02005f4e(s32, s32, s32, s32);
extern void Func_02005dc2(void);
extern void Func_02005da0(s32);
extern void Func_02003760(s32, s32);

void Func_02001a98(void)
{
    Func_02005d88();
    Func_02005ec2(-1, -1, -1, 0);
    Func_02005d10(1);
    Func_02005e2a(20, 0, 0);
    Func_02005e34(22, 0, 0);
    Func_02005e3e(24, 0, 0);
    Func_02005e48(25, 0, 0);
    Func_02005e52(26, 0, 0);
    Func_02005e5c(27, 0, 0);
    Func_02005e66(0, 0, 0);
    Func_02005e70(23, 0, 0);
    {
        u8 *q = Func_02005e16(23);
        s32 x = 0xC0;
        *(u16 *)(q + 6) = x << 6;
    }
    Func_02005e88(21, 0xE80000, 0x28A0000);
    {
        u8 *q = Func_02005e2e(21);
        s32 x = 0xB0;
        *(u16 *)(q + 6) = x << 8;
    }
    Func_02005f4e(0xE80000, -1, 0x27C0000, 0);
    Func_02005dc2();
    Func_02005da0(1);
    Func_02003760(23, 21);
}

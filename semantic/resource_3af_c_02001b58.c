#include "types.h"

extern void Func_02005e48(void);
extern void Func_02005e5e(s32);
extern void Func_02005dc4(s32);
extern void Func_02005ee2();
extern void Func_02005f22(s32, s32);
extern s32 Func_02005e90(s32);
extern void Func_02005e2e(s32, s32);
extern void Func_02005dec(s32);
extern void Func_02005f9c(s32, s32);
extern void Func_02005e20(void);
extern void Func_02005dfe(s32);
extern void Func_02005edc(s32);
extern void Func_02005e10(s32);
extern void Func_02005f2a(s32, s32, s32);
extern void Func_02005f34(s32, s32, s32);
extern void Func_02005f3e(s32, s32, s32);
extern u8 *Func_02005ee4(s32);
extern void Func_02005f56(s32, s32, s32);
extern u8 *Func_02005efc(s32);
extern void Func_02005e58(s32);
extern void Func_02003818(s32, s32);

extern u8 Data_0200d160[];

void Func_02001b58(void)
{
    Func_02005e48();
    Func_02005e5e((s32)Data_0200d160);
    Func_02005dc4(1);
    Func_02005ee2(0, 0xE80000, 0x27C0000);
    Func_02005f22(0, 15);
    Func_02005e2e(Func_02005e90(0), 0);
    Func_02005dec(1);
    Func_02005f9c(0, 0);
    Func_02005e20();
    Func_02005dfe(1);
    Func_02005edc(22);
    Func_02005ee2(21);
    Func_02005e10(1);
    Func_02005f2a(22, 0, 0);
    Func_02005f34(21, 0, 0);
    Func_02005f3e(20, 0, 0);
    {
        u8 *q = Func_02005ee4(20);
        s32 x = 0xC0;
        *(u16 *)(q + 6) = x << 6;
    }
    Func_02005f56(23, 0xE80000, 0x28A0000);
    {
        u8 *q = Func_02005efc(23);
        s32 x = 0xB0;
        *(u16 *)(q + 6) = x << 8;
    }
    Func_02005e58(1);
    Func_02003818(20, 23);
}

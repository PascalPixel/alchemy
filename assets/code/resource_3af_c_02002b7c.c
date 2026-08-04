#include "types.h"

extern void Func_02006e6c(void);
extern void Func_02006f2c(s32, s32);
extern s32 Func_02006e9a(s32);
extern void Func_02006e38(s32, s32);
extern void Func_02006e96(s32);
extern void Func_02006dfc(s32);
extern void Func_02006f1a(s32, s32, s32);
extern u8 *Func_02006ec0(s32);
extern void Func_02006f34(s32, s32, s32);
extern u8 *Func_02006eda(s32);
extern void Func_02006fc0(s32, s32);
extern void Func_02006f56(s32, s32, s32);
extern u8 *Func_02006efc(s32);
extern void Func_02007040(void);
extern void Func_02007054(void);
extern void Func_02006ef2(s32);
extern void Func_02006f9c(s32, s32, s32);
extern void Func_02006fa6(s32, s32, s32);
extern void Func_02006fd4(s32);
extern void Func_0200661a(s32);
extern void Func_02006fb2(s32, s32);
extern void Func_02006f60(s32, s32, s32);
extern void Func_02006faa(s32, s32, s32);
extern void Func_0200702c(s32, s32, s32);
extern void Func_02006ff4(s32, s32);
extern void Func_02006652(s32);
extern void Func_02007090(s32);

extern u8 Data_0200d160[];
extern s32 *Data_03001ebc;

void Func_02002b7c(void)
{
    s32 m = 0xB0;

    m <<= 8;
    Func_02006e6c();
    Func_02006f2c(0, 15);
    Func_02006e38(Func_02006e9a(0), 0);
    Func_02006e96((s32)Data_0200d160);
    Func_02006dfc(1);
    Func_02006f1a(20, 0xC40000, 0x1F60000);
    {
        s32 x = 0xA0;

        *(u16 *)(Func_02006ec0(20) + 6) = x << 8;
    }
    Func_02006f34(22, 0xB80000, 0x20C0000);
    *(u16 *)(Func_02006eda(22) + 6) = m;
    Func_02006fc0(21, 1);
    Func_02006f56(21, 0xB80000, 0x2780000);
    *(u16 *)(Func_02006efc(21) + 6) = m;
    Data_03001ebc[0x70] = 0x202;
    Func_02007040();
    Func_02007054();
    Func_02006ef2(20);
    Func_02006f9c(22, 4, 10);
    Func_02006fa6(22, 6, 20);
    Func_02006fd4(0x1EE5);
    Func_0200661a(22);
    Func_02006fb2(20, 3);
    Func_02006f60(21, 0x30000, 0x18000);
    Func_02006faa(21, 180, 0x222);
    Func_0200702c(21, m, 40);
    Func_02006ff4(21, 1);
    Func_02006652(21);
    Func_02007090(15);
}

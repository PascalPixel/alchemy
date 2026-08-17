#include "types.h"

extern u8 Value_0000096c;
extern u8 Value_00002233;

extern void Func_020081ee(s32);
extern void Func_0200822a(void);
extern void Func_02008356(void);
extern void Func_0200830a(s32, s32, s32);
extern void Func_02008316(s32, s32, s32);
extern void Func_020082aa(s32, s32, s32);
extern s32 Func_0200832e(s32, s32, s32);
extern void Func_0200825c(s32);
extern void Func_02008324(s32);
extern s32 Func_02008334(s32, s32);
extern s32 Func_02008294(s32, s32);
extern void Func_0200827e(s32);
extern void Func_02008344(s32);
extern void Func_0200835c(s32, s32);
extern void Func_02008294_b(s32);
extern void Func_0200835a(s32);
extern s32 Func_02008372(s32, s32);
extern void Func_020082a8(s32);
extern void Func_0200836a(s32, s32, s32);
extern s32 Func_02008396(s32, s32, s32);
extern void Func_020082c4(s32);
extern s32 Func_02008374(s32, s32);
extern void Func_020082d2(s32);
extern void Func_02008394(s32, s32, s32);
extern void Func_020082ea(s32);
extern void Func_020083d6(s32, s32, s32);
extern void Func_020083da(s32, s32, s32);
extern s32 Func_020083e6(s32, s32, s32);
extern void Func_02008314(s32);
extern s32 Func_020083ac(s32, s32);
extern void Func_02008322(s32);
extern s32 Func_020083fa(s32, s32);
extern void Func_02008330(s32);
extern s32 Func_020083e0(s32, s32);
extern void Func_0200833e(s32);
extern void Func_02008416(s32, s32);
extern void Func_0200835a_b(void);

void Func_02003e40(void)
{
    s32 k;

    Func_020081ee((s32)&Value_0000096c);
    Func_0200822a();
    Func_02008356();
    Func_0200830a(8, 0x5000, 0);
    Func_02008316(9, 0x3000, 0);
    Func_020082aa(0, 200, 0x110);
    Func_0200832e(0, 0xc000, 0);
    Func_0200825c(20);
    k = (s32)&Value_00002233;
    Func_02008324(k);
    Func_02008334(8, 0);
    if (Func_02008294(0, 0) == 0) {
        Func_0200827e(20);
        Func_02008344(k + 1);
        Func_0200835c(8, 0);
    } else {
        Func_02008294_b(20);
        Func_0200835a(k + 2);
        Func_02008372(8, 0);
        Func_020082a8(20);
        Func_0200836a(8, 9, 60);
        Func_02008396(9, 0x3000, 0);
        Func_020082c4(40);
        Func_02008374(9, 2);
        Func_020082d2(30);
        Func_02008394(8, 9, 30);
        Func_02008374(9, 3);
        Func_020082ea(30);
        Func_020083d6(8, 0x102, 50);
        Func_020083da(8, 0x5000, 0);
        Func_020083e6(9, 0x3000, 0);
        Func_02008314(20);
        Func_020083ac(8, 4);
        Func_02008322(20);
        Func_020083fa(8, 0);
        Func_02008330(10);
        Func_020083e0(8, 2);
        Func_0200833e(20);
        Func_02008416(8, 0);
    }
    Func_0200835a_b();
}

#include "types.h"

extern u8 *Func_02006fd6(s32);
extern u8 *Func_02006fde(s32);
extern void Func_02006fbc(void);
extern void Func_02006eb4(s32);
extern void Func_02007022();
extern void Func_02007064();
extern void Func_020070be(s32, s32, s32);
extern void Func_020070d4(s32);
extern void Func_020070fe(s32, s32, s32);
extern void Func_0200710a(s32, s32, s32);
extern void Func_020070c2(s32, s32);
extern void Func_02007030(s32);
extern void Func_020070c8(s32, s32);
extern u8 *Func_0200706e(s32);
extern void Func_020070a8(s32, s32, s32);
extern void Func_020070d6(s32);
extern void Func_020070e8(s32, s32, s32);
extern void Func_020070da();
extern void Func_0200707e(void);

void Func_02000fec(void)
{
    u8 *a = Func_02006fd6(0);
    u8 *b = Func_02006fde(5);
    Func_02006fbc();
    *(s32 *)(b + 8) = *(s32 *)(a + 8);
    *(s32 *)(b + 12) = *(s32 *)(a + 12);
    *(s32 *)(b + 16) = *(s32 *)(a + 16);
    *(s32 *)(b + 0x38) = 0x80000000;
    *(s32 *)(b + 0x3c) = 0x80000000;
    *(s32 *)(b + 0x40) = 0x80000000;
    *(s32 *)(b + 0x24) = 0;
    *(s32 *)(b + 0x28) = 0;
    *(s32 *)(b + 0x2c) = 0;
    *(s32 *)(b + 0x14) = *(s32 *)(a + 12);
    Func_02006eb4(1);
    Func_02007022(5, 0x10000, 0x8000);
    Func_02007064(5, 110, 0x11b);
    Func_020070be(0, 5, 2);
    Func_020070d4(0xf39);
    if (*(s32 *)(a + 8) < *(s32 *)(b + 8)) {
        Func_020070fe(0xa005, 0, 2);
    } else {
        Func_0200710a(0x8005, 0, 2);
    }
    Func_020070c2(0, 3);
    Func_02007030(2);
    Func_020070c8(5, 2);
    {
        u8 *c = Func_0200706e(0);
        if (c != 0) {
            Func_020070a8(5, *(s16 *)(c + 10), *(s16 *)(c + 18));
        }
    }
    Func_020070d6(5);
    Func_020070e8(5, 0, 0);
    Func_020070da(0, 110, 0x12f);
    Func_0200707e();
}

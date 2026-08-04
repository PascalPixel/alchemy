#include "types.h"

extern s32 Func_0200561a(s32);
extern void Func_02005644(void);
extern s32 Func_0200562a(s32);
extern u8 *Func_02005684(s32);
extern void Func_02005626(s32, s32, s32);
extern void Func_02005824(s32);
extern void Func_020055b2(s32);
extern void Func_02005830(s32);
extern void Func_02005714(s32, s32, s32);
extern void Func_0200572a(s32, s32, s32);
extern void Func_020057ba(s32, s32);
extern void Func_02005740(s32, s32, s32);
extern void Func_020057d0(s32, s32);
extern void Func_02005754(s32, s32, s32);
extern void Func_020056d2(s32);
extern void Func_02005898(s32);
extern void Func_020056ae(s32, s32, s32);
extern void Func_020056ba(void);
extern void Func_020058a6(void);
extern void Func_020056e4(s32);
extern void Func_020020a8(void);
extern void Func_020056ee(s32);
extern s32 Func_020056ec(s32);
extern s32 Func_020056f6(s32);
extern s32 Func_02005702(s32);
extern u8 *Func_0200575c(s32);
extern s32 Func_02002aa6(s32, s32);
extern void Func_020057c8(s32, s32, s32);
extern s32 Func_02002abc(s32, s32);
extern void Func_020057de(s32, s32, s32);
extern void Func_0200574e(s32);
extern void Func_02005772(void);


void Func_02000f38(void)
{
    u8 *obj;
    u8 *obj2;
    s32 e1;
    s32 e2;
    s32 c1 = 0x311;
    s32 c2 = 0x831;
    s32 c3 = 0x311;
    s32 c4 = 0x831;
    s32 c5 = 0x30c;
    s32 c6 = 0x30c;

    if (Func_0200561a(c1) == 0) {
        Func_02005644();
        if (Func_0200562a(c2) == 0) {
            obj = Func_02005684(12);
            e1 = 0x40000;
            e2 = 0x40000;
            Func_02005626(e1, e2, 0x10000);
            Func_02005824(0x8d);
            Func_020055b2(40);
            Func_02005830(0x91);
            Func_02005714(12, 0x17d0000, 0x3280000);
            *(s32 *)(obj + 0x30) = 0x18000;
            *(s32 *)(obj + 0x34) = 0x18000;
            *(s32 *)(obj + 0xc) += 0x1000000;
            *(s32 *)(obj + 0x3c) = *(s32 *)(obj + 0xc);
            *(s32 *)(obj + 0x44) = 0x8000;
            Func_0200572a(12, 0x122, 0x341);
            Func_020057ba(12, 1);
            Func_02005740(12, 0x102, 0x354);
            Func_020057d0(12, 2);
            Func_02005754(12, 0xe0, 0x368);
            Func_020056d2(40);
            Func_02005898(0x121);
            Func_020056ae(-1, -1, 0xe666);
            Func_020056ba();
            Func_020058a6();
            Func_020056e4(c4);
        }
        Func_020020a8();
        Func_020056ee(c3);
        if (Func_020056ec(0x837) != 0 && Func_020056f6(0x841) == 0 && Func_02005702(c5) == 0) {
            obj2 = Func_0200575c(0);
            if (*(s32 *)(obj2 + 0xc) > 0x800000) {
                Func_02002aa6(0xdb, 0x34b);
                Func_020057c8(0, 0xb3, 0x33d);
            } else {
                Func_02002abc(0xd6, 0x38c);
                Func_020057de(0, 0xdb, 0x38f);
            }
            Func_0200574e(c6);
        }
        Func_02005772();
    }
}

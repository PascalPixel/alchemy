#include "types.h"

extern void Func_0200345a();
extern s32 Func_0200348a();
extern void Func_02002184();
extern void Func_02003484();
extern void Func_020021a4();
extern void Func_020034a4();
extern s32 Func_020034d2();
extern void Func_020021cc();
extern void Func_020034cc();
extern void Func_020034de();
extern s32 Func_0200350e();
extern void Func_02002208();
extern void Func_02003504();
extern void Func_02003514();
extern s32 Func_02003542();
extern void Func_0200223c();
extern void Func_0200353c();
extern void Func_0200225c();
extern void Func_0200355c();
extern s32 Func_0200358a();
extern void Func_02002284();
extern void Func_02003584();
extern void Func_020022a4();
extern void Func_020035a4();
void DialogueLayout_ConfigureGroupTwo(void)
{
    { s32 f1 = 12; s32 g1 = 8; Func_0200345a(0, 28, 10, 18,  f1, g1); }

    if (Func_0200348a((s32)0x304) != 0) {
        Func_02002184(8, 21, 20);
        { s32 f2 = 13; s32 g2 = 19; Func_02003484(20, 19, 1, 3,  f2, g2); }
    } else {
        Func_020021a4(8, 13, 20);
        { s32 f3 = 21; s32 g3 = 19; Func_020034a4(20, 19, 1, 3,  f3, g3); }
    }

    if (Func_020034d2((s32)0x305) != 0) {
        Func_020021cc(8, 12, 20);
        { s32 f4 = 12; s32 g4 = 19; Func_020034cc(5, 19, 1, 3,  f4, g4); }
        { s32 f5 = 13; s32 g5 = 19; Func_020034de(20, 19, 1, 3,  f5, g5); }
        if (Func_0200350e((s32)0x304) != 0) {
            Func_02002208(8, 21, 20);
            { s32 f6 = 13; s32 g6 = 19; Func_02003504(20, 19, 1, 3,  f6, g6); }
            { s32 f7 = 12; s32 g7 = 19; Func_02003514(20, 19, 1, 3,  f7, g7); }
        }
    }

    if (Func_02003542((s32)0x306) != 0) {
        Func_0200223c(9, 15, 21);
        { s32 f8 = 14; s32 g8 = 17; Func_0200353c(14, 18, 3, 1,  f8, g8); }
    } else {
        Func_0200225c(9, 15, 17);
        { s32 f9 = 14; s32 g9 = 21; Func_0200355c(14, 18, 3, 1,  f9, g9); }
    }

    if (Func_0200358a((s32)0x307) != 0) {
        Func_02002284(10, 19, 8);
        { s32 f10 = 18; s32 g10 = 25; Func_02003584(14, 18, 3, 1,  f10, g10); }
    } else {
        Func_020022a4(10, 19, 25);
        { s32 f11 = 18; s32 g11 = 8; Func_020035a4(14, 18, 3, 1,  f11, g11); }
    }
}

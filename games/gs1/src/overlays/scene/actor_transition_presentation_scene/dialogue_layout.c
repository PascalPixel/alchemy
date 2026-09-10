#include "types.h"

#define DialogueLayout_ConfigureGroupOne Func_02001050
#define DialogueLayout_ConfigureGroupTwo Func_02001154
#define DialogueLayout_ConfigureGroupThree Func_020012cc
#define FieldScene_RunFlagBranchedLayoutSteps Func_020015dc
extern u16 Data_02000240[];

void Func_02003356();
s32 Func_02003384();
void Func_0200207e();
void Func_0200337c();
void Func_0200209c();
void Func_0200339c();
s32 Func_020033ca();
void Func_020020c4();
void Func_020033c4();
void Func_020020e4();
void Func_020033e4();
s32 Func_02003412();
void Func_0200210c();
void Func_0200340c();
void Func_0200212c();
void Func_0200342c();
void Func_0200345a();
s32 Func_0200348a();
void Func_02002184();
void Func_02003484();
void Func_020021a4();
void Func_020034a4();
s32 Func_020034d2();
void Func_020021cc();
void Func_020034cc();
void Func_020034de();
s32 Func_0200350e();
void Func_02002208();
void Func_02003504();
void Func_02003514();
s32 Func_02003542();
void Func_0200223c();
void Func_0200353c();
void Func_0200225c();
void Func_0200355c();
s32 Func_0200358a();
void Func_02002284();
void Func_02003584();
void Func_020022a4();
void Func_020035a4();
void Func_020022fc();
void Func_02002326();
void Func_0200236a();
void Func_02002392();
void Func_020023b2();
void Func_020023da();
void Func_0200240e();
void Func_02002450();
void Func_02002494();
void Func_020024d8();
void Func_020024ec();
void Func_02002520();
void Func_02002552();
void Func_02002584();
void Func_020025b6();
void Func_020035d2();
void Func_020035fc();
s32 Func_02003602();
void Func_02003626();
s32 Func_0200362c();
void Func_02003638();
void Func_0200364a();
void Func_0200366a();
void Func_02003692();
s32 Func_02003698();
void Func_020036b2();
void Func_020036da();
s32 Func_020036e0();
void Func_0200370e();
s32 Func_02003714();
void Func_02003720();
void Func_02003750();
s32 Func_02003756();
void Func_02003762();
void Func_02003794();
s32 Func_0200379a();
void Func_020037a6();
void Func_020037b8();
void Func_020037ec();
s32 Func_020037f2();
void Func_02003820();
s32 Func_02003826();
void Func_02003852();
s32 Func_02003858();
void Func_02003884();
s32 Func_0200388a();
void Func_02003896();
void Func_020038f8();
void Func_0200390e();
s32 Func_0200393c();
void Func_02002636();
void Func_02003936();
void Func_02002656();
void Func_02003956();
s32 Func_02003986();
void Func_02002680();
void Func_02003980();
void Func_020026a0();
void Func_020039a0();
s32 Func_020039ce();
void Func_020026c8();
void Func_020039c8();
void Func_020026e8();
void Func_020039e8();

/* Imports; the three queried ones are typed for their return value. */

/* Dialogue layout for resource_39a. */

/*
 * 0x02000240 is below the link base, so it is a resident table; entry 225 is
 * read here as an unsigned halfword.
 */

/* Imports; the queried ones are typed for their return value. */

void DialogueLayout_ConfigureGroupOne(void)
{
    { s32 f1 = 8; s32 g1 = 29; Func_02003356(8, 42, 15, 5,  f1, g1); }

    if (Func_02003384((s32)0x301) != 0) {
        Func_0200207e(8, 22, 31);
        { s32 f2 = 8; s32 g2 = 30; Func_0200337c(9, 30, 1, 3,  f2, g2); }
    } else {
        Func_0200209c(8, 8, 31);
        { s32 f3 = 22; s32 g3 = 30; Func_0200339c(9, 30, 1, 3,  f3, g3); }
    }

    if (Func_020033ca((s32)0x302) != 0) {
        Func_020020c4(9, 12, 29);
        { s32 f4 = 11; s32 g4 = 33; Func_020033c4(14, 33, 3, 1,  f4, g4); }
    } else {
        Func_020020e4(9, 12, 33);
        { s32 f5 = 11; s32 g5 = 29; Func_020033e4(14, 29, 3, 1,  f5, g5); }
    }

    if (Func_02003412((s32)0x303) != 0) {
        Func_0200210c(10, 18, 29);
        { s32 f6 = 17; s32 g6 = 33; Func_0200340c(14, 33, 3, 1,  f6, g6); }
    } else {
        Func_0200212c(10, 18, 33);
        { s32 f7 = 17; s32 g7 = 29; Func_0200342c(14, 29, 3, 1,  f7, g7); }
    }
}

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

void DialogueLayout_ConfigureGroupThree(void)
{
    { s32 k5 = 12, k6 = 21; Func_020035d2(12, 3, 9, 16, k5, k6); }

    if (Func_02003602((s32)0x308) != 0) {
        Func_020022fc(8, 14, 25);
        { s32 k5 = 20, k6 = 24; Func_020035fc(16, 24, 1, 3, k5, k6); }
    } else if (Func_0200362c((s32)0x309) != 0) {
        Func_02002326(8, 17, 25);
        { s32 k6 = 24;
          Func_02003626(18, 24, 1, 3, 20, k6);
          Func_02003638(18, 24, 1, 3, 14, k6);
          Func_0200364a(8, 41, 1, 3, 17, k6);
        }
    } else {
        Func_0200236a(8, 20, 25);
        { s32 k5 = 14, k6 = 24; Func_0200366a(16, 24, 1, 3, k5, k6); }
    }

    if (Func_02003698((s32)0x30a) != 0) {
        Func_02002392(9, 13, 35);
        { s32 k5 = 15, k6 = 34; Func_02003692(14, 34, 1, 3, k5, k6); }
    } else {
        Func_020023b2(9, 15, 35);
        { s32 k5 = 13, k6 = 34; Func_020036b2(14, 34, 1, 3, k5, k6); }
    }

    if (Func_020036e0((s32)0x30b) != 0) {
        Func_020023da(10, 15, 22);
        { s32 k5 = 14;
          Func_020036da(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 41, 3, 1, k5, 22);
        }
    } else if (Func_02003714((s32)0x30c) != 0) {
        Func_0200240e(10, 15, 23);
        { s32 k5 = 14;
          Func_0200370e(5, 42, 3, 1, k5, 23);
          Func_02003720(14, 29, 3, 1, k5, 30);
          Func_020037b8(10, 44, 3, 1, k5, 21);
        }
    } else if (Func_02003756((s32)0x30d) != 0) {
        Func_02002450(10, 15, 26);
        { s32 k5 = 14;
          Func_02003750(14, 29, 3, 1, k5, 22);
          Func_02003762(5, 43, 3, 1, k5, 26);
          Func_020037b8(14, 29, 3, 1, k5, 30);
        }
    } else if (Func_0200379a((s32)0x30e) != 0) {
        Func_02002494(10, 15, 27);
        { s32 k5 = 14;
          Func_02003794(14, 29, 3, 1, k5, 22);
          Func_020037a6(14, 29, 3, 1, k5, 30);
          Func_020037b8(5, 44, 3, 1, k5, 27);
        }
    } else {
        Func_020024d8(10, 15, 30);
    }

    if (Func_020037f2((s32)0x30f) != 0) {
        Func_020024ec(11, 15, 23);
        { s32 k5 = 14;
          Func_020037ec(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 40, 3, 1, k5, 23);
        }
    } else if (Func_02003826((s32)0x310) != 0) {
        Func_02002520(11, 15, 24);
        { s32 k5 = 14;
          Func_02003820(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 41, 3, 1, k5, 24);
        }
    } else if (Func_02003858((s32)0x311) != 0) {
        Func_02002552(11, 15, 27);
        { s32 k5 = 14;
          Func_02003852(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 42, 3, 1, k5, 27);
        }
    } else if (Func_0200388a((s32)0x312) != 0) {
        Func_02002584(11, 15, 28);
        { s32 k5 = 14;
          Func_02003884(14, 29, 3, 1, k5, 31);
          Func_02003896(10, 43, 3, 1, k5, 28);
        }
    } else {
        Func_020025b6(11, 15, 31);
    }
}

/*
 * Four flag-branched layout steps.  Nothing is returned; the three pool words
 * after the return belong to the owner.  The eight bytes of frame are the
 * fifth and sixth arguments of the six-argument layout calls.  Imports are
 * named by the address their call site computes, and are old-style because
 * arity varies between sites.
 */
void FieldScene_RunFlagBranchedLayoutSteps(void)
{
    /*
     * The byte offset 450 is built by shifting, giving entry 225.  The test
     * is (entry - 1) << 16 against 0x10000 with an unsigned compare, which
     * selects exactly entries 1 and 2.
     */
    if ((u32)((u32)(Data_02000240[225] - 1) << 16) <= (u32)0x10000) {
        { s32 f1 = 14; s32 g1 = 10; Func_020038f8(22, 20, 9, 8,  f1, g1); }
    } else {
        { s32 f2 = 7; s32 g2 = 45; Func_0200390e(20, 45, 11, 4,  f2, g2); }
    }

    if (Func_0200393c((s32)0x313) != 0) {
        Func_02002636(8, 20, 17);
        { s32 f3 = 19; s32 g3 = 10; Func_02003936(19, 11, 3, 1,  f3, g3); }
    } else {
        Func_02002656(8, 20, 10);
        { s32 f4 = 19; s32 g4 = 17; Func_02003956(19, 11, 3, 1,  f4, g4); }
    }

    /* 0x314 is built by shifting. */
    if (Func_02003986((s32)0x314) != 0) {
        Func_02002680(9, 14, 16);
        { s32 f5 = 22; s32 g5 = 15; Func_02003980(16, 15, 1, 3,  f5, g5); }
    } else {
        Func_020026a0(9, 22, 16);
        { s32 f6 = 14; s32 g6 = 15; Func_020039a0(16, 15, 1, 3,  f6, g6); }
    }

    if (Func_020039ce((s32)0x315) != 0) {
        Func_020026c8(10, 17, 46);
        { s32 f7 = 7; s32 g7 = 45; Func_020039c8(15, 15, 1, 3,  f7, g7); }
    } else {
        Func_020026e8(10, 7, 46);
        { s32 f8 = 17; s32 g8 = 45; Func_020039e8(15, 15, 1, 3,  f8, g8); }
    }
}

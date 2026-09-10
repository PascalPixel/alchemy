#include "types.h"

#define FieldScene_RunScene394SequenceA Func_020003f0

extern u8 Data_00000001[];
void Func_020006de();
void Func_020006f2();
void Func_02000706();
void Func_02000720();
void Func_02000734();
void Func_02000748();
void Func_020007a8();
void Func_020007bc();
void Func_020007d0();
void Func_02000802();
void Func_02000816();
void Func_0200082a();
void Func_02000952();
void Func_02001458();
void Func_0200146e();
void Func_02001484();
void Func_02001492();
void Func_0200149c();
void Func_020014a4();
void Func_020014c0();
void Func_020014c2();
void Func_020014d6();
void Func_020014d8();
void Func_020014e6();
void Func_020014ee();
void Func_020014fe();
void Func_02001500();
void Func_02001506();
void Func_0200150c();
void Func_02001514();
void Func_02001516();
void Func_0200151c();
void Func_0200152a();
void Func_0200153a();
void Func_02001540();
void Func_02001556();
void Func_02001568();
void Func_02001578();
void Func_0200157e();
void Func_0200158e();
void Func_02001594();
void Func_02001594_a();
void Func_020015a4();
void Func_020015aa();
void Func_020015be();
void Func_020015d2();
void Func_020015de();
void Func_020015e2();
void Func_020015f4();
void Func_020015f8();
void Func_0200160a();
void Func_0200160e();
void Func_02001620();
void Func_02001628();
void Func_02001636();
void Func_0200163a();
void Func_0200165e();
void Func_02001674();
void Func_0200168a();
void Func_020016b2();
void Func_020016c2();
void Func_020016c4();
void Func_020016da();
void Func_020016f0();
void Func_020016f4();
void Func_02001794();
void Func_020017a4();
void Func_020017ba();
void Func_020017c6();
void Func_020017ea();
void Func_02001802();
void Func_0200180e();
void Func_0200186a();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call7(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    f(a0, a1, a2, a3, a4, a5, a6);
}

void FieldScene_RunScene394SequenceA(void)
{
    u32 i;
    s32 record;
    s32 v5;
    s32 base5_200a0dc;
    s32 v3;

    Func_02001492();
    Call2(Func_020014e6, 0x10000, 0x2000);
    Call4(Func_02001500, 0x1080000, -1, 0x1c00000, 1);
    Func_0200150c();
    Call2(Func_020014a4, 0x1528, 1);
    Func_0200153a(232);
    if (*(s16 *)(*(s32 *)0x020092c8) != 0) {
    } else {
        Call3(Func_02001516, 9, 0x1000000, 0x1ce0000);
        Call6(Func_020014c2, 77, 34, 1, 2, 83, 25);
        Func_02001458(3);
        Call6(Func_020014d8, 78, 34, 1, 2, 83, 25);
        Func_0200146e(3);
        Call6(Func_020014ee, 79, 34, 1, 2, 83, 25);
        v5 = 79;
        Func_02001484(30);
        Call6(Func_02001506, 67, 34, 2, 5, v5, 25);
        Func_0200149c(6);
        Call6(Func_0200151c, 69, 34, 2, 5, v5, 25);
        Func_02001594(9, 1);
        Func_020015d2(240);
        Func_020014c0(6);
        Call6(Func_02001540, 71, 34, 2, 5, v5, 25);
        Func_020014d6(6);
        Call6(Func_02001556, 73, 34, 2, 5, v5, 25);
        Call6(Func_02001568, 75, 38, 2, 1, v5, 29);
        Func_020014fe(4);
        Call6(Func_0200157e, 77, 38, 2, 1, v5, 29);
        Func_02001514(6);
        Call6(Func_02001594_a, 79, 38, 2, 1, v5, 29);
        Func_0200152a(8);
        Call6(Func_020015aa, 65, 53, 2, 1, v5, 29);
        Call6(Func_020015be, 65, 40, 2, 4, 15, 28);
        goto L_02000626;
    }
    Call3(Func_02001636, 9, 0x1000000, 0x1e00000);
    Call6(Func_020015e2, 78, 34, 1, 2, 83, 25);
    Func_02001578(3);
    Call6(Func_020015f8, 77, 34, 1, 2, 83, 25);
    Func_0200158e(3);
    Call6(Func_0200160e, 76, 34, 1, 2, 83, 25);
    Func_020015a4(30);
    Call6(Func_02001628, 65, 45, 2, 4, 15, 28);
    Call6(Func_0200163a, 71, 50, 2, 5, 79, 25);
    Func_020016b2(9, 2);
    Func_020016f0(230);
    Func_020015de(6);
    Call6(Func_0200165e, 69, 50, 2, 5, 79, 25);
    Func_020015f4(6);
    Call6(Func_02001674, 67, 50, 2, 5, 79, 25);
    Func_0200160a(6);
    Call6(Func_0200168a, 65, 50, 2, 5, 79, 25);
    Func_02001620(30);
    L_02000626:;
    if (*(s16 *)(*(s32 *)0x020092c8) == 0) {
        Call7(Func_020006de, 9, 19, 16, 5, *(s16 *)(*(s32 *)0x020092c8), 9, 30);
        Call7(Func_020006f2, 9, 51, 16, 5, 1, 9, 30);
        Call7(Func_02000706, 41, 51, 16, 5, 2, 9, 30);
    } else {
        Call7(Func_02000720, 9, 19, 16, 5, 0, 9, 30);
        Call7(Func_02000734, 9, 83, 16, 5, 1, 9, 30);
        Call7(Func_02000748, 41, 83, 16, 5, 2, 9, 30);
    }
    base5_200a0dc = 0x200a0dc;
    *(s32 *)base5_200a0dc = 0;
    Call2(Func_020016c4, 0x20083c1, 0xc80);
    Func_020016c2(1);
    Func_020016f4(1, 0, 0x200836d);
    Func_020017ea(231);
    *(s32 *)base5_200a0dc = 0;
    do {
        Func_020016da(1);
        v3 = (*(s32 *)base5_200a0dc + 1);
        *(s32 *)base5_200a0dc += 1;
    } while (v3 <= 100);
    Call1(Func_02001802, 0x121);
    if (*(s16 *)(*(s32 *)0x020092c8) == 0) {
        Call7(Func_020007a8, 9, 19, 16, 5, *(s16 *)(*(s32 *)0x020092c8), 9, 19);
        Call7(Func_020007bc, 9, 51, 16, 5, 1, 9, 19);
        Call7(Func_020007d0, 41, 51, 16, 5, 2, 9, 19);
    } else {
        Call7(Func_02000802, 9, 19, 16, 5, 0, 9, 19);
        Call7(Func_02000816, 9, 83, 16, 5, 1, 9, 19);
        Call7(Func_0200082a, 41, 83, 16, 5, 2, 9, 19);
    }
    Func_02001794(1);
    Func_020017c6(1, 0, 0);
    Func_020017a4(1);
    Call1(Func_020017ba, 0x20083c1);
    *(u16 *)(*(s32 *)0x020092c8) = (*(u16 *)(*(s32 *)0x020092c8) ^ (s32)Data_00000001);
    Func_02000952();
    Func_0200180e();
    Func_0200186a();
}

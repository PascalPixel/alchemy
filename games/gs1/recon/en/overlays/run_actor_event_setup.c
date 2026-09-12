#include "types.h"

#define FieldScene_RunActorEventSetup Func_020011b0

/* Complete 744-byte owner. The loader relocates the call at resource
 * 0x0200124c to 0x020081ec; its raw disassembly label is not an inner entry.
 * The two scene selectors below are numeric values, not address symbols. */
extern s16 Data_02000240[];
void Func_0200143a();
void Func_02001a70();
void Func_02001f80();
void Func_02002344();
void Func_020023e6();
void Func_020026b4();
s32 Func_020026d6();
s32 Func_02002714();
void Func_02002716();
void Func_02002724();
void Func_02002730();
void Func_02002738();
void Func_02002740();
s32 Func_0200274c();
s32 Func_02002762();
u8 *Func_02002766();
void Func_02002768();
u8 *Func_02002772();
s32 Func_02002792();
void Func_0200279c();
s32 Func_0200279c_1();
s32 Func_020027a6();
s32 Func_020027ba();
s32 Func_020027c4();
void Func_020027ca();
void Func_020027d0();
void Func_020027e0();
void Func_020027f0();
void Func_020027f6();
void Func_020027f6_1();
void Func_020027f6_2();
s32 Func_020027fe();
void Func_02002812();
void Func_0200281a();
void Func_02002834();
void Func_02002834_1();
s32 Func_02002852();
s32 Func_0200285c();
void Func_02002862();
void Func_02002878();
s32 Func_020028ce();
void Func_020028ea();
void Func_020028f4();
u8 *Func_020028fc();
void Func_02002902();
void Func_02002906();
s32 Func_02002908();
void Func_02002910();
s32 Func_02002912();
s32 Func_0200291c();
void Func_0200291e();
u8 *Func_02002926();
void Func_02002930();
void Func_02002960();
void Func_02002982();
void Func_0200298e();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

s32 FieldScene_RunActorEventSetup(void)
{
    u8 *record;

    *(s32 *)(*(s32 *)0x03001ebc + 0x1c0) = 0x100;
    if (Data_02000240[224] == 0x58) {
        Func_020027d0(169);
        Func_02002730(11, 5);
        Func_02002738(12, 5);
        Func_02002740(14, 2);
        Call6(Func_020026b4, 21, 9, 1, 1, 21, 73);
        Func_02001a70();
        if (Value1(Func_020026d6, 0x8b2) != 0) {
            Call3(Func_02002768, 13, 0x880000, 0x1000000);
            Func_020027ca(13, 0, 0);
        }
        if (Data_02000240[225] == 2) {
            Call1(Func_02002716, 0x12f);
            goto L_02001456;
        }
        if (Data_02000240[225] != 3) {
            goto L_02001456;
        }
        if (Value1(Func_02002714, 0x109) != 0) {
            goto L_02001456;
        }
        Func_0200143a();
    } else {
        if (Data_02000240[224] != 0x4a) {
        } else {
            record = Func_02002766(14);
            Func_02002724((s32)record, 0);
            record = Func_02002772(14);
            record[35] |= 2;
            if (Value1(Func_0200274c, 0x200) != 0) {
                Func_020027e0(14, 5);
                Func_02002344();
            }
            if (Value1(Func_02002762, 0x201) != 0) {
                Func_020027f6(15, 4);
                Func_020023e6();
            }
            if ((u16)(Data_02000240[225] - 4) <= 1) {
                Call1(Func_0200279c, 0x12f);
            }
            if (Value1(Func_02002792, 0x89a) == 0) {
                if (Value1(Func_0200279c_1, 0x895) == 0) {
                    if (Value1(Func_020027a6, 0x8b2) == 0) {
                        Func_02002834(10, 0, 0);
                    }
                }
            }
            if (Value1(Func_020027ba, 0x8b2) == 0) {
                if (Value1(Func_020027c4, 0x895) != 0) {
                    if (Data_02000240[225] == 2) {
                        Func_02002862(11, 0, 0);
                        Call1(Func_020027f0, 0x8b2);
                        Call1(Func_020027f6_1, 0x8b3);
                        Func_02002878(10, 0, 0);
                    }
                }
            }
            if (Value1(Func_020027fe, 0x8b2) != 0) {
                Call6(Func_020027f6_2, 54, 21, 53, 21, 1, 2);
                Call6(Func_02002812, 18, 20, 1, 3, 17, 21);
                Call6(Func_0200281a, 44, 18, 43, 17, 1, 1);
                Call6(Func_02002834_1, 8, 17, 1, 1, 7, 17);
            }
            if (Value1(Func_02002852, 0x895) != 0) {
                if (Value1(Func_0200285c, 0x8b2) == 0) {
                    Func_020028ea(12, 0, 0);
                    Func_020028f4(13, 0, 0);
                    Call3(Func_02002902, 8, 0xc00000, 0x1080000);
                    Call3(Func_02002910, 9, 0xa40000, 0x1180000);
                    Call3(Func_0200291e, 10, 0xb80000, 0x1300000);
                    Call3(Func_02002982, 8, 0x5000, 0);
                    Call3(Func_0200298e, 10, 0xb000, 0);
                    Call2(Func_02002906, 9, 0x2009730);
                    record = Func_020028fc(9);
                    *(s32 *)((s32)record + 24) = -0x10000;
                }
            }
            if (Value1(Func_020028ce, 0x8b2) == 0) {
                Call3(Func_02002960, 9, 0xa40000, 0x1180000);
                Call2(Func_02002930, 9, 0x2009730);
                record = Func_02002926(9);
                *(s32 *)((s32)record + 24) = -0x10000;
            }
            if (Data_02000240[225] == 5) {
                if (Value1(Func_02002908, 0x8b1) == 0) {
                    if (Value1(Func_02002912, 0x109) == 0) {
                        if (Value1(Func_0200291c, 0x8b2) == 0) {
                            Func_02001f80();
                        }
                    }
                }
            }
        }
    }
    L_02001456:;
    return 0;
}

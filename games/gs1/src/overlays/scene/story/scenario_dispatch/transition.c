#include "types.h"

#define Scene_RunTransitionCue Func_0200161c
#define Scene_RunActorFormation Func_020017c0
#define Scene_UpdateCueTimer Func_02001ac8
#define FieldScene_RunScriptedStep953 Func_02001aac

void Func_020031cc();
void Func_020031e0();
s32 Func_0200320e();
void Func_0200322a();
void Func_02003232();
void Func_0200324c();
void Func_0200325e();
void Func_02003266();
void Func_0200326a();
void Func_0200326c();
void Func_02003284();
void Func_0200328a();
void Func_020032ac();
void Func_020032d6();
void Func_020032ea();
void Func_020032ee();
void Func_020032fa();
void Func_0200331e();
void Func_02003322();
s32 Func_0200332e();
void Func_02003334();
void Func_0200333c();
void Func_02003342();
void Func_02003346();
void Func_02003348();
void Func_02003350();
void Func_02003354();
void Func_02003360();
void Func_02003360_a();
void Func_02003362();
void Func_0200336a();
void Func_0200336c();
void Func_0200336e();
void Func_02003376();
void Func_02003376_a();
void Func_02003380();
void Func_02003382();
void Func_0200338e();
void Func_020033a0();
void Func_020033a2();
void Func_020033a6();
s32 Func_0200333c_a();
void Func_0200334e();
void Func_02003360_b();
void Func_02003372();
void Func_02003384();
void Func_02003396();
void Func_020033b0();
s32 Func_020033c4();
void Func_020033e0();
s32 Func_020033f4();
void Func_02003410();
s32 Func_02003420();
void Func_02003442();
s32 Func_02003452();
void Func_0200345a();
void Func_02003472();
s32 Func_02003482();
void Func_0200348a();
void Func_020034a6();
s32 Func_020034b6();
void Func_020034ba();
void Func_020034d6();
s32 Func_020034e6();
void Func_020034ec();
void Func_02003508();
s32 Func_02003518();
void Func_0200351c();
void Func_02003538();
s32 Func_02003548();
void Func_02003550();
void Func_0200356c();
s32 Func_0200357c();
void Func_02003580();
void Func_0200359c();
s32 Func_020035ac();
void Func_020035b2();
void Func_020035ce();
s32 Func_020035de();
void Func_020035e2();
void Func_02003616();
void Func_02003646();
void Func_02003678();
void Func_0200365c(void);
void Func_0200363c(s32, s32);
void Func_02003670(void);
s32 Func_02003620();
void Func_02003662();
void Func_0200368a();
void Func_020037ca();

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

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_020017c0(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    return f(a0, a1, a2, a3, a4, a5);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

void Scene_RunTransitionCue(void)
{
    u32 i;
    s32 record;

    Func_020031cc();
    *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x204;
    Func_020032d6();
    Func_020032ea();
    Func_020031e0(20);
    record = Value1(Func_0200320e, 0);
    if (record != 0) {
        Func_0200324c(8, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Call3(Func_02003232, 8, 0x10000, 0x8000);
    Func_0200326a(8, 2);
    Call3(Func_0200325e, 8, 24, -10);
    Func_0200326c(8);
    Func_02003284(8, 1);
    Func_0200322a(6);
    Call3(Func_020032ee, 8, 0xb000, 0);
    Call3(Func_020032fa, 0, 0xc000, 40);
    Call2(Func_02003322, 0x26666, 0x4ccc);
    Call4(Func_0200333c, 0x6880000, -1, 0x20c0000, 1);
    Func_02003348();
    Func_02003266(20);
    Call2(Func_02003346, 0x19999, 0x3333);
    Call4(Func_02003360, 0x7580000, -1, 0x20c0000, 1);
    Func_0200336c();
    Func_0200328a(20);
    Call2(Func_0200336a, 0x33333, 0x6666);
    Call4(Func_02003382, 0x6e90000, -1, 0x2240000, 1);
    Func_0200338e();
    Func_020032ac(20);
    Func_02003334(8, 2);
    Func_02003376(8, 0, 30);
    Call1(Func_02003354, 0x103a);
    Call3(Func_02003376_a, 0x4008, 0, 10);
    Call3(Func_020033a2, 8, 0x100, 40);
    Func_02003362(8, 1);
    Call3(Func_020033a6, 8, 0x5000, 20);
    Call3(Func_020033a0, 0x4008, 0, 10);
    Func_02003360_a(8, 2);
    record = Value1(Func_0200332e, 0);
    if (record != 0) {
        Func_02003350(8, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200336e(8);
    Func_02003380(8, 0, 0);
    Call1(Func_0200331e, 0x825);
    Func_02003342();
}

void Scene_RunActorFormation(s32 a0)
{
    u32 i;
    s32 record;

    Value6(Func_0200333c_a, 122, 20, 1, 1, 100, 32);
    Call6(Func_0200334e, 122, 20, 1, 1, 104, 32);
    Call6(Func_02003360_b, 122, 20, 1, 1, 108, 32);
    Call6(Func_02003372, 122, 20, 1, 1, 112, 32);
    Call6(Func_02003384, 122, 20, 1, 1, 116, 32);
    Call6(Func_02003396, 122, 20, 1, 1, 120, 32);
    if (Value1_020017c0(Func_020033c4, 0x311) != 0) {
        Call6(Func_020033b0, 121, 20, 1, 1, 100, 32);
        if (a0 == 0) {
            goto L_02001890;
        }
        Call3(Func_0200345a, 9, 0x6380000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020033f4, 0x310) != 0) {
            Call6(Func_020033e0, 121, 20, 1, 1, 100, 32);
            if (a0 != 0) {
                Call3(Func_0200348a, 9, 0x6580000, 0x2080000);
            }
        }
    }
    L_02001890:;
    if (Value1_020017c0(Func_02003420, 0x313) != 0) {
        Call6(Func_02003410, 121, 20, 1, 1, 104, 32);
        if (a0 == 0) {
            goto L_020018f2;
        }
        Call3(Func_020034ba, 10, 0x6780000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_02003452, 0x312) != 0) {
            Call6(Func_02003442, 121, 20, 1, 1, 104, 32);
            if (a0 != 0) {
                Call3(Func_020034ec, 10, 0x6980000, 0x2080000);
            }
        }
    }
    L_020018f2:;
    if (Value1_020017c0(Func_02003482, 0x315) != 0) {
        Call6(Func_02003472, 121, 20, 1, 1, 108, 32);
        if (a0 == 0) {
            goto L_02001956;
        }
        Call3(Func_0200351c, 11, 0x6b80000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020034b6, 0x314) != 0) {
            Call6(Func_020034a6, 121, 20, 1, 1, 108, 32);
            if (a0 != 0) {
                Call3(Func_02003550, 11, 0x6d80000, 0x2080000);
            }
        }
    }
    L_02001956:;
    if (Value1_020017c0(Func_020034e6, 0x317) != 0) {
        Call6(Func_020034d6, 121, 20, 1, 1, 112, 32);
        if (a0 == 0) {
            goto L_020019b8;
        }
        Call3(Func_02003580, 12, 0x6f80000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_02003518, 0x316) != 0) {
            Call6(Func_02003508, 121, 20, 1, 1, 112, 32);
            if (a0 != 0) {
                Call3(Func_020035b2, 12, 0x7180000, 0x2080000);
            }
        }
    }
    L_020019b8:;
    if (Value1_020017c0(Func_02003548, 0x319) != 0) {
        Call6(Func_02003538, 121, 20, 1, 1, 116, 32);
        if (a0 == 0) {
            goto L_02001a1c;
        }
        Call3(Func_020035e2, 13, 0x7380000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_0200357c, 0x318) != 0) {
            Call6(Func_0200356c, 121, 20, 1, 1, 116, 32);
            if (a0 != 0) {
                Call3(Func_02003616, 13, 0x7580000, 0x2080000);
            }
        }
    }
    L_02001a1c:;
    if (Value1_020017c0(Func_020035ac, 0x31b) != 0) {
        Call6(Func_0200359c, 121, 20, 1, 1, 120, 32);
        if (a0 == 0) {
            goto L_02001a7e;
        }
        Call3(Func_02003646, 14, 0x7780000, 0x2080000);
    } else {
        if (Value1_020017c0(Func_020035de, 0x31a) != 0) {
            Call6(Func_020035ce, 121, 20, 1, 1, 120, 32);
            if (a0 != 0) {
                Call3(Func_02003678, 14, 0x7980000, 0x2080000);
            }
        }
    }
    L_02001a7e:;
}

void FieldScene_RunScriptedStep953(void)
{
    Func_0200365c();
    Func_0200363c(0x953, 1);
    Func_02003670();
}

void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2)
{
    u32 i;
    s32 record;
    s32 value;
    s32 base5_200a69c;
    s32 v3;

    base5_200a69c = 0x200a69c;
    if (*(s32 *)base5_200a69c != 0) {
        v3 = (*(s32 *)base5_200a69c - 1);
        *(s32 *)base5_200a69c = (*(s32 *)base5_200a69c - 1);
        if (v3 != 40) {
            goto L_02001b14;
        }
        Call3(Func_02003662, -1, -1, 0xe666);
    } else {
        value = Value0(Func_02003620);
        if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
            Func_020037ca(138);
            Call3(Func_0200368a, 0x10000, 0x20000, 0x10000);
            *(s32 *)base5_200a69c = 80;
        }
    }
    L_02001b14:;
}

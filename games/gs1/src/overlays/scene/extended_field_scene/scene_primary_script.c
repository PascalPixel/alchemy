#include "types.h"

#define NULL ((void *)0)
#define Audio_PlayCue_1(a0) Call1(Func_02002fa2, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Call3(Func_02002e66, a0, a1, a2)
#define Audio_PlayCue_1_02000780(a0) Call1(Func_02002fd4, a0)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_02002ec8, a0, a1, a2)
#define ObjectMotion_SetActionVariant_1(a0, a1) Call2(Func_02002f60, a0, a1)
#define ObjectMotion_SetPositionAndReset_2(a0, a1, a2) Call3(Func_02002ede, a0, a1, a2)
#define Audio_PlayCue_1_020007ec(a0) Call1(Func_0200303e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020007ec(a0, a1, a2) Call3(Func_02002f02, a0, a1, a2)
#define FieldScene_RunSequence111F Func_0200010c
#define FieldScene_RunOpeningAuxiliarySequence Func_02000248
#define SceneDialogue_RunFlagGatedMessageStep Func_020003b4
#define FieldScene_RunActorTwentySixEffectPresentation Func_02000444
#define FieldScene_RunMiddleAuxiliarySequence Func_020004d4
#define FieldScene_RunSupplementalSequenceOne Func_02000750
#define FieldScene_RunSupplementalSequenceTwo Func_02000780
#define FieldScene_RunSupplementalSequenceThree Func_020007ec
#define Audio_PlayCue_1_0200081c(a0) Call1(Func_0200306e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200081c(a0, a1, a2) Call3(Func_02002f34, a0, a1, a2)
#define Audio_PlayCue_1_0200084c(a0) Call1(Func_0200309e, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200084c(a0, a1, a2) Call3(Func_02002f62, a0, a1, a2)
#define Audio_PlayCue_1_0200087c(a0) Call1(Func_020030ce, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_0200087c(a0, a1, a2) Call3(Func_02002f92, a0, a1, a2)
#define Audio_PlayCue_1_020008ac(a0) Call1(Func_020030fe, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020008ac(a0, a1, a2) Call3(Func_02002fc4, a0, a1, a2)
#define FieldScene_RunSupplementalSequenceFour Func_0200081c
#define FieldScene_RunSupplementalSequenceFive Func_0200084c
#define FieldScene_RunSupplementalSequenceSix Func_0200087c
#define FieldScene_RunSupplementalSequenceSeven Func_020008ac
#define FieldScene_RunStep7BThen8 Func_020008dc
#define FieldScene_RunStep15 Func_02002580
#define FieldScene_RunStep17 Func_02002590
#define FieldScene_RunStep20 Func_020025a0

extern u8 Data_00001197[];
extern u8 Data_03001ebc[];
extern u8 Value_0200beb4;

void Func_020027ac(void);
void Func_0200277c(s32, s32);
void Func_0200296a(s32);
void Func_02002962(s32, s32);
void Func_020027c0(s32);
void Func_02002798(s32, s32);
void Func_020027a4(void);
void Func_020027c4(s32);
void Func_020027e8(void);
void Func_020025fa();
void Func_02002858();
s32 Func_020028c6();
s32 Func_020028d8();
void Func_020028dc();
void Func_020028e8();
s32 Func_020028f2();
void Func_020028f6();
s32 Func_0200290c();
s32 Func_0200290c_a();
void Func_02002950();
void Func_0200295a();
s32 Func_0200295e();
void Func_02002976();
void Func_02002984();
void Func_020029c2();
void Func_020029c2_a();
void Func_020029e2();
s32 Func_02002a06();
void Func_02002a0a();
void Func_02002a10();
void Func_02002a18();
void Func_02002a1e();
void Func_02002a20();
void Func_02002a2c();
void Func_02002a2e();
void Func_02002a30();
void Func_02002a60();
void Func_02002ac4();
void Func_02002ae0();
void Func_02002af8();
void Func_02002b6a();
void Func_02002ba4();
void Func_02002a54(void);
s32 Func_02002a32(s32);
void Func_02002b2c(s32);
s32 Func_02002b3c(s32, s32);
s32 Func_02002a94(s32, s32);
void Func_02002b58(s32, s32);
s32 Func_02002a74(s32);
void Func_02002b6e(s32);
void Func_02002b96(s32, s32);
void Func_02002b7e(s32);
void Func_02002ba6(s32, s32);
void Func_02002aca(void);
void Func_02002ae4(void);
void Func_02002b6c(s32, s32);
void Func_02002b9e(s32, s32, s32);
void Func_02002ba8(s32, s32, s32);
void Func_02002bce(s32);
void Func_0200273a(s32, s32);
void Func_02002c26(s32, s32);
void Func_02002c3e(s32, s32, s32, s32);
void Func_02002b24(s32);
void Func_02002bd4(s32, s32);
void Func_02002b32(s32);
void Func_02002bec(s32, s32, s32);
void Func_02002778(s32, s32);
void Func_02002b8c(s32, s32);
void Func_02002b60(void);
void Func_020027c8();
s32 Func_020027ec();
void Func_02002824();
void Func_02002a84();
void Func_02002b20();
void Func_02002b2e();
s32 Func_02002b36();
s32 Func_02002b40();
void Func_02002b6c_a();
void Func_02002b78();
void Func_02002b86();
void Func_02002b8a();
void Func_02002b90();
void Func_02002bb0();
s32 Func_02002bec_b();
s32 Func_02002bec_a();
s32 Func_02002c22();
void Func_02002c3e_a();
void Func_02002c4a();
void Func_02002c5c();
void Func_02002c74();
void Func_02002cba();
void Func_02002ce4();
void Func_02002cfc();
void Func_02002d04();
void Func_02002d10();
void Func_02002d40();
void Func_02002dec();
void Func_02000e84();
void Func_02002d9c();
void Func_02002e66();
void Func_02002fa2();
void Func_02000ef4();
void Func_02002d8c();
void Func_02002da2();
void Func_02002dde();
void Func_02002df4();
void Func_02002ec8();
void Func_02002ede();
void Func_02002f60();
void Func_02002fd4();
void Func_02000f20();
void Func_02002e38();
void Func_02002f02();
void Func_0200303e();
void Func_02000f52();
void Func_02002e68();
void Func_02002f34();
void Func_0200306e();
void Func_02000f80();
void Func_02002e98();
void Func_02002f62();
void Func_0200309e();
void Func_02000fb0();
void Func_02002ec8_a();
void Func_02002f92();
void Func_020030ce();
void Func_02000fe2();
void Func_02002ef8();
void Func_02002fc4();
void Func_020030fe();
void Func_0200312e(s32);
void Func_02000ffc(s32);
void Func_02004c4a(s32);
void Func_020048be(void);
void Func_02004c5a(s32);
void Func_020048ce(void);
void Func_02004c6a(s32);
void Func_0200491a(void);

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02000750(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */
static __inline__ void Call1_02000780(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_020007ec(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block. */
static __inline__ s32 Value2_0200081c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call1_0200081c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_0200084c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_0200087c(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value2_020008ac(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

void FieldScene_RunSequence111F(void)
{
    Func_020027ac();
    Func_0200277c(0x111f, 1);
    Func_0200296a(126);
    Func_02002962(0x3e7, 0);
    Func_020027c0(10);
    Func_02002798(0x974, 1);
    Func_020027a4();
    Func_020027c4(322);
    Func_020027e8();
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    u32 i;
    s32 base5_1197;
    s32 base7_0;
    u8 *p6;

    Func_020028e8();
    if (Value1(Func_020028c6, 0x815) != 0) {
        base5_1197 = (s32)Data_00001197;
        Func_020029c2(base5_1197);
        if (Value1(Func_020028d8, 2) != 0) {
            bump_step(1);
        }
        if (Value1(Func_020028f2, 3) != 0) {
            bump_step(1);
        }
        Value2(Func_02002a06, 17, 0);
        if (Value2(Func_0200295e, 0, 0) == 0) {
            Func_02002a10((base5_1197 + 3));
        } else {
            Func_02002a18((base5_1197 + 4));
        }
        Func_02002a30(17, 0);
    } else {
        p6 = *(volatile s32 *)(*(volatile s32 *)0x03001e70);
        Call1(Func_02002a2e, 0xf48);
        Func_02002a20(17, 0, 0);
        Func_02002a60(17, 0);
        Func_02002976(20);
        Func_02002a1e(17, 2);
        Func_02002984(15);
        Func_02002858();
        base7_0 = 0;
        for (i = 0; i < 40; i++) {
            Func_020025fa(((s32 (*)())Func_020029c2_a)(17));
            Func_020028f6(1);
        }
        Value2(Func_0200290c, 0x200a591, 0xc80);
        Func_02002b6a(107);
        for (i = 0; i != 180; i++) {
            if (Value2(Func_0200290c_a, i, 10) == 0) {
                if ((1 & base7_0) != 0) {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 - 0x10000;
                } else {
                    *(volatile s32 *)p6 = *(volatile s32 *)p6 + 0x10000;
                }
                base7_0 = (base7_0 + 1);
            }
            Func_020029e2(1);
        }
        Call1(Func_02002ba4, 0x121);
        Call1(Func_0200295a, 0x200a591);
        Func_02002950(1);
        Func_020028dc();
        Func_02002ac4(17, 0);
        Func_02002a0a(40);
        Call1(Func_02002ae0, 0xf4b);
        Func_02002af8(17, 0);
    }
    Func_02002a2c();
}

void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Func_02002a54();
    if (Func_02002a32(0x87a) != 0) {
        Func_02002b2c(0x1be8);
        Func_02002b3c(15, 0);
        if (Func_02002a94(0, 0) == 1) {
            Func_02002b58(15, 0);
        } else {
            u8 *p = *(u8 **)0x03001ebc;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Func_02002b96(15, 0);
        }
    } else if (Func_02002a74(0x815) != 0) {
        Func_02002b6e(0x1191);
        Func_02002b96(11, 0);
    } else {
        Func_02002b7e(0xea8);
        Func_02002ba6(11, 0);
    }
    Func_02002aca();
}

void FieldScene_RunActorTwentySixEffectPresentation(void)
{
    Func_02002ae4();
    Func_02002b6c(26, 1);
    Func_02002b9e(26, 0, 20);
    Func_02002ba8(26, 21, 40);
    Func_02002bce(0x11c7);
    Func_0200273a(26, 20);
    Func_02002c26(0x19999, 0x3333);
    Func_02002c3e(0x1510000, -1, 0x1100000, 1);
    Func_02002b24(20);
    Func_02002bd4(26, 2);
    Func_02002b32(20);
    Func_02002bec(26, 0, 10);
    Func_02002778(26, 40);
    Func_02002b8c(26, 2);
    Func_02002b60();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Func_02002b78();
    Call3(Func_02002bec_b, 0, 82, 0x2f8);
    Func_02002c3e_a(15, 0, 30);
    Call1(Func_02002c5c, 0xeae);
    Func_020027c8(15, 20);
    Value3(Func_020027ec, 15, 0xa000, 20);
    Call2(Func_02002cba, 15, 0x102);
    Func_02002bb0(20);
    Func_02002a84();
    for (i = 0; i < 40; i++) {
        Func_02002824(Func_02002bec_a(15));
        Func_02002b20(1);
    }
    Value2(Func_02002b36, 0x200a581, 0xc80);
    Value2(Func_02002b40, 0x200a5a1, 0xc80);
    Call3(Func_02002ce4, 0, 0xa000, 10);
    rec8 = Value1(Func_02002c22, 20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(volatile s32 *)((s32)rec8 + 12) += 0x1800;
        Func_02002b6c_a(1);
    }
    rec8[85] = p8;
    Call1(Func_02002b8a, 0x200a581);
    Call1(Func_02002b90, 0x200a5a1);
    Func_02002b86(1);
    Func_02002dec(161);
    Func_02002cfc(15, 0);
    Func_02002d04(20, 0);
    Func_02002c4a(40);
    Func_02002b2e();
    Func_02002d10(0, 15, 30);
    Func_02002d40(15, 0);
    Func_02002c74();
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    Audio_PlayCue_1(158);
    Call3(Func_02002d9c, (s32)&Value_0200beb4, 44, 7);
    ObjectMotion_ResetAndSetPositionInMode2_1(0, 248, 0x117);
    Call1_02000750(Func_02000e84, 1);
}

/* Sets step 188, then runs a pair of 6-argument setup calls for indices 0
 * and 2 sharing the same trailing four values, followed by a pair of
 * 3-argument calls sharing the same leading two arguments, and a closing
 * 1-argument call. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    Audio_PlayCue_1_02000780(188);
    Call6(Func_02002dde, 0, 63, 51, 8, 2, 2);
    Call1_02000780(Func_02002d8c, 10);
    Call6(Func_02002df4, 2, 63, 51, 8, 2, 2);
    Call1_02000780(Func_02002da2, 10);
    ObjectMotion_SetPositionAndReset_1(0, 352, 306);
    ObjectMotion_SetActionVariant_1(0, 3);
    ObjectMotion_SetPositionAndReset_2(0, 352, 296);
    Call1_02000780(Func_02000ef4, 2);
}

/* Runs four fixed scene-helper calls in sequence, one of them passed the
 * address of Value_0200beb4 as its first argument. */
void FieldScene_RunSupplementalSequenceThree(void)
{
    Audio_PlayCue_1_020007ec(158);
    Call3(Func_02002e38, (s32)&Value_0200beb4, 43, 15); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_020007ec(0, 230, 0x197);
    Call1_020007ec(Func_02000f20, 3);
}

/* Runs four scene calls in sequence: a single-argument call, a call that
 * passes the address of Value_0200beb4 with two more values, a call that
 * passes 0, 374, and 0x1a3, and a final single-argument call. */
void FieldScene_RunSupplementalSequenceFour(void)
{
    Audio_PlayCue_1_0200081c(158);
    Call3(Func_02002e68, (s32)&Value_0200beb4, 52, 18); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_0200081c(0, 374, 0x1a3); /* object_id 0, x 374, z 0x1a3 */
    Call1_0200081c(Func_02000f52, 4);
}

/* Runs a fixed sequence of four scripted calls: one keyed off Value_0200beb4
 * with two small numeric arguments, one with a 0x222 argument, and two plain
 * single-argument calls. */
void FieldScene_RunSupplementalSequenceFive(void)
{
    Audio_PlayCue_1_0200084c(158);
    Call3(Func_02002e98, (s32)&Value_0200beb4, 41, 32); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_0200084c(0, 200, 0x222);
    Call1_0200081c(Func_02000f80, 5);
}

/* Runs four scripted calls with fixed literal arguments: a single-argument
 * call, a 3-argument call whose first argument is the address of
 * Value_0200beb4, another 3-argument call, and a closing single-argument
 * call. */
void FieldScene_RunSupplementalSequenceSix(void)
{
    Audio_PlayCue_1_0200087c(158);
    Call3(Func_02002ec8_a, (s32)&Value_0200beb4, 35, 36); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_0200087c(0, 102, 0x263); /* object_id 0, x 102, z 611 */
    Call1_0200081c(Func_02000fb0, 6);
}

/* Runs four scripted scene calls in sequence, passing a byte's address and a
 * handful of small immediate constants to each. */
void FieldScene_RunSupplementalSequenceSeven(void)
{
    Audio_PlayCue_1_020008ac(158);
    Call3(Func_02002ef8, (s32)&Value_0200beb4, 51, 39); /* main:08009178 */
    ObjectMotion_ResetAndSetPositionInMode2_1_020008ac(0, 358, 0x29e);
    Call1_0200081c(Func_02000fe2, 7);
}

void FieldScene_RunStep7BThen8(void)
{
    Func_0200312e(123);
    Func_02000ffc(8);
}

void FieldScene_RunStep15(void)
{
    Func_02004c4a(15);
    Func_020048be();
}

void FieldScene_RunStep17(void)
{
    Func_02004c5a(17);
    Func_020048ce();
}

void FieldScene_RunStep20(void)
{
    Func_02004c6a(20);
    Func_0200491a();
}

#include "types.h"

#define GameFlag_IsSet_1(args...) Func_02003456(args)
#define BattleRuntime_Reset_1() Call0(Func_0200347e)
#define Scene_GetRecord_1(a0) Call1(Func_0200349c, a0)
#define ObjectMotion_SetSpeedParameters_1(a0, a1, a2) Call3(Func_020034ae, a0, a1, a2)
#define ObjectMotion_SetPositionAndReset_1(a0, a1, a2) Call3(Func_020034da, a0, a1, a2)
#define BattleRuntime_ScheduleShoulderButtonModeUpdate_1() Call0(Func_020034a6)
#define TARGET_ID 9
#define GATE_CODE 2059
#define FieldScene_RunPrimarySequenceHead Func_0200101c
#define FieldScene_RunClosingSequence Func_02001624
#define FieldScene_RunScene37bSequenceA Func_0200195c
#define NULL ((void *)0)
#define FieldScene_RunFiveValueStep9 Func_020019e4
#define FieldScene_RunFiveValueStep11 Func_02001a04
#define FieldScene_RunFiveValueStep15 Func_02001a44
#define FieldScene_RunScene37b_02002244 Func_02002244
#define FieldScene_RunScene37b_020022f4 Func_020022f4

extern u8 Data_02000240[];
extern u8 Data_0200aa50[];
extern u8 Data_03001ebc[];
extern u16 Data_02000240_t[][1];

s32 Func_02003456();
void Func_0200347e();
void Func_0200349c();
void Func_020034a6();
void Func_020034ae();
void Func_020034da();
void Func_0200157c();
void Func_02001a82();
void Func_0200252c();
void Func_020034b4();
void Func_020034b8();
s32 Func_020034ee();
u8 *Func_02003506();
s32 Func_02003512();
s32 Func_0200351c();
void Func_0200354a();
void Func_02003568();
void Func_0200357a();
void Func_0200357c();
void Func_02003586();
void Func_0200359e();
void Func_020035ac();
void Func_020035b4();
void Func_020035ba();
s32 Func_020035be();
s32 Func_020035c8();
s32 Func_020035c8_a();
void Func_020035cc();
void Func_020035d6();
void Func_020035f0();
void Func_020035f8();
void Func_02003610();
void Func_02003620();
void Func_0200363c();
void Func_0200364a();
void Func_02003658();
void Func_02003662();
s32 Func_0200366c();
void Func_0200367a();
void Func_0200368e();
s32 Func_02003698();
void Func_0200369e();
void Func_020036ba();
s32 Func_020036c4();
void Func_020036ca();
void Func_020036de();
void Func_020036ee();
void Func_0200370a();
s32 Func_02003714();
void Func_0200371a();
void Func_0200371a_a();
void Func_0200372e();
void Func_02003746();
void Func_0200375a();
s32 Func_02003764();
void Func_0200376a();
void Func_0200377c();
void Func_02003796();
void Func_020037a8();
s32 Func_020037b2();
void Func_020037b8();
void Func_020037ca();
void Func_020037e6();
s32 Func_02003810();
void Func_02003834();
s32 Func_02003858();
void Func_0200386e();
s32 Func_0200387a();
void Func_0200388e();
s32 Func_02003890();
void Func_02003898();
s32 Func_0200389a();
void Func_020038aa();
s32 Func_020038ae();
void Func_020038b0();
void Func_020038b0_a();
s32 Func_020038be();
s32 Func_020038e6();
s32 Func_020038e8();
void Func_020038f8();
void Func_02003902();
s32 Func_02003910();
s32 Func_02003914();
void Func_02003920();
void Func_02003926();
void Func_020039d4();
void Func_020039e0();
s32 Func_020038be_a();
void Func_02003a78();
void Func_02003a88();
s32 Func_02003a96();
s32 Func_02003aa0();
void Func_02003dc6();
s32 Func_02003dce();
void Func_02003df6();
s32 Func_02003dfe();
void Func_02003e00();
s32 Func_02003e16();
void Func_02003e30();
void Func_02003e32();
void Func_02003e3e();
void Func_02003e50();
void Func_02003e64();
void Func_02003e7c();
void Func_02003edc();
void Func_0200353a(s32, s32, s32, s32, s32);
void Func_0200360e(void);
void Func_0200355a(s32, s32, s32, s32, s32);
void Func_0200372e_a(void);
void Func_0200359a(s32, s32, s32, s32, s32);
void Func_02003966(void);
void Func_02002372();
s32 Func_02004684();
s32 Func_0200468e();
void Func_0200469e();
void Func_0200469e_a();
void Func_020046ba();
s32 Func_020046d4();
s32 Func_02004706();
void Func_02004708();
void Func_02004712();
void Func_0200473a();
void Func_0200477e();
void Func_02004798();
void Func_020047a4();
void Func_020047b4();
void Func_020047ea();
void Func_02002424();
s32 Func_02004734();
s32 Func_0200473e();
void Func_0200474e();
void Func_0200474e_a();
void Func_0200476a();
s32 Func_02004784();
s32 Func_020047b8();
void Func_020047ba();
void Func_020047c4();
void Func_020047ec();
void Func_0200482e();
void Func_02004848();
void Func_02004854();
void Func_02004866();
void Func_0200489a();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Resolved engine calls: each pseudo symbol is the per-site call word the
 * overlay image holds (a word can serve two sites with different targets),
 * and the macro names the engine function the site reaches through the
 * overlay veneer and the main-image veneer island, keeping the site's own
 * calling form. Names without a repository binding are provisional.
 */

/* Argument shared by every configuration call below. */

/* Condition code passed to the gating check; the body below runs only when
 * that check reports 0. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call0(void (*f)())
{
    f();
}

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

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200105c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ s32 Value4(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    return f(a0, a1, a2, a3);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020014b8(void (*f)(), s32 a0)
{
    f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001624(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_0200195c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_020019e4(void (*f)(), s32 a0)
{
    f(a0);
}

/* If the code-2059 check passes, runs a short setup/configuration sequence
 * for id 9: two no-argument calls bracket a select call and two calls each
 * taking a pair of numeric arguments. */
void FieldScene_RunPrimarySequenceHead(void)
{
    if (GameFlag_IsSet_1(GATE_CODE) == 0) {
        BattleRuntime_Reset_1();
        Scene_GetRecord_1(TARGET_ID);
        ObjectMotion_SetSpeedParameters_1(TARGET_ID, 13107, 0x00001999); /* object_id, speed_limit, acceleration */
        ObjectMotion_SetPositionAndReset_1(TARGET_ID, 504, 152); /* object_id, x=504, z=152 */
        BattleRuntime_ScheduleShoulderButtonModeUpdate_1();
    }
}

s32 Func_0200105c(void)
{
    u8 *record;

    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c0)) = 0x204;
    Func_0200252c();
    Call1_0200105c(Func_020034b8, 0x144);
    record = (u8 *)Value1(Func_020034ee, 18);
    record[89] = 0;
    record += 35;
    {
        u8 value = *(volatile u8 *)record;

        *record = (u8)(value | 2);
    }
    Func_020034b4((s32)Func_02003506(18), 0);
    *(u8 *)(Func_02003512(18) + 35) &= 254;
    Func_020035b4(18, 1);
    if ((u32)((Data_02000240_t[225][0] - 3) << 16) > 0x10000) {
        Func_0200357c(5, 0, 0);
        Func_02003586(1, 0, 0);
    }
    if (Value1(Func_0200351c, 0x818) != 0) {
        Call3(Func_0200359e, 18, 0x1200000, 0xb20000);
        Call3(Func_020035ac, 17, 0x6480000, 0x6480000);
        Call3(Func_020035ba, 10, 0xe80000, 0x780000);
        Call3(Func_020035c8, 12, 0x1580000, 0x780000);
        Call3(Func_020035d6, 10, 0xe80000, 0x780000);
        Call6(Func_0200354a, 0, 59, 15, 38, 4, 3);
        Call3(Func_020035f8, 12, 0x1580000, 0x780000);
        Call6(Func_02003568, 4, 59, 17, 38, 4, 3);
        Call6(Func_0200357a, 8, 60, 17, 39, 2, 2);
        Call6(Func_0200363c, 0, 1, 2, 1, 17, 7);
    } else if (Value1(Func_020035be, 0x816) != 0
                && Value1(Func_020035c8_a, 0x817) != 0) {
        Call3(Func_0200364a, 10, 0xe80000, 0x780000);
        Call3(Func_02003658, 12, 0x1580000, 0x780000);
        Call6(Func_020035cc, 0, 28, 17, 8, 2, 1);
        Call3(Func_0200367a, 10, 0xe80000, 0x780000);
        Call6(Func_020035f0, 0, 59, 15, 38, 4, 3);
        Call3(Func_0200369e, 12, 0x1580000, 0x780000);
        Call6(Func_02003610, 4, 59, 17, 38, 4, 3);
        Call6(Func_02003620, 8, 60, 17, 39, 2, 2);
        Call6(Func_0200363c, 0, 0, 2, 1, 17, 8);
    } else {
        if (Value1(Func_0200366c, 0x816) != 0) {
            Call3(Func_020036ee, 10, 0xe80000, 0x780000);
            Call6(Func_02003662, 0, 59, 15, 38, 4, 3);
        }
        if (Value1(Func_02003698, 0x817) != 0) {
            Call3(Func_0200371a, 12, 0x1580000, 0x780000);
            Call6(Func_0200368e, 4, 59, 17, 38, 4, 3);
        }
    }
    if (Value1(Func_020036c4, 0x80b) != 0) {
        Call3(Func_02003746, 9, 0x1f80000, 0x980000);
        Call6(Func_020036ba, 2, 28, 34, 10, 2, 1);
        Call6(Func_020036ca, 2, 30, 16, 10, 2, 1);
        Call6(Func_020036de, 0, 55, 32, 40, 4, 3);
    }
    if (Value1(Func_02003714, 0x80c) != 0) {
        Call3(Func_02003796, 11, 0x2880000, 0x980000);
        Call6(Func_0200370a, 4, 28, 36, 10, 2, 1);
        Call6(Func_0200371a_a, 4, 30, 18, 10, 2, 1);
        Call6(Func_0200372e, 4, 55, 36, 40, 4, 3);
    }
    if (Value1(Func_02003764, 0x80d) != 0) {
        Call3(Func_020037e6, 13, 0x1f80000, 0xc80000);
        Call6(Func_0200375a, 2, 29, 34, 11, 2, 1);
        Call6(Func_0200376a, 2, 31, 16, 11, 2, 1);
        Call6(Func_0200377c, 0, 58, 32, 43, 4, 1);
    }
    if (Value1(Func_020037b2, 0x80e) != 0) {
        Call3(Func_02003834, 15, 0x2880000, 0xc80000);
        Call6(Func_020037a8, 4, 29, 36, 11, 2, 1);
        Call6(Func_020037b8, 4, 31, 18, 11, 2, 1);
        Call6(Func_020037ca, 4, 58, 36, 43, 4, 1);
    }
    {
    s16 *state = (s16 *)Data_02000240_t;

    if (state[225] == 3) {
        if (Value1(Func_02003810, 0x30a) != 0) {
            Func_0200388e(1, 0, 0);
            Func_02003898(5, 0, 0);
        } else if (Value1(Func_02003858, 0x109) == 0) {
            Func_0200157c();
            Call1_0200105c(Func_0200386e, 0x30a);
        }
    }
    if (state[225] == 4) {
        if (Value1(Func_0200387a, 0x30b) != 0) {
            Func_020038f8(1, 0, 0);
            Func_02003902(5, 0, 0);
        } else if (Value1(Func_0200389a, 0x109) == 0) {
            Func_02001a82();
            Call1_0200105c(Func_020038b0, 0x30b);
        }
    }
    }
    if (Value1(Func_020038ae, 0x814) != 0) {
        Func_020039e0(141);
        Call3(Func_020038b0_a, 0x10000, 0x10000, 0x10000);
        Func_020039d4();
    }
    return 0;
}

void Func_020014b8(void)
{
    u32 i;
    s32 value;
    volatile s32 *p;
    s32 buf;

    p = (volatile s32 *)Data_0200aa50;
    buf = Value2(Func_02003890, 14, 0x400);
    Call2(Func_020038aa, 0x200a56c, buf);
    value = Func_020038be_a(Func_020038be(), 128, buf);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xac00;
    }
    value = Func_020038e8(Func_020038e6(), 128, buf + 128);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xdc00;
    }
    value = Func_02003914(Func_02003910(), 128, buf + 0x100);
    for (i = 0; i < 9; i++) {
        volatile s32 *q = p;

        *q++ = 0;
        *q++ = 0x40004000;
        p += 3;
        *q = value | 0xc00;
    }
    Func_02003926(14);
    {
        s32 size = 0xc80;

        Func_02003920(0x2008eb1, size);
    }
}

void FieldScene_RunClosingSequence(void)
{
    s32 first;
    s32 kind;
    s32 second;

    first = Value1_02001624(Func_02003a96, 0);
    kind = *(volatile s32 *)(first + 8) >> 20;
    second = Value1_02001624(Func_02003aa0, 0);
    if ((*(volatile s32 *)(second + 16) >> 20) == 8) {
        if ((u32)(kind - 17) <= 1) {
            Call4(Func_02003a78, 2, 0x1100000, 0x800000, 255);
            Call4(Func_02003a88, 2, 0x1200000, 0x800000, 255);
        }
    }
}

void FieldScene_RunScene37bSequenceA(void)
{
    u32 i;
    s32 record;

    record = Value1_0200195c(Func_02003dce, 17);
    if (record != 0) {
        if ((*(volatile s32 *)(record + 16) >> 20) == 8) {
            Func_02003dc6();
            Func_02003edc(185);
            Call3(Func_02003df6, 17, 0x3333, 0x1999);
            Call3(Func_02003e00, 0, 0x3333, 0x1999);
            *(u8 *)(Func_02003dfe(17) + 90) &= 254;
            Func_02003e50(0, 8);
            record = Func_02003e16(0);
            Func_02003e32(0, *(s16 *)(record + 10), 136);
            Call3(Func_02003e3e, 17, 0x120, 120);
            Func_02003e64(17);
            Func_02003e7c(0, 1);
            Func_02003e30();
        }
    }
}

void FieldScene_RunFiveValueStep9(void)
{
    Func_0200353a(9, 31, 9, 30, 9);
    Func_0200360e();
}

void FieldScene_RunFiveValueStep11(void)
{
    Func_0200355a(11, 40, 9, 41, 9);
    Func_0200372e_a();
}

void FieldScene_RunFiveValueStep15(void)
{
    Func_0200359a(15, 40, 12, 41, 12);
    Func_02003966();
}

void FieldScene_RunScene37b_02002244(void)
{
    u32 i;
    s32 record;

    Func_0200469e();
    if (Value1(Func_02004684, 0x818) == 0) {
        if (Value1(Func_0200468e, 0x816) == 0) {
            Call2(Func_0200477e, 0x20000, 0x4000);
            Call4(Func_02004798, 0x11e0000, -1, 0x920000, 1);
            Func_020047a4();
            Func_020047ea(186);
            Call6(Func_0200469e_a, 0, 59, 15, 38, 4, 3);
            if (Value1(Func_020046d4, 0x817) != 0) {
                Call6(Func_020046ba, 8, 60, 17, 39, 2, 2);
            }
            Func_020047b4(0, 0, 0);
            Func_02004712(30);
            Call1_020019e4(Func_02004708, 0x816);
            if (Value1(Func_02004706, 0x817) != 0) {
                Func_02002372();
            }
        }
    }
    Func_0200473a();
}

void FieldScene_RunScene37b_020022f4(void)
{
    u32 i;
    s32 record;

    Func_0200474e();
    if (Value1(Func_02004734, 0x818) == 0) {
        if (Value1(Func_0200473e, 0x817) == 0) {
            Call2(Func_0200482e, 0x20000, 0x4000);
            Call4(Func_02004848, 0x11e0000, -1, 0x920000, 1);
            Func_02004854();
            Func_0200489a(186);
            Call6(Func_0200474e_a, 4, 59, 17, 38, 4, 3);
            if (Value1(Func_02004784, 0x816) != 0) {
                Call6(Func_0200476a, 8, 60, 17, 39, 2, 2);
            }
            Call3(Func_02004866, 0, 0x8000, 0);
            Func_020047c4(30);
            Call1_020019e4(Func_020047ba, 0x817);
            if (Value1(Func_020047b8, 0x816) != 0) {
                Func_02002424();
            }
        }
    }
    Func_020047ec();
}

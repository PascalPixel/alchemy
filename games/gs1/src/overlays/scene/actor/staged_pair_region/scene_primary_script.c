#include "types.h"

#define FieldScene_RunScene3a6SequenceA Func_020011a0
#define FieldScene_RunScene3a6SequenceB Func_02001258
#define FieldScene_RunScene3a6SequenceC Func_02001300
#define FieldScene_RunScene3a6_020014ac Func_020014ac
#define FieldScene_RunShiftAndSetFlag301 Func_020018b4
#define FieldScene_RunActor11Transition301 Func_020018f4
extern u8 Data_03001ebc[];
extern u8 Data_02000240[];

void Func_02002d4e();
void Func_02002d60();
void Func_02002d74();
s32 Func_02002d90();
s32 Func_02002da8();
void Func_02002dac();
void Func_02002dd0();
void Func_02002dde();
void Func_02002dfc();
s32 Func_02002e3c_a();
s32 Func_02002e3c_b();
void Func_02002e94();
s32 Func_02002e34();
void Func_02002e48();
void Func_02002e64();
void Func_02002e7a();
void Func_02002e9e();
void Func_02002ecc();
s32 Func_02002edc();
void Func_02002ede();
void Func_02002ef0_a();
void Func_02002ef0_b();
void Func_02002ef6();
void Func_02002ef8_a();
void Func_02002ef8_b();
void Func_02002ef8_c();
void Func_02002f04();
void Func_02002f0c();
void Func_02002f10_a();
void Func_02002f10_b();
void Func_02002f5a();
void Func_02002f76();
s32 Func_02002eec();
s32 Func_02002ef6_a();
void Func_02002f08();
void Func_02002f0e();
void Func_02002f2a();
void Func_02002f3a();
void Func_02002f68();
void Func_02002f78();
void Func_02002f8e();
void Func_02002f94();
void Func_02002f9c();
s32 Func_02002fa2_a();
s32 Func_02002fa2_b();
void Func_02002faa();
void Func_02002fac();
void Func_02002ff4();
s32 Func_02002f96();
s32 Func_02002fe8();
s32 Func_02002ff0();
s32 Func_02002ffa();
s32 Func_02003002();
void Func_02003030();
s32 Func_02001ac2();
void Func_02001ad2();
void Func_0200223c();
void Func_0200315c();
void Func_0200315e();
void Func_02003176();
void Func_02003186();
void Func_02003188();
void Func_020031c0_a();
void Func_020031c0_b();
s32 Func_020031c6();
void Func_020031c8();
s32 Func_020031d2();
void Func_02003200();
void Func_02003206();
s32 Func_0200320e();
void Func_02003214();
void Func_02003222();
void Func_02003224();
void Func_0200322c();
s32 Func_0200323e();
void Func_02003244();
void Func_02003246_a();
void Func_02003246_b();
void Func_02003248();
void Func_0200324c();
void Func_0200326e();
void Func_02003270();
void Func_02003298();
void Func_0200329a();
void Func_020032a0();
void Func_02003312();
void Func_02003616();
void Func_0200309c();
void Func_020030a6();
void Func_02003568();
void Func_020034f6();
void Func_0200304e();
void Func_02003640();
void Func_02003656(s32 soundId);
void Func_020030de(s32 actorId, s32 cellX, s32 cellZ);
void Func_020030ea(s32 actorId, s32 cellX, s32 cellZ);
void Func_020035b4(s32 flagId);
void Func_0200353a(s32 frames);
void Func_02003092(void);
void Func_02003684(s32 soundId);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/*
 * Camera-shift beat for overlay resource_3a6.  Each Func_ name spells the
 * overlay's own relocated call word, not a runtime address.
 */

/* Mirror the preceding transition on the negative grid axis, latch its event
 * flag, wait for the map update, and close on the paired sound cue. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_020011a0(s32 (*f)(), s32 a0)
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
static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call1_02001258(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001258(s32 (*f)(), s32 a0)
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

s32 Func_020010e0(s32 a0)
{
    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Value1(Func_02002da8, 0);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(volatile s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(volatile s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(volatile s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Func_02002d90, (s32)rec, (s32)slot) == 0) {
        Func_02002dd0();
        Func_02002d60((s32)rec, 6);
        Func_02002d4e(6);
        Func_02002e94(152);
        Func_02002d74((s32)rec, 7);
        *(volatile s32 *)(rec + 48) = 0x30000;
        *(volatile s32 *)(rec + 52) = 0x20000;
        *(volatile s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Func_02002dde((s32)rec, 0);
        Value3(Func_02002e3c_a, 0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Func_02002dac((s32)rec, 6);
        Func_02002dfc((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Func_02002e3c_b)();
        return 1;
    }
    return 0;
}

void FieldScene_RunScene3a6SequenceA(void)
{
    u32 i;
    s32 record;

    if (Value1_020011a0(Func_02002e34, 0x200) == 0) {
        Call1(Func_02002e48, 0x200);
        Func_02002e64();
        Call2(Func_02002ef8_a, 0x10000, 0x2000);
        Func_02002ef8_b(8, 1);
        Func_02002f0c();
        Func_02002e7a(60);
        Call3(Func_02002ef6, 8, 0xc000, 20);
        Call2(Func_02002f10_a, 8, 0x102);
        Func_02002ef8_c(8, 2);
        Func_02002e9e(20);
        Call3(Func_02002ecc, 8, 0x10000, 0x8000);
        Call3(Func_02002ef0_a, 8, 0x318, 248);
        Func_02002f76(152);
        record = Func_02002edc(8);
        *(volatile s32 *)(record + 40) = 0x80000;
        Call3(Func_02002f10_b, 8, 0x318, 0x118);
        Func_02002ede(20);
        Call3(Func_02002f5a, 8, 0xc000, 20);
        Func_02002ef0_b(30);
        Func_02002f04();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{
    u32 i;
    s32 record;

    if (Value1_02001258(Func_02002eec, 0x200) != 0) {
        if (Value1_02001258(Func_02002ef6_a, 0x201) == 0) {
            Call1_02001258(Func_02002f08, 0x201);
            Call1_02001258(Func_02002f0e, 0x302);
            Func_02002f2a();
            Call2(Func_02002fac, 8, 0x102);
            Func_02002f94(8, 2);
            Func_02002f3a(20);
            Call3(Func_02002f68, 8, 0x20000, 0x10000);
            Call3(Func_02002f8e, 8, 0x2f8, 0x118);
            Call3(Func_02002f9c, 8, 0x2f8, 0x138);
            Call3(Func_02002faa, 8, 0x318, 0x138);
            Func_02002f78(10);
            Call3(Func_02002ff4, 8, 0xc000, 20);
            record = Func_02002fa2_a(8);
            *(volatile s32 *)(record + 108) = 0x2008cf9;
            Call0((void (*)())Func_02002fa2_b);
        }
    }
}

void FieldScene_RunScene3a6SequenceC(void)
{
    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    if (Value1(Func_02002f96, 0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 8) {
            idx = *(s16 *)((s32)p5 + 0x16c);
            rec8 = Value1(Func_02002fe8, 8);
            record = Value1(Func_02002ff0, 0);
            *(volatile s32 *)(rec8 + 48) = *(volatile s32 *)(record + 48);
            rec8 = Value1(Func_02002ffa, 8);
            record = Value1(Func_02003002, 0);
            *(volatile s32 *)(rec8 + 52) = *(volatile s32 *)(record + 52);
            idx -= 45;
            tbl = 0x02009f00;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003030(8, *(volatile s32 *)(tbl + idx), *(volatile s32 *)(tbl + idx4));
        }
    }
}

void FieldScene_RunScene3a6_020014ac(void)
{
    u32 i;
    s32 record;
    s32 zero;

    Func_0200315e();
    Func_0200315c(10);
    Call3(Func_02003188, 0, 0x8000, 0x1999);
    Func_020031c8(0, 8);
    Func_02003176(15);
    Func_020031c0_a(0, 8, 0);
    Func_02003186(4);
    Call1(Func_02003246_a, 0x120);
    Func_0200324c(239);
    Call3(Func_020031c0_b, 9, 0x8000, 0x1999);
    Func_02003200(9, 2);
    zero = 0;
    *(u8 *)(Func_020031c6(9) + 85) = zero;
    record = Func_020031d2(9);
    *(volatile s32 *)(record + 68) = zero;
    Func_02003206(9, 12, 0);
    Func_02003214(0);
    Func_0200322c(0, 1);
    Func_02003222(9);
    Call1(Func_0200329a, 0x120);
    Func_020032a0(213);
    Func_02003248(9, 3);
    *(u8 *)(Func_0200320e(9) + 85) = 3;
    Func_02003246_b(9, 6, 0);
    Func_02003224(9);
    Func_0200223c();
    Func_02003270(9, 8);
    Func_02003298(9, 3);
    *(u8 *)(Func_0200323e(9) + 35) = 2;
    Value6(Func_02001ac2, 0, 12, 16, 1, 4, 0);
    Call6(Func_02001ad2, 0, 13, 16, 1, 4, 0);
    Call1(Func_02003244, 0x202);
    Func_02003312(240);
    Func_0200326e();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void FieldScene_RunShiftAndSetFlag301(void)
{
    Func_02003616(241);
    Func_0200309c(11, 112, 0);
    Func_020030a6(11, 80, 0);
    Func_02003568(0x301);
    Func_020034f6(2);
    Func_0200304e();
    Func_02003640(0x121);
}

void FieldScene_RunActor11Transition301(void)
{
    Func_02003656(241);
    Func_020030de(11, -112, 0);
    Func_020030ea(11, -80, 0);
    Func_020035b4(0x301);
    Func_0200353a(2);
    Func_02003092();
    Func_02003684(0x121);
}

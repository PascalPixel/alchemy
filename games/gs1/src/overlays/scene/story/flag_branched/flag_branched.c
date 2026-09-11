#include "types.h"
#include "facing_object.h"

/* overlays/scene/story/flag_branched/conditional_scene_setup.c */
#define Scene_RunStepWithValue174d Func_020003ec
#define Scene_RunStepWithValue174e Func_02000408
#define Scene_RunBranchedStep Func_02000424
#define Scene_RunStep210ByFlag84e Func_0200043c
#define Scene_RunStep211ByFlag84e Func_02000468
#define Scene_RunStep212ByFlag84e Func_02000494
#define Scene_RunStep213ByFlag84e Func_020004c0
void Func_02002860(void);
void Func_02002840(s32, s32);
void Func_02002874(void);
void Func_0200287c(void);
void Func_0200285c(s32, s32);
void Func_02002890(void);
s32 Func_02002878(void);
void Func_0200081c(void);
void Func_0200083e(void);
s32 Func_02002892(s32);
void Func_02000872(s32);
s32 Func_02000948(s32, s32, s32);
s32 Func_020028be(s32);
void Func_0200089c(s32);
s32 Func_02000970(s32, s32, s32);
s32 Func_020028ea(s32);
void Func_020008c8(s32);
s32 Func_0200099c(s32, s32, s32);
s32 Func_02002916(s32);
void Func_020008f4(s32);
s32 Func_020009c8(s32, s32, s32);

void Scene_RunStepWithValue174d(void)
{
    Func_02002860();
    Func_02002840(0x174D, 1);
    Func_02002874();
}

void Scene_RunStepWithValue174e(void)
{
    Func_0200287c();
    Func_0200285c(0x174E, 1);
    Func_02002890();
}

void Scene_RunBranchedStep(void)
{
    if (Func_02002878() != 0) {
        Func_0200081c();
    } else {
        Func_0200083e();
    }
}

void Scene_RunStep210ByFlag84e(void)
{
    if (Func_02002892(0x84E) != 0) {
        Func_02000872(0x210);
    } else {
        Func_02000948(21, 182, 0x210);
    }
}

void Scene_RunStep211ByFlag84e(void)
{
    if (Func_020028be(0x84E) != 0) {
        Func_0200089c(0x211);
    } else {
        Func_02000970(22, 183, 0x211);
    }
}

void Scene_RunStep212ByFlag84e(void)
{
    if (Func_020028ea(0x84E) != 0) {
        Func_020008c8(0x212);
    } else {
        Func_0200099c(23, 186, 0x212);
    }
}

void Scene_RunStep213ByFlag84e(void)
{
    if (Func_02002916(0x84E) != 0) {
        Func_020008f4(0x213);
    } else {
        Func_020009c8(24, 189, 0x213);
    }
}
#undef Scene_RunStepWithValue174d
#undef Scene_RunStepWithValue174e
#undef Scene_RunBranchedStep
#undef Scene_RunStep210ByFlag84e
#undef Scene_RunStep211ByFlag84e
#undef Scene_RunStep212ByFlag84e
#undef Scene_RunStep213ByFlag84e

/* overlays/scene/story/flag_branched/overlay_object.c */
#define NULL ((void *)0)
#define CalculateFacingAngle Func_02002462
#define OvObj_UpdateFacingTowardTarget Func_02000030


s32 Func_02002462(s32, s32);

s32 OvObj_UpdateFacingTowardTarget(struct FacingObject *obj)
{
    s32 delta;
    u16 old;
    s32 angle;
    struct FacingObject *target;

    target = obj->facing_target;
    if (target != NULL) {
        obj->facing_flags = (u8)(0xFE & obj->facing_flags);
        angle = (u16)CalculateFacingAngle(target->position_z - obj->position_z, target->position_x - obj->position_x);
        old = obj->facing;
        delta = (s16)(angle - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            /* The loader relocates the stored pool word to -0x1000. */
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            obj->facing = (u16)(old + delta);
        }
    }
    return 1;
}
#undef NULL
#undef CalculateFacingAngle
#undef OvObj_UpdateFacingTowardTarget

/* overlays/scene/story/flag_branched/run_scene_long_branching_choreography.c */
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00001440[];
extern u8 Data_0200a5c0[];
extern u8 Data_03001ebc[];
typedef void SceneEndFunc(void);
extern volatile SceneEndFunc Func_02002596;
void Func_02002ce4();
void Func_02002d02();
void Func_02002d34();
void Func_02002d50();
s32 Func_02002d5a();
void Func_02002d62();
s32 Func_02002dbc();
void Func_02002de0();
void Func_02002dfc();
void Func_02002e1c();
void Func_02002e20();
void Func_02002e36();
void Func_02002e42();
void Func_02002e56();
s32 Func_02002e68();
void Func_02002e70();
void Func_02002e74();
void Func_02002e7a();
void Func_02002e7e_a();
void Func_02002e7e_b();
void Func_02002e94();
void Func_02002e9c();
void Func_02002eae();
void Func_02002eba_a();
void Func_02002eba_b();
void Func_02002ec0();
void Func_02002ed2();
void Func_02002ef4();
void Func_02002efa();
void Func_02002f02();
void Func_02002f0a();
void Func_02002f0e();
void Func_02002f18_a();
void Func_02002f18_b();
void Func_02002f3e();
void Func_02002f44();
void Func_02002f4c();
void Func_02002f4e_a();
void Func_02002f4e_b();
void Func_02002f54();
void Func_02002f5a();
void Func_02002f60();
void Func_02002f66();
void Func_02002f76();
void Func_02002f88();
s32 Func_02002f8e();
void Func_02002f92();
void Func_02002f9a();
void Func_02002fa8();
s32 Func_02002fae();
void Func_02002fb2();
void Func_02002fb4();
s32 Func_02002fc2();
void Func_02002fca();
void Func_02002fcc();
s32 Func_02002fe0();
void Func_02002fe6();
void Func_02002fe8();
void Func_02002ff6();
void Func_02002ff8();
s32 Func_02002ffc();
void Func_02003000();
void Func_02003004();
void Func_0200300a();
void Func_0200300c();
void Func_02003018();
void Func_0200301c();
void Func_02003030();
void Func_02003032();
void Func_0200303c();
void Func_02003046();
void Func_0200304e();
void Func_02003080();
void Func_02003084_a();
void Func_02003084_b();
void Func_02003088();
void Func_0200308e();
void Func_02003090();
void Func_020030a2();
void Func_020030b0();
void Func_020030c8();
void Func_020030d0();
void Func_020030d8_a();
void Func_020030d8_b();
void Func_0200311c();
s32 Func_0200313e_a();
s32 Func_0200313e_b();
void Func_0200314a();
void Func_02003156();
void Func_0200315e();
void Func_02003162();
void Func_02003172();
void Func_02003178();
void Func_0200318e();
void Func_020031a2();
void Func_020031ae();
void Func_020031b0();
s32 Func_020031ce();
void Func_020031da();
void Func_020031dc();
s32 Func_020031e6();
void Func_020031ee();
void Func_020031f2();
s32 Func_020031f6();
void Func_020031fa();
void Func_02003206();
void Func_02003214();
s32 Func_02003216();
void Func_0200322e();
void Func_02003230();
void Func_0200323c();
s32 Func_02003240();
void Func_02003250();
void Func_02003258_a();
void Func_02003258_b();
void Func_02003278();
void Func_0200327a();
void Func_0200327e();
void Func_02003280();
void Func_02003292();
void Func_0200329c();
void Func_020032a2();
void Func_020032a8();
void Func_020032ae();
void Func_020032b0();
void Func_020032c2();
void Func_020032cc();
void Func_020032d4();
void Func_020032d6_a();
void Func_020032d6_b();
s32 Func_020032ea();
s32 Func_0200330a();
void Func_02003310();
s32 Func_0200331e();
s32 Func_0200333c();
void Func_02003354();
s32 Func_02003358();
void Func_0200335c();
void Func_02003366();
void Func_0200336c();
void Func_02003374();
void Func_02003378();
void Func_0200338c();
void Func_020033a2();
void Func_020033aa();
void Func_020033b0();
void Func_020033bc();
void Func_020033c6();
void Func_020033dc();
void Func_020033de();
void Func_020033e0();
void Func_020033e4();
void Func_020033ea();
void Func_020033ec();
void Func_020033fe();
void Func_0200340c();
s32 Func_0200340e();
void Func_02003424();
void Func_02003428();
void Func_0200342c();
void Func_02003434();
void Func_02003438();
s32 Func_0200348a();
s32 Func_02003496();
s32 Func_020034a4();
void Func_020034a6_a();
void Func_020034a6_b();
void Func_020034b2_a();
void Func_020034b2_b();
s32 Func_020034b6();
void Func_020034bc();
void Func_020034be();
void Func_020034c4();
void Func_020034cc();
void Func_020034d0();
void Func_020034d8();
void Func_020034dc();
void Func_020034f0();
void Func_020034fc();
void Func_02003508();
void Func_0200350c();
void Func_02003512();
void Func_02003514();
void Func_0200351c();
void Func_02003534();
void Func_0200353a();
void Func_02003546();
s32 Func_02003568();
void Func_0200358c();
s32 Func_02003592();
void Func_020035a4();
void Func_020035c6();
void Func_020035cc();
void Func_020035d0();
void Func_020035da();
void Func_020035de();
void Func_020035e4();
s32 Func_020035ec();
void Func_020035fc();
void Func_02003600();
void Func_02003604();
void Func_0200360c();
void Func_02003616();
void Func_02003618();
void Func_02003624();
void Func_0200362a();
void Func_02003630();
s32 Func_0200363a();
void Func_0200364c();
void Func_02003656();
void Func_02003666();
void Func_0200366e();
void Func_0200368a();
void Func_02003696();
void Func_020036a2();
void Func_020036ae();
void Func_020036b0();
void Func_020036c8();
void Func_020036d6_a();
void Func_020036d6_b();
void Func_020036ec();
void Func_020036f0();
void Func_02003702();
void Func_0200370c();
void Func_0200370e();
void Func_02003710();
void Func_02003722();
void Func_0200372a();
void Func_0200372c();
void Func_02003738();
s32 Func_02003748();
void Func_0200374a();
void Func_0200374c();
void Func_02003750();
void Func_02003758();
void Func_02003786();
void Func_020037aa();
s32 Func_020037b6();
void Func_020037c6();
void Func_020037e0();
void Func_020037e2();
void Func_020037ee();
void Func_020037f0();
void Func_020037f4();
void Func_020037fa();
void Func_020037fe();
void Func_02003812();
void Func_02003824();
void Func_0200382e();
void Func_02003838();
void Func_0200383c();
void Func_0200383e();
void Func_02003842();
void Func_02003844();
void Func_02003848();
void Func_0200385e();
void Func_02003860();
void Func_02003862();
void Func_0200386a();
void Func_02003878();
void Func_02003880();
void Func_02003888();
void Func_0200388e();
void Func_02003890();
void Func_02003898();
void Func_020038ac();
void Func_020038b2();
void Func_020038b8();
s32 Func_020038ba();
void Func_020038c4();
void Func_020038d0();
void Func_020038e0();
void Func_020038ee();
void Func_020038f4();
void Func_020038fa();
void Func_02003908_a();
void Func_02003908_b();
void Func_02003908_c();
void Func_0200390c();
void Func_02003916();
void Func_0200391a();
void Func_02003926();
void Func_0200392a();
void Func_0200392c();
void Func_0200393a();
void Func_0200393c();
void Func_02003940();
void Func_02003962();
void Func_0200399a();
void Func_020039a0();
void Func_020039ac();
void Func_020039c4();
void Func_020039cc();
void Func_020039ce();
void Func_020039d8();
void Func_020039da();
void Func_020039e2();
void Func_020039e4();
void Func_020039ec();
void Func_020039f2();
void Func_02003a0c();
void Func_02003a0e();
void Func_02003a1e();
void Func_02003a20();
void Func_02003a28();
void Func_02003a34();
void Func_02003a36();
void Func_02003a46();
void Func_02003a4a();
void Func_02003a58();
void Func_02003a66();
void Func_02003a68();
void Func_02003a7a();
void Func_02003a8a();
void Func_02003a8c();
void Func_02003a96();
void Func_02003a98();
void Func_02003ab8();
void Func_02003aba();
void Func_02003abc();
void Func_02003ac4();
void Func_02003ace_a();
void Func_02003ace_b();
void Func_02003ad8();
void Func_02003ada();
void Func_02003adc();
void Func_02003ae4_a();
void Func_02003ae4_b();
void Func_02003ae8();
void Func_02003aea();
void Func_02003af0();
void Func_02003af2();
void Func_02003afc();
void Func_02003b06();
void Func_02003b08();
void Func_02003b10();
s32 Func_02003b20();
s32 Func_02003b4e();
void Func_02003b5e();
void Func_02003b66();
void Func_02003b76_a();
void Func_02003b76_b();
void Func_02003b7c();
s32 Func_02003b7e();
void Func_02003b82();
void Func_02003b98();
s32 Func_02003ba8();
void Func_02003baa_a();
void Func_02003baa_b();
void Func_02003bb4();
void Func_02003bb8();
void Func_02003bbe();
void Func_02003bd4();
void Func_02003be0();
void Func_02003be2();
void Func_02003bf4();
void Func_02003bfe();
void Func_02003c0a();
void Func_02003c18();
void Func_02003c2a();
void Func_02003c34();
void Func_02003c38();
void Func_02003c3e();
void Func_02003d12_a();
void Func_02003d12_b();
void Func_02003d1e();

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void Scene_RunLongBranchingChoreography(void)
{
    s32 record;
    u8 *work;
    s32 base5_200a5c0;

    Func_02002d34();
    Call4(Func_02002e56, -1, -1, -1, 0);
    Func_02002ce4(1);
    *(u8 *)(Func_02002e68() + 85) = 0;
    Call3(Func_02002e74, 0x37e0000, -1, 0x2980000);
    Func_02002d02(1);
    Func_02002dfc(0, 0, 0);
    if (Value1(Func_02002d5a, 0x85f) != 0) {
        Call4(Func_02002e9c, 0x37e0000, -1, 0x2ba0000, 0);
        Call3(Func_02002e20, 19, 0x36c0000, 0x27a0000);
        Call3(Func_02002e94, 19, 0x3000, 0);
        Call3(Func_02002e36, 0, 0x37e0000, 0x31e0000);
    }
    Func_02002d62();
    Func_02002d50(1);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 40;
    Func_02002efa();
    Func_02002f0e();
    if (Value1(Func_02002dbc, 0x85f) != 0) {
    } else {
        Func_02002de0(80);
        Call3(Func_02002e7a, 19, 0x37e0000, 0x31e0000);
        Call2(Func_02002f02, 0x9999, 0x1333);
        Call4(Func_02002f18_a, 0x37e0000, -1, 0x2ba0000, 1);
        Call3(Func_02002e42, 19, 0xcccc, 0x6666);
        Call3(Func_02002e7e_a, 19, 0x37e, 0x2b8);
        Func_02002e1c(80);
        Call4(Func_02002f44, 0x37e0000, -1, 0x2980000, 1);
        Func_02002eba_a(19);
        Call3(Func_02002eae, 19, 0x34a, 0x2b8);
        Call3(Func_02002eba_b, 19, 0x34a, 0x27c);
        Call3(Func_02002f4e_a, 18, 0x7000, 20);
        Call3(Func_02002ed2, 19, 0x36c, 0x27a);
        Func_02002f0a(19, 3);
        Func_02002e70(20);
        Func_02002f18_b(18, 3);
        Func_02002e7e_b(10);
        Call1(Func_02002f54, 0x1437);
        Call3(Func_02002f76, 0x2012, 0, 10);
        Func_02002f4e_b(19, 2);
        Func_02002f88(19, 0, 20);
        Func_02002f60(18, 1);
        Call3(Func_02002f9a, 0x2012, 0, 10);
        Func_02002f5a(19, 3);
        Func_02002ec0(40);
        Call3(Func_02002fca, 18, 0x105, 60);
        Call2(Func_02002fb2, 0x2012, 0);
        Func_02002f92(18, 1);
        Call3(Func_02002fcc, 0x2012, 0, 10);
        Call2(Func_02002ff6, 19, 0x102);
        Func_02002ef4(60);
        Call3(Func_02002ff8, 19, 0x3000, 10);
        Call3(Func_02003004, 18, 0x5000, 10);
        Call4(Func_02003032, 0x37e0000, -1, 0x2ba0000, 1);
        Call3(Func_02002fb4, 0, 0x37e0000, 0x31e0000);
        Call3(Func_02002f66, 0, 0x9999, 0x4ccc);
        Call3(Func_02002fa8, 0, 0x37e, 0x2d6);
        Func_02002f3e(20);
        Func_02002fe6(0, 3);
        Func_02002f4c(10);
        Func_0200300c(18, 1);
        Call2(Func_0200303c, 0x2012, 0);
        Call4(Func_02003084_a, 0x37e0000, -1, 0x2980000, 1);
        Call3(Func_02002fe8, 0, 0x37e, 0x2ac);
        record = Value1(Func_02002fae, 0);
        if (record != 0) {
            Func_0200301c(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        record = Value1(Func_02002fc2, 0);
        if (record != 0) {
            Func_02003030(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
        if (Value1(Func_02002f8e, 3) != 0) {
            record = Value1(Func_02002fe0, 0);
            if (record != 0) {
                Func_0200304e(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
            }
        }
        Call3(Func_02003000, 1, 0x9999, 0x4ccc);
        Call3(Func_0200300a, 2, 0x9999, 0x4ccc);
        Call3(Func_02003018, 3, 0x10000, 0x8000);
        Func_02003080(1, 2);
        Func_02003088(2, 2);
        Func_02003090(3, 2);
        Call3(Func_02003084_b, 1, -16, 16);
        Func_0200308e(2, 16, 16);
        if (Value1(Func_02002ffc, 3) != 0) {
            Func_020030a2(3, 32, 16);
        }
        Func_020030b0(2);
        Func_020030c8(1, 1);
        Func_020030d0(2, 1);
        Func_020030d8_a(3, 1);
        Func_02003046(10);
        Call3(Func_0200314a, 3, 0xc000, 0);
        Call3(Func_02003156, 1, 0xc000, 0);
        Call3(Func_02003162, 2, 0xc000, 20);
        Func_0200311c(18, 2, 20);
        Call3(Func_02003178, 18, 0x7000, 10);
        Call3(Func_02003172, 0x2012, 0, 10);
        Call3(Func_0200318e, 19, 0x1000, 10);
        ((void (*)())Func_0200313e_a)(19, 3);
        Call3(Func_020031a2, 18, 0x5000, 40);
        Call3(Func_020031ae, 18, 0x7000, 10);
        Func_0200315e(18, 4);
        Call3(Func_020031b0, 0x2012, 0, 10);
        Call2(Func_020031da, 19, 0x102);
        Func_020030d8_b(40);
        Call3(Func_020031dc, 18, 0x5000, 20);
        Call3(Func_020031ee, 18, 0x105, 40);
        Value2(Func_020031ce, 0x2012, 0);
        Call3(Func_020031fa, 1, 0xe000, 0);
        Call3(Func_02003206, 2, 0xa000, 0);
        if (Value2(Func_0200313e_b, 0, 0) == 0) {
            goto L_02000f86;
        }
        L_02000cb6:;
        Func_020031f2((s32)Data_00001440);
        Call3(Func_02003214, 0x2012, 0, 10);
        Call3(Func_0200327e, 19, 0x3000, 0);
        Func_0200322e(18, 4);
        Call3(Func_02003280, 0x2012, 0, 10);
        Call3(Func_0200329c, 1, 0xc000, 0);
        Call3(Func_020032a8, 2, 0xc000, 0);
        Func_02003258_a(0, 3);
        Func_02003258_b(1, 2);
        record = Value1(Func_020031f6, 0);
        if (record != 0) {
            Func_02003230(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Func_02003278(2, 2);
        record = Value1(Func_02003216, 0);
        if (record != 0) {
            Func_02003250(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        if (Value1(Func_020031e6, 3) != 0) {
            Func_020032a2(3, 2);
            record = Value1(Func_02003240, 0);
            if (record != 0) {
                Func_0200327a(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
            }
        }
        Func_020032b0(2);
        Func_020032c2(1, 0, 0);
        Func_020032cc(2, 0, 0);
        Func_020032d6_a(3, 0, 0);
        Call1(Func_0200323c, 0x85f);
        Call3(Func_02003292, 0, 0x10000, 0x8000);
        Call3(Func_020032d6_b, 0, 0x37e, 0x2f0);
        *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
        Func_020033b0();
        Func_020033bc();
        Func_02002596();
    }
    Call3(Func_020032d4, 0, 0x9999, 0x4ccc);
    Call3(Func_02003310, 0, 0x37e, 0x2ac);
    Func_020032ae(80);
    Call2(Func_020033c6, 0x9999, 0x1333);
    Call4(Func_020033de, 0x37e0000, -1, 0x2980000, 1);
    Func_02003354(0);
    Func_0200336c(0, 1);
    record = Value1(Func_0200330a, 0);
    if (record != 0) {
        Func_02003378(1, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    record = Value1(Func_0200331e, 0);
    if (record != 0) {
        Func_0200338c(2, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    if (Value1(Func_020032ea, 3) != 0) {
        record = Value1(Func_0200333c, 0);
        if (record != 0) {
            Func_020033aa(3, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
        }
    }
    Call3(Func_0200335c, 1, 0x9999, 0x4ccc);
    Call3(Func_02003366, 2, 0x9999, 0x4ccc);
    Call3(Func_02003374, 3, 0x10000, 0x8000);
    Func_020033dc(1, 2);
    Func_020033e4(2, 2);
    Func_020033ec(3, 2);
    Call3(Func_020033e0, 1, -16, 16);
    Func_020033ea(2, 16, 16);
    if (Value1(Func_02003358, 3) != 0) {
        Func_020033fe(3, 32, 16);
    }
    Func_0200340c(2);
    Func_02003424(1, 1);
    Func_0200342c(2, 1);
    Func_02003434(3, 1);
    Func_020033a2(10);
    Call3(Func_020034a6_a, 3, 0xc000, 0);
    Call3(Func_020034b2_a, 1, 0xc000, 0);
    Call3(Func_020034be, 2, 0xc000, 20);
    Call3(Func_020034d0, 18, 0x101, 60);
    Call1(Func_020034a6_b, 0x1442);
    Value2(Func_020034b6, 0x2012, 0);
    if (Value2(Func_0200340e, 0, 0) == 1) {
        goto L_02000cb6;
    }
    L_02000f86:;
    Call3(Func_020034f0, 3, 0xc000, 0);
    Call3(Func_020034fc, 0, 0xc000, 0);
    Call3(Func_02003508, 1, 0xc000, 0);
    Call3(Func_02003514, 2, 0xc000, 20);
    Func_020034bc(3, 3);
    Func_020034c4(0, 3);
    Func_020034cc(1, 3);
    Func_020034dc(2, 3);
    Call3(Func_02003546, 18, 0x105, 60);
    Call1(Func_0200351c, 0x1443);
    Call2(Func_02003534, 0x2012, 0);
    record = Func_0200348a(20);
    Func_02003438(record, 0);
    record = Func_02003496(20);
    *(volatile s32 *)(record + 24) = 0x8000;
    *(volatile s32 *)(record + 28) = 0x8000;
    record = Value1(Func_020034a4, 18);
    if (record != 0) {
        Func_02003512(20, *(volatile s32 *)(record + 8), *(volatile s32 *)(record + 16));
    }
    Func_02003428(1);
    Func_0200353a(20, 6, 0);
    Call3(Func_020034d8, 20, 0x20000, 0x10000);
    Call3(Func_0200350c, 20, 0x37e, 0x29c);
    Func_020034b2_b(40);
    Call3(Func_020035a4, 0x2012, 0, 10);
    Call3(Func_020035c6, 3, 0x101, 0);
    Call3(Func_020035d0, 0, 0x101, 0);
    Call3(Func_020035da, 1, 0x101, 0);
    Call3(Func_020035e4, 2, 0x101, 60);
    Func_0200358c(18, 4);
    Call3(Func_020035de, 0x2012, 0, 10);
    Call3(Func_02003600, 1, 0x103, 60);
    Call3(Func_02003604, 1, 0xe000, 10);
    Value2(Func_020035ec, 0x4001, 0);
    Call3(Func_02003618, 3, 0xa000, 0);
    Call3(Func_02003624, 2, 0xa000, 0);
    Call3(Func_02003630, 0, 0x6000, 0);
    if (Value2(Func_02003568, 0, 0) == 1) {
        do {
            Func_020035fc(1, 2);
            Func_0200360c(2, 2);
            Call1(Func_0200362a, 0x1447);
            Value2(Func_0200363a, 0x4001, 0);
        } while (Value2(Func_02003592, 0, 0) != 1);
    }
    Func_02003616(1, 3);
    Call1(Func_0200364c, 0x1448);
    Call3(Func_0200366e, 0x4001, 0, 10);
    Call3(Func_0200368a, 3, 0xc000, 0);
    Call3(Func_02003696, 0, 0xc000, 0);
    Call3(Func_020036a2, 1, 0xc000, 0);
    Call3(Func_020036ae, 2, 0xc000, 10);
    Func_02003656(0, 3);
    Func_02003666(0, 3);
    Func_020035cc(20);
    Call3(Func_020036d6_a, 18, 0x105, 60);
    Call3(Func_020036c8, 0x2012, 0, 20);
    Call3(Func_020036ec, 2, 0x102, 60);
    Call3(Func_020036f0, 18, 0x3000, 10);
    Call3(Func_02003702, 18, 0x101, 60);
    Call3(Func_0200370c, 1, 0x101, 40);
    Func_0200370e(1, 0, 20);
    Func_020036d6_b(1, 1);
    Call3(Func_02003710, 0x4001, 0, 10);
    Call3(Func_0200372c, 3, 0x8000, 0);
    Call3(Func_02003738, 0, 0x2000, 20);
    Call3(Func_0200374c, 2, 0x102, 60);
    Call3(Func_02003750, 2, 0x8000, 10);
    Call3(Func_0200374a, 0x4002, 0, 10);
    Func_02003722(1, 1);
    Func_020036b0(40);
    Func_02003758(1, 3);
    Call3(Func_020037aa, 0x4001, 0, 10);
    Call3(Func_020037c6, 1, 0xe000, 10);
    Func_02003786(1, 1);
    Value2(Func_020037b6, 0x4001, 0);
    Call3(Func_020037e2, 3, 0xa000, 0);
    Call3(Func_020037ee, 2, 0xa000, 0);
    Call3(Func_020037fa, 0, 0x6000, 0);
    while (Value2(Func_02003748, 0, 0) != 0) {
        Call1(Func_020037e0, 0x144e);
        Call2(Func_020037f0, 0x4001, 0);
    }
    Func_0200372a(10);
    Call3(Func_0200382e, 3, 0x8000, 0);
    Func_02003838(1, 0, 10);
    Call3(Func_02003844, 0, 0x2000, 10);
    Func_020037f4(1, 3);
    Call3(Func_0200385e, 2, 0x105, 60);
    Call3(Func_02003862, 2, 0x8000, 10);
    Func_02003812(2, 4);
    Call1(Func_02003848, 0x144f);
    Call3(Func_0200386a, 0x4002, 0, 20);
    Func_02003842(18, 1);
    Call3(Func_0200388e, 18, 0x5000, 10);
    Func_0200383e(18, 4);
    Call3(Func_02003890, 0x2012, 0, 10);
    Call3(Func_020038ac, 3, 0xc000, 0);
    Call3(Func_020038b8, 0, 0xc000, 0);
    Call3(Func_020038c4, 1, 0xc000, 0);
    Call3(Func_020038d0, 2, 0xc000, 20);
    Func_02003878(3, 3);
    Func_02003880(0, 3);
    Func_02003888(1, 3);
    Func_02003898(2, 3);
    Func_020037fe(40);
    Call3(Func_02003908_a, 18, 0x105, 80);
    Call3(Func_0200390c, 19, 0x1000, 10);
    Call2(Func_02003926, 19, 0x102);
    Func_02003824(40);
    Func_02003916(19, 0, 10);
    Func_020038ee(18, 1);
    Func_0200383c(40);
    Call3(Func_02003940, 18, 0x7000, 20);
    Call3(Func_0200393a, 0x2012, 0, 10);
    Func_020038fa(19, 3);
    Func_02003860(20);
    Func_02003908_b(18, 4);
    Func_02003908_c(18, 4);
    Call3(Func_02003962, 0x2012, 0, 10);
    Func_0200392c(20, 6, 0);
    record = Value1(Func_020038ba, 18);
    if (record != 0) {
        Func_020038f4(20, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_0200392a(20);
    Func_0200393c(20, 0, 0);
    Func_020038b2(20);
    Call2(Func_020039c4, 3, 0x102);
    Call2(Func_020039ce, 0, 0x102);
    Call2(Func_020039d8, 1, 0x102);
    Call2(Func_020039e2, 2, 0x102);
    Func_020038e0(40);
    Func_020039a0(19, 2);
    Func_020039da(19, 0, 10);
    Func_0200399a(18, 3);
    Call3(Func_020039ec, 0x2012, 0, 20);
    Func_020039ac(19, 3);
    Func_020039cc(2, 2);
    Func_0200391a(20);
    Call3(Func_02003a0c, 0x4002, 0, 10);
    Call3(Func_02003a28, 19, 0x3000, 0);
    Call3(Func_02003a34, 18, 0x3000, 10);
    Func_020039e4(18, 4);
    Call3(Func_02003a36, 0x2012, 0, 10);
    Call3(Func_02003a58, 1, 0x103, 60);
    Call3(Func_02003a4a, 0x4001, 0, 10);
    Call3(Func_02003a66, 18, 0x5000, 10);
    Func_02003a0e(18, 4);
    Call3(Func_02003a68, 0x2012, 0, 10);
    Func_02003a20(2, 4);
    Call3(Func_02003a7a, 0x4002, 0, 10);
    Call3(Func_02003a96, 18, 0x3000, 10);
    Func_02003a46(18, 3);
    Call3(Func_02003a98, 0x2012, 0, 10);
    Call3(Func_02003aba, 3, 0x107, 0);
    Call3(Func_02003ac4, 0, 0x107, 0);
    Call3(Func_02003ace_a, 1, 0x107, 0);
    Call3(Func_02003ad8, 2, 0x107, 60);
    Call3(Func_02003adc, 18, 0x7000, 10);
    Func_02003a8c(18, 3);
    Func_020039f2(10);
    Call3(Func_02003ae4_a, 0x2012, 0, 10);
    Func_02003abc(19, 2);
    Call3(Func_02003b08, 19, 0x1000, 10);
    Func_02003ab8(19, 3);
    Func_02003a1e(20);
    base5_200a5c0 = (s32)Data_0200a5c0;
    Call3(Func_02003af2, 0, 0x10013, base5_200a5c0);
    Call3(Func_02003afc, 1, 0x10013, base5_200a5c0);
    Call3(Func_02003b06, 2, 0x10013, base5_200a5c0);
    Call3(Func_02003b10, 3, 0x10013, base5_200a5c0);
    Call3(Func_02003a8a, 19, 0x9999, 0x4ccc);
    Call3(Func_02003ace_b, 19, 0x354, 0x286);
    Call3(Func_02003ada, 19, 0x354, 0x29a);
    Call3(Func_02003ae8, 19, 0x360, 0x2a0);
    Call3(Func_02003b7c, 19, 0x1000, 10);
    Call3(Func_02003b76_a, 0x4013, 0, 20);
    Func_02003ae4_b(0);
    Func_02003aea(1);
    Func_02003af0(2);
    Call3(Func_02003baa_a, 0, 0x105, 0);
    Call3(Func_02003bb4, 1, 0x105, 0);
    Call3(Func_02003bbe, 2, 0x105, 60);
    Func_02003b5e(0, 3);
    Func_02003b66(1, 3);
    Func_02003b76_b(2, 3);
    Call3(Func_02003baa_b, 19, 0x10000, base5_200a5c0);
    Func_02003b82(1, 2);
    record = Value1(Func_02003b20, 0);
    if (record != 0) {
        Func_02003b98(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Func_02003be0(2, 2);
    record = Value1(Func_02003b7e, 0);
    if (record != 0) {
        Func_02003bb8(2, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    if (Value1(Func_02003b4e, 3) != 0) {
        Func_02003c0a(3, 2);
        record = Value1(Func_02003ba8, 0);
        if (record != 0) {
            Func_02003be2(3, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
    }
    Func_02003c18(2);
    Func_02003c2a(1, 0, 0);
    Func_02003c34(2, 0, 0);
    Func_02003c3e(3, 0, 0);
    Call3(Func_02003bf4, 0, 0x10000, 0x8000);
    Call3(Func_02003c38, 0, 0x37e, 0x2f0);
    *(s32 *)((*(u8 *volatile *)Data_03001ebc + 0x1c8)) = 16;
    ((s64 (*)())Func_02003d12_a)();
    Func_02003d1e();
    Call1(Func_02003bd4, 0x321);
    Func_02003d12_b(29);
    Func_02003bfe();
}

/* overlays/scene/story/flag_branched/scene_channel.c */
#define ConfigurePrimarySceneChannels Func_02001958
#define ConfigureSecondarySceneChannels Func_02001984
void Func_02003ec4(s32, s32, s32);
void Func_02003ed0(s32, s32, s32);
void Func_02003edc(s32, s32, s32);
void Func_02003ef0(s32, s32, s32);
void Func_02003efc(s32, s32, s32);
void Func_02003f08(s32, s32, s32);

static __inline__ void ConfigureFirst(s32 channel, s32 value, s32 zero)
{
    Func_02003ec4(channel, value, zero);
}

static __inline__ void ConfigureSecond(s32 channel, s32 value, s32 zero)
{
    Func_02003ed0(channel, value, zero);
}

static __inline__ void ConfigureThird(s32 channel, s32 value, s32 zero)
{
    Func_02003edc(channel, value, zero);
}

static __inline__ void ConfigureFirst_02003ef0(s32 channel, s32 value, s32 zero)
{
    Func_02003ef0(channel, value, zero);
}

static __inline__ void ConfigureSecond_02003efc(s32 channel, s32 value, s32 zero)
{
    Func_02003efc(channel, value, zero);
}

static __inline__ void ConfigureThird_02003f08(s32 channel, s32 value, s32 zero)
{
    Func_02003f08(channel, value, zero);
}

void ConfigurePrimarySceneChannels(void)
{
    ConfigureFirst(1, 0xe000, 0);
    ConfigureSecond(2, 0xa000, 0);
    ConfigureThird(3, 0x8000, 0);
}

void ConfigureSecondarySceneChannels(void)
{
    ConfigureFirst_02003ef0(1, 0xc000, 0);
    ConfigureSecond_02003efc(2, 0xc000, 0);
    ConfigureThird_02003f08(3, 0xa000, 0);
}
#undef ConfigurePrimarySceneChannels
#undef ConfigureSecondarySceneChannels

/* overlays/scene/story/flag_branched/scene_data.c */
#define SceneData_GetTableA638 Func_02000088
#define SceneData_GetTableA920 Func_02000094
#define SceneData_SelectRecordByScene21 Func_0200009c
struct SceneRecord {
    u8 unk_000[166];
    u8 field_166;
    u8 unk_167[23];
    u8 field_190;
    u8 unk_191[23];
    u8 field_214;
    u8 unk_215[23];
    u8 field_238;
};

/* Scene selector and table getters for resource_38d. The two records live in
 * the overlay image, which is writable, so the stores below land in it. */
extern s16 Data_02000240[];     /* Shared scene workspace, not overlay data. */
extern u8 Value_00000021;
extern u8 Data_0200a9b4[];      /* In-image record. */
extern u8 Data_0200a99c[];      /* In-image record. */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02002536();           /* Record hand-off. */

s32 Func_0200250c();            /* Story-flag test. */

/*
 * Returns the in-image table at 0x0200a638. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA638(void)
{
    return (u8 *)0x0200a638;
}

/*
 * Returns the in-image table at 0x0200a920. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTableA920(void)
{
    return (u8 *)0x0200a920;
}

/*
 * Picks one of two scene records by the current scene selector. For scene
 * 0x21 the record is handed to Func_02002536 and, when story flag 0x84e is
 * set, four of its bytes are stamped; otherwise the other record is returned
 * untouched. The four offsets are one field at a 24-byte stride over four
 * entries, left literal because the entry layout is not established.
 */
s32 SceneData_SelectRecordByScene21(void)
{
    u8 *p;

    /* A signed halfword read. */
    if (Data_02000240[224] == (s32)&Value_00000021) {
        p = Data_0200a9b4;
        Func_02002536(p);

        if (Func_0200250c(0x84e) != 0) {
            struct SceneRecord *rec = (struct SceneRecord *)p;

            rec->field_166 = 2;
            rec->field_190 = 0;
            rec->field_214 = 3;
            rec->field_238 = 1;
        }

        return (s32)p;
    }
    return (s32)Data_0200a99c;
}
#undef SceneData_GetTableA638
#undef SceneData_GetTableA920
#undef SceneData_SelectRecordByScene21

/* overlays/scene/story/flag_branched/scene_primary_script_head.c */
#define FieldScene_RunScene38d_02000150 Func_02000150
#define Scene_RunScene38dSequenceA Func_020001b4
#define Scene_RunActorSeventeenFlaggedDialogue Func_02000214
#define FieldScene_RunScene38d_02000568 Func_02000568

extern u8 Data_03001ebc[];

void Func_020025c4();
void Func_020025d6();
void Func_020025f6();
void Func_02002620();
void Func_02002688();
void Func_020026ac();
void Func_020026b0();
void Func_020026c8();
void Func_020026ce();
void Func_020026e8();
void Func_02002704();
s32 Func_02002614();
void Func_02002628();
void Func_02002634();
void Func_02002658();
void Func_020026f6();
void Func_02002706();
void Func_0200271e();
s32 Func_02002680();
void Func_02002772();
void Func_0200277a();
s32 Func_02002698();
s32 Func_020029be();
s32 Func_020029c8();
void Func_020029f0();
void Func_02002a0e();
void Func_02002a56();
void Func_02002a62();
void Func_02002aa6();
void Func_02002ac0();
void Func_02002ae4();
void Func_02002af4();
void Func_02002af8();
void Func_02002afc();
void Func_02002b3a();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

void Func_02002688_a();           /* scripted-scene bracket: open */

s32 Func_0200266e();            /* story-flag test; used in a condition */

void Func_02002760();           /* show a dialogue line by id */

void Func_020027ac();           /* wait for the slot's action to finish */

void Func_020026e0();           /* scripted-scene bracket: close */

void FieldScene_RunScene38d_02000150(void)
{
    u32 i;
    s32 record;

    Func_020025c4();
    Call3(Func_020026c8, 14, 0x102, 0);
    Func_02002688(14, 2);
    Func_020025d6(40);
    Call1(Func_020026ac, 0x1764);
    Func_020026ce(14, 0, 20);
    Func_020026b0(14, 0, 0);
    Func_020025f6(20);
    Func_020026e8(14, 0, 10);
    Call3(Func_02002704, 14, 0xb000, 10);
    Func_02002620();
}

void Scene_RunScene38dSequenceA(void)
{
    u32 i;
    s32 record;

    Func_02002628();
    Call1(Func_020026f6, 0x1750);
    if (Value1(Func_02002614, 0x302) != 0) {
        Call1(Func_02002706, 0x1768);
    }
    Func_0200271e(15, 0);
    Call1(Func_02002634, 0x302);
    Func_02002658();
}

void Scene_RunActorSeventeenFlaggedDialogue(void)
{
    u8 *work;

    Func_02002688_a();

    if (Func_0200266e(0x202) != 0) {
        Func_02002760(0x174b);
    } else if (Func_02002680(0x84e) != 0) {
        Func_02002772(0x176e);
    } else {
        Func_0200277a(0x1432);
        if (Func_02002698(0x84d) != 0) {
            work = *(u8 **)0x03001ebc;
            *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        }
    }

    Func_020027ac(17, 0);
    Func_020026e0();
}

void FieldScene_RunScene38d_02000568(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_020029be, 0x84e) == 0) {
        if (Value1(Func_020029c8, 0x322) != 0) {
            Func_020029f0();
            Call3(Func_02002af4, 19, 0x100, 0);
            Call3(Func_02002af8, 19, 0x7000, 10);
            Func_02002ac0(19, 2);
            Func_02002a0e(20);
            Call1(Func_02002ae4, 0x1748);
            Func_02002afc(19, 0);
            Call3(Func_02002a62, 0, 0x10000, 0x8000);
            Call3(Func_02002aa6, 0, 0x268, 0x2fa);
            Call3(Func_02002b3a, 19, 0xd000, 10);
            Func_02002a56();
        }
    }
}
#undef FieldScene_RunScene38d_02000150
#undef Scene_RunScene38dSequenceA
#undef Scene_RunActorSeventeenFlaggedDialogue
#undef FieldScene_RunScene38d_02000568

/* overlays/scene/story/flag_branched/scene_setup.c */
#define SceneData_SelectTableac9cByState Func_02000100
#define Dialogue_RunActor10Message1420 Func_02000130
#define Scene_RunSlotSubjectBranch Func_020004ec
extern s16 Data_02000240[];
extern u8 Value_00000021;
extern u8 Data_0200aca8[];
extern u8 Data_0200ac9c[];
extern u8 Value_00001420;

void Func_020025a4(void);
void Func_02002672(s32);
s32 Func_0200269a(s32, s32);
void Func_020025be(void);
void Func_0200296a(void);
s32 Func_02002ab4(s32, s32, s32);
/* 0x02002996 serves two imports: the two-argument reader that guards the
 * branch and the one-argument flag clear inside its first arm. */
extern s32 Func_02002996_a(s32, s32);
void Func_02002996_b(s32);
void Func_02002a1e(s32, s32);
void Func_0200297c(s32);
void Func_02002982(s32);
void Func_02002990(s32);
void Func_02002af6(s32);
void Func_02002a46(s32, s32);
void Func_02002964(s32);
void Func_020029c8_a(void);

s32 SceneData_SelectTableac9cByState(void)
{
    if (Data_02000240[224] == (s32)&Value_00000021) {
        return (s32)Data_0200aca8;
    }
    return (s32)Data_0200ac9c;
}

void Dialogue_RunActor10Message1420(void)
{
    Func_020025a4();
    Func_02002672((s32)&Value_00001420);
    Func_0200269a(10, 0);
    Func_020025be();
}

void Scene_RunSlotSubjectBranch(s32 slot, s32 subject, s32 flag)
{
    s32 record;

    Func_0200296a();

    record = Func_02002ab4(0, slot, subject);

    if (Func_02002996_a(subject, 0) != -1) {
        Func_02002a1e(slot, 2);
        Func_0200297c(0x84E);
        Func_02002982(flag);
        Func_02002990(0x322);
        Func_02002996_b(0x202);
    } else {
        Func_02002af6(125);
        Func_02002a46(slot, 5);
    }

    Func_02002964(record);
    Func_020029c8_a();
}
#undef SceneData_SelectTableac9cByState
#undef Dialogue_RunActor10Message1420
#undef Scene_RunSlotSubjectBranch

/* overlays/scene/story/flag_branched/scene_state_interaction.c */
#define Scene_RunActorSeventeenFlagDialogue Func_020002cc
s32 Func_02002738();
void Func_0200282a();
void Func_02002832();
s32 Func_02002750();
void Func_02002842();

void Func_02002740();           /* scripted-scene bracket: open */

s32 Func_02002726();            /* story-flag test; used in a condition */

void Func_02002818();           /* show a dialogue line by id */

void Func_0200285a();           /* wait for the slot's action to finish */

void Func_0200278e();           /* scripted-scene bracket: close */

void Scene_RunActorSeventeenFlagDialogue(void)
{
    Func_02002740();

    if (Func_02002726(0x202) != 0) {
        Func_02002818(0x174c);
    } else if (Func_02002738(0x845) == 0) {
        Func_0200282a(0x1436);
    } else {
        Func_02002832(0x1434);
        if (Func_02002750(0x84e) != 0) {
            Func_02002842(0x176f);
        }
    }

    Func_0200285a(17, 0);
    Func_0200278e();
}
#undef Scene_RunActorSeventeenFlagDialogue

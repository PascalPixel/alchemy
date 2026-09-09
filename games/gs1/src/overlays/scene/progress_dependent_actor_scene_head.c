#include "types.h"

#define NULL ((void *)0)
#define CalculateFacingAngle Func_02001a9e
#define FieldScene_RunActorPositionSequence Func_02000170
#define OverlayObject_UpdateFacingTowardTarget Func_02000030
#define SceneData_GetTable9CF4 Func_02000088
#define SceneData_GetTable9db4 Func_02000094
#define SceneData_GetTable9ddc Func_0200009c
#define SceneData_SelectTable9efcByFlags Func_020000a4

#include "facing_object.h"

extern u8 Data_0200a028[];
extern u8 Data_02009fb0[];
extern u8 Data_02009efc[];
extern u8 Data_00000fb0[];
extern u8 Data_00000fbf[];
extern u8 Data_03001ebc[];

s32 Func_02001a9e(s32, s32);
s32 Func_02001b0e(s32);
s32 Func_02001b1c(s32);
s32 Func_02001bda();
void Func_02001c04();
void Func_02001c32();
struct ObjectRuntime *Func_02001c6e(u32);
void Func_02001c7c();
void Func_02001c9c();
void Func_02001ca4();
void Func_02001ca8();
void Func_02001cb2();
void Func_02001cc0_a();
void Func_02001cc0_b();
void Func_02001ccc();
void Func_02001cce();
void Func_02001cd6_a();
void Func_02001cd6_b();
void Func_02001ce6_a();
void Func_02001ce6_b();
void Func_02001cfa();
void Func_02001cfe();
void Func_02001d10();
void Func_02001d1a();
void Func_02001d22();
void Func_02001d2a();
void Func_02001d58();
void Func_02001d88_a();
void Func_02001d88_b();
void Func_02001da4();
void Func_02001db0();
void Func_02001db8_a();
void Func_02001db8_b();
void Func_02001dbe();
void Func_02001dca();
void Func_02001ddc();
void Func_02001de2();
void Func_02001df8();
void Func_02001dfa_a();
void Func_02001dfa_b();
void Func_02001e1e();
void Func_02001e20();
void Func_02001e36();
void Func_02001e3c();
void Func_02001e52();
void Func_02001e5a();
void Func_02001e60();
void Func_02001e66();
void Func_02001e7c();
void Func_02001e86();
s32 Func_02001e8a();
void Func_02001e90();
void Func_02001e94();
void Func_02001e9a();
void Func_02001ea0();
void Func_02001ea2();
void Func_02001ea8();
void Func_02001ec0();
void Func_02001ec6();
void Func_02001eda();
void Func_02001ee6();
void Func_02001efa();
void Func_02001efe();
void Func_02001f0a();
void Func_02001f16();
void Func_02001f1a();
s32 Func_02001f22_a();
s32 Func_02001f22_b();
void Func_02001f2c();
void Func_02001f2e();
void Func_02001f3a();
void Func_02001f44_a();
void Func_02001f44_b();
void Func_02001f50();
void Func_02001f68();
void Func_02001f6a();
void Func_02001f7a();
void Func_02001f98();
void Func_02001f9e_a();
void Func_02001f9e_b();
void Func_02001fa4();
void Func_02001fa6();
void Func_02001fa8();
void Func_02001fb2();
void Func_02001fb4();
void Func_02001fb6();
void Func_02001fbc();
void Func_02001fbe();
void Func_02001fc0();
void Func_02001fc4();
void Func_02001fc6();
void Func_02001fcc_a();
void Func_02001fcc_b();
void Func_02001fd4();
void Func_02002020();
void Func_02002022();
void Func_02002040();
void Func_02002046();
void Func_0200204c();
void Func_02002058();
void Func_0200206a();
void Func_02002072();
void Func_02002076();
void Func_02002084();
void Func_0200208e();
void Func_02002092();
void Func_0200209a();
void Func_020020a6_a();
void Func_020020a6_b();
void Func_020020b4();
void Func_020020b8();
void Func_020020be();
void Func_020020c8();
void Func_020020ce();
void Func_020020d4();
void Func_020020ea();
void Func_020020f2();
void Func_020020f6();
s32 Func_0200211c();
void Func_0200212c();
void Func_0200212e();
void Func_02002130();
void Func_02002132();
void Func_02002138();
void Func_0200214e();
void Func_02002160();
s32 Func_02002162();
void Func_0200216c();
void Func_0200217e();
void Func_02002184();
void Func_0200218e();
void Func_0200219a();
void Func_020021a6();
s32 Func_020021ae();
s32 Func_020021b4();
void Func_020021b8();
void Func_020021be();
void Func_020021c2();
void Func_020021c6();
void Func_020021ca();
void Func_020021cc();
void Func_020021d8();
void Func_020021e8();
void Func_020021f0();
s32 Func_020021fa();
void Func_02002204();
void Func_0200220a();
void Func_0200220c();
void Func_02002210();
void Func_02002216();
void Func_0200221c();
void Func_02002224();
void Func_0200222e();
void Func_02002240();
void Func_0200224a();
void Func_02002250();
void Func_02002252();
void Func_02002262_a();
void Func_02002262_b();
void Func_02002268();
void Func_02002270();
void Func_02002276();
void Func_02002286();
void Func_02002288();
s32 Func_020022a6_a();
s32 Func_020022a6_b();
void Func_020022bc();
void Func_020022c4();
void Func_020022d4();
void Func_020022da();
void Func_020022e8();
void Func_02002304();
void Func_02002308();
void Func_02002310();
void Func_0200231c();
void Func_02002336();
void Func_02002338();
void Func_0200233e();
void Func_02002348();
void Func_0200234e();
void Func_02002350();
void Func_02002378();
void Func_0200238a();
void Func_02002390();
void Func_02002394();
void Func_0200239e();
void Func_020023a6();
void Func_020023a8();
void Func_020023cc();
void Func_020023ce_a();
void Func_020023ce_b();
void Func_020023d0();
void Func_020023de();
void Func_020023e8();
void Func_020023f4();
void Func_02002402();
void Func_0200240e();
void Func_0200241a();
void Func_02002426();
void Func_0200246a();
void Func_02002474();
void Func_0200247e();
void Func_020024ac();
void Func_020024ca();
void Func_020024f0();
void Func_020024fc();

/* Each Func_ symbol names the loader-relocated call word the image holds for
 * one call site, not a runtime address, so several names can reach the same
 * target. Declarations are old-style where the arity varies between sites. */

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

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

struct ObjectRuntime;

s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 ang;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        ang = (u16)CalculateFacingAngle(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(ang - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

/*
 * Returns the in-image table at 0x02009cf4. The eight-byte owner at
 * 0x02000088 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9CF4(void)
{
    return (u8 *)0x02009cf4;
}

/*
 * Returns the in-image table at 0x02009db4. The eight-byte owner at
 * 0x02000094 includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9db4(void)
{
    return (u8 *)0x02009db4;
}

/*
 * Returns the in-image table at 0x02009ddc. The eight-byte owner at
 * 0x0200009c includes its one pool word, which holds that address and is
 * returned without being dereferenced.
 */
u8 *SceneData_GetTable9ddc(void)
{
    return (u8 *)0x02009ddc;
}

/* Picks one of three tables by story progress, testing flags 0x87a then
 * 0x834. */
u8 *SceneData_SelectTable9efcByFlags(void)
{
    if (Func_02001b0e(0x87A) != 0) {
        return Data_0200a028;
    }
    if (Func_02001b1c(0x834) != 0) {
        return Data_02009fb0;
    }
    return Data_02009efc;
}

/*
 * Runs the scene's fixed position and pose sequence for actors 0, 1, 5 and
 * 13, skipped entirely when flag 0x801 is set. Two of the record fields feed
 * the placement calls as s16 values shifted into fixed point, and base5_fbf
 * holds a base that later sites use as base + 1 and base + 2.
 */
void FieldScene_RunActorPositionSequence(void)
{
    u8 *record;
    s32 x, y;
    s32 base5_fbf;

    if (Value1(Func_02001bda, 0x801) != 0) {
    } else {
        Func_02001c04();
        Call3(Func_02001c32, 0, 0x10000, 0x8000);
        Call1(Func_02001cc0_a, 0xfa6);
        Func_02001ca8(13, 1);
        Call3(Func_02001c7c, 0, 232, 0x108);
        Func_02001c9c(0, 0);
        Func_02001cd6_a(0, 13, 20);
        Func_02001cce(13, 2);
        Func_02001d10(13, 0, 10);
        record = (u8 *)Func_02001c6e(0);
        x = *(s16 *)(record + 10);
        y = *(s16 *)(record + 18);
        Func_02001ccc(5, x << 16, y << 16);
        Func_02001cd6_b(1, x << 16, y << 16);
        Call3(Func_02001ca4, 5, 0x8000, 0x4000);
        Call3(Func_02001cb2, 1, 0x8000, 0x4000);
        Call3(Func_02001ce6_a, 5, 248, 0x108);
        Call3(Func_02001cfa, 1, 216, 0x108);
        Func_02001d1a(0, 1);
        Func_02001d22(5, 1);
        Func_02001d2a(1, 1);
        Func_02001cc0_b(4);
        Call3(Func_02001da4, 5, 0xb000, 0);
        Call3(Func_02001db0, 1, 0xd000, 20);
        Func_02001d58(5, 4);
        Func_02001ce6_b(10);
        Func_02001db8_a(5, 0, 20);
        Func_02001d88_a(13, 1);
        Func_02001cfe(10);
        Call3(Func_02001de2, 13, 0x3000, 10);
        Func_02001ddc(13, 0, 10);
        Call3(Func_02001df8, 1, 0x3000, 40);
        Func_02001db8_b(1, 2);
        Func_02001dfa_a(1, 0, 10);
        Func_02001dca(13, 2);
        Call3(Func_02001e1e, 13, 0x5000, 10);
        Func_02001dbe(13, 3);
        Func_02001e20(13, 0, 8);
        Call3(Func_02001e3c, 5, 0x3000, 20);
        Func_02001e36(5, 0, 10);
        Call3(Func_02001e52, 13, 0x3000, 10);
        Func_02001dfa_b(13, 3);
        Func_02001d88_b(10);
        Func_02001e5a(13, 0, 6);
        Call3(Func_02001e7c, 0, 0x101, 0);
        Call3(Func_02001e86, 1, 0x101, 0);
        Call3(Func_02001e90, 5, 0x101, 60);
        Call3(Func_02001e94, 1, 0xd000, 0);
        Call3(Func_02001ea0, 5, 0xb000, 20);
        Func_02001e60(5, 2);
        Func_02001ea2(5, 0, 10);
        Call3(Func_02001ec6, 13, 0x102, 60);
        Func_02001e66(13, 4);
        Func_02001ec0(13, 0, 10);
        Func_02001eda(1, 0, 0);
        Call3(Func_02001ee6, 5, 0x8000, 20);
        Call3(Func_02001efa, 0, 0x100, 60);
        Call3(Func_02001efe, 1, 0xd000, 0);
        Call3(Func_02001f0a, 5, 0xb000, 0);
        Call3(Func_02001f16, 13, 0x5000, 20);
        Call3(Func_02001f22_a, 13, 0x3000, 40);
        Call3(Func_02001f2e, 13, 0x5000, 40);
        Call3(Func_02001f3a, 13, 0x3000, 20);
        Value2(Func_02001f22_b, 13, 0);
        if (Value2(Func_02001e8a, 0, 0) == 0) {
            Func_02001f2c((s32)Data_00000fb0);
        } else {
            Call1(Func_02001f44_a, 0xfb1);
        }
        Func_02001e9a(20);
        Func_02001f1a(13, 3);
        Func_02001ea8(10);
        Func_02001f7a(13, 0, 10);
        Call1(Func_02001f68, 0xfb2);
        Func_02001f50(5, 2);
        Call3(Func_02001fa4, 5, 0x8000, 10);
        Func_02001f9e_a(5, 0, 6);
        Call3(Func_02001fc0, 1, 0x103, 30);
        Func_02001f6a(1, 4, 30);
        Func_02001fcc_a(1, 0, 10);
        Func_02001fc6(1, 0, 6);
        Func_02001fa8(0, 1, 10);
        Func_02001fb2(0, 5, 0);
        Func_02001fb4(13, 1, 10);
        Func_02001fbe(13, 5, 10);
        Func_02001f9e_b(0, 3);
        Func_02001fa6(1, 3);
        Func_02001fb6(5, 3);
        Func_02001f44_b(10);
        Func_02001fbc(5, 1);
        Func_02001fc4(1, 1);
        Func_02001fcc_b(0, 0);
        Call3(Func_02002040, 0, 0x4000, 0);
        Call3(Func_0200204c, 1, 0x4000, 0);
        Call3(Func_02002058, 5, 0x4000, 16);
        Call2(Func_02002072, 13, 0x102);
        Func_02002022(13, 3);
        Func_02001f98(10);
        Func_0200206a(13, 0, 6);
        Call3(Func_0200208e, 0, 0x100, 0);
        Call3(Func_0200209a, 1, 0x100, 0);
        Call3(Func_020020a6_a, 5, 0x100, 40);
        Func_02002046(13, 4);
        Func_02001fd4(10);
        Func_020020a6_b(13, 0, 6);
        Func_02002076(13, 1);
        Func_020020b8(13, 0, 6);
        Call3(Func_020020d4, 5, 0xb000, 10);
        Func_020020ce(5, 0, 6);
        Call3(Func_020020ea, 13, 0x3000, 10);
        Func_02002092(13, 3);
        Func_02002020(6);
        Func_020020f2(13, 0, 10);
        Func_020020b4(0, 2, 0);
        Func_020020be(1, 2, 0);
        Func_020020c8(5, 2, 10);
        Call3(Func_0200212c, 0, 0xc000, 0);
        Call3(Func_02002138, 1, 0xd000, 20);
        Func_02002132(1, 0, 6);
        Call3(Func_0200214e, 13, 0x5000, 10);
        Func_020020f6(13, 3);
        Func_02002084(16);
        Func_0200212e(0, 5, 40);
        Func_02002160(5, 0, 10);
        Func_02002130(13, 2);
        Call3(Func_02002184, 13, 0x3000, 10);
        Func_0200217e(13, 0, 6);
        Call3(Func_0200219a, 0, 0xc000, 0);
        Call3(Func_020021a6, 5, 0xb000, 30);
        Call3(Func_020021b8, 0, 0x105, 0);
        Call3(Func_020021c2, 1, 0x105, 0);
        Call3(Func_020021cc, 5, 0x105, 80);
        Func_0200216c(13, 4);
        Value2(Func_020021b4, 13, 0);
        if (Value2(Func_0200211c, 0, 0) == 0) {
            Call1(Func_020021be, 0xfbd);
        } else {
            Call1(Func_020021c6, 0xfbe);
        }
        Func_020021e8(13, 0, 20);
        base5_fbf = (s32)Data_00000fbf;
        Func_020021d8(base5_fbf);
        Func_0200220a(1, 0, 10);
        Func_020021ca(1, 2);
        Value2(Func_020021fa, 1, 0);
        if (Value2(Func_02002162, 0, 0) == 0) {
            Func_02002204((base5_fbf + 1));
        } else {
            Func_0200220c((base5_fbf + 2));
        }
        Func_0200222e(1, 0, 6);
        Call1(Func_0200221c, 0xfc2);
        Call3(Func_02002250, 5, 0x8000, 10);
        Func_02002210(5, 1);
        Func_02002240(5, 0);
        Func_0200218e(4);
        if (Value2(Func_020021ae, 0, 0) == 1) {
            Func_02002224(5, 2, 20);
            Func_02002276(5, 0, 10);
        } else {
            Func_0200224a(0, 3);
            Func_02002252(1, 3);
            Func_02002262_a(5, 3);
            Func_020021f0(8);
            Func_02002268(0, 0);
            bump_step(1);
        }
        Func_02002288(13, 3);
        Func_02002216(10);
        Func_020022e8(13, 0, 10);
        Call3(Func_02002304, 0, 0xc000, 0);
        Call3(Func_02002310, 1, 0xd000, 0);
        Call3(Func_0200231c, 5, 0xb000, 10);
        Func_020022bc(0, 3);
        Func_020022c4(1, 3);
        Func_020022d4(5, 3);
        Func_02002262_b(10);
        Func_020022da(0, 0);
        Func_02002270(20);
        Func_02002308(13, 2);
        Func_02002338(13, 0);
        Func_02002286(4);
        if (Value2(Func_020022a6_a, 0, 0) == 0) {
            Call1(Func_02002348, 0xfc6);
        } else {
            Call1(Func_02002350, 0xfc9);
        }
        ((void (*)())Func_020022a6_b)(10);
        Func_0200233e(1, 2);
        Func_02002390(1, 0, 10);
        Func_0200238a(1, 0, 6);
        Call3(Func_020023a6, 5, 0x8000, 10);
        Func_0200234e(5, 4);
        Func_020023a8(5, 0, 6);
        Func_02002378(1, 2);
        Call3(Func_020023cc, 1, 0x8000, 10);
        Call3(Func_020023de, 1, 0x103, 30);
        Func_020023d0(1, 0, 10);
        Call3(Func_020023f4, 13, 0x100, 40);
        Func_0200239e(13, 4, 40);
        Call3(Func_02002402, 0, 0xc000, 0);
        Call3(Func_0200240e, 1, 0xc000, 0);
        Call3(Func_0200241a, 5, 0xb000, 20);
        Call3(Func_02002426, 13, 0xb000, 10);
        Func_020024ac(158);
        Call3(Func_02002336, 0x200a0ac, 43, 8);
        Call3(Func_02002394, 13, 0x10000, 0x8000);
        Func_020023ce_a(13, 232, 218);
        Func_020023e8(13, 0, 0);
        Call3(Func_0200246a, 0, 0x101, 0);
        Call3(Func_02002474, 1, 0x101, 0);
        Call3(Func_0200247e, 5, 0x101, 60);
        *(s32 *)((*(u8 **)Data_03001ebc + 0x1c0)) = 0x202;
        Func_020024f0();
        Func_020024fc();
        Func_020024ca(13);
        Func_020023ce_b();
    }
}

#include "types.h"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02001dae
#define SetOverlayObjectMode Func_02001e10
#define SetOverlayObjectSlot Func_02001eb8
#define EffectDescriptorTable Data_0200a120
#define GetPartyEffect Func_02001ed2
#define SpawnEffect Func_02001e80
#define SetEffectVariant Func_02001e8a
#define SetEffectDescriptor Func_02001ea4
#define SetEffectMode Func_02001fee
#define ScaleEffectDeltaFromAccumulated Func_02001f00
#define ScaleEffectDeltaFromOrigin Func_02001f18
#define ScaleEffectVerticalDelta Func_02001f26
#define SetEffectCallbackMode Func_02001fa4
#define SetEffectCallbackArgument Func_02001fb4
#define OverlayObject_PrepareObject Func_02000048
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define SceneEffect_SpawnConfiguredEffect Func_0200013c
#define FieldScene_ApplyActor13Values3And3 Func_02000314
#define OverlayObject_WaitUntilField12BelowLimit Func_02000324
#define SceneData_SelectTableByScene59To5c Func_020003ac
#define SceneData_ReturnZero Func_02000410
#define SceneData_GetTablea36c Func_02000414
#define FieldScene_RunFlags8B2And8B3Steps Func_02000498
#define SceneData_GetTableA500 Func_020004dc
#define FieldScene_RunScene3a5_02000c38 Func_02000c38
#define FieldScene_RunScene3a5_02000c6c Func_02000c6c
#define CalculatePlanarDistance Func_02000e00
#define FieldScene_RunScene3a5_020014b0 Func_020014b0
#define FieldScene_RunScene3a5_02001874 Func_02001874
#define SceneState_SetHalfwordB030 Func_02001c78

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

extern u8 Value_00000059;
extern u8 Value_0000005a;
extern u8 Value_0000005b;
extern u8 Value_0000005c;
extern u8 Data_0200a174[];
extern u8 Data_0200a1d4[];
extern u8 Data_0200a234[];
extern u8 Data_0200a2dc[];
extern u8 Data_0200a12c[];
extern u8 Data_03001ebc[];
extern u8 Data_0000005b[];

void *Func_02001d56(s32, s32, s32, s32);
void Func_02001db4(void *, s32);
void Func_02001dcc(void *, s32);
void Func_02001e5c(void *, s32);
void Func_02002152();
void Func_02001fc0();
s32 Func_020021e6();
s32 Func_020021f0();
void Func_02002200();
void Func_02002202();
void Func_02002208();
void Func_02002238();
void Func_020022c4();
void Func_02002306();
void Func_02002a04();
void Func_02002a54();
void Func_02002a5e();
s32 Func_0200291c();
s32 Func_020029d4();
void Func_020029e6();
void Func_02002a02();
void Func_02002a26();
void Func_02002a8a();
void Func_02002a9a();
typedef s32(*IwramSqrt)(s32);
void Func_02002512();
void Func_02003160();
s32 Func_02003208();
s32 Func_02003216();
void Func_02003216_a();
void Func_0200322e();
s32 Func_02003270();
s32 Func_02003278();
s32 Func_02003280();
s32 Func_0200328c();
void Func_0200328e();
void Func_020032a6();
s32 Func_020032c4();
void Func_020032ce();
s32 Func_020032dc();
void Func_020032f6();
void Func_020032f8();
void Func_020032fc();
void Func_0200330e();
void Func_02003324();
s32 Func_0200332c();
void Func_02003334();
void Func_02003338();
void Func_0200333a();
void Func_02003348();
void Func_02003366();
void Func_0200337e();
void Func_02003380();
void Func_02003398();
void Func_0200339a();
void Func_02003412();
void Func_020035fe();
void Func_02003620();
void Func_02003636();
void Func_02003648();

/*
 * Waits for an object field to fall to a limit -- resource_3a5.
 */

/* Contiguous unnamed leaf-owner run for resource_3a5. */

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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
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
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
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
static __inline__ void Call3_02001874(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void *OverlayObject_PrepareObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02001d56(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *rec = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = rec[9];
        mask = -mask;
        mask &= flags;
        rec[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02001db4(obj, 0);
        Func_02001e5c(obj, 14);
        Func_02001dcc(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SceneEffect_SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void FieldScene_ApplyActor13Values3And3(void) { Func_02002152(13, 3, 3); }

/*
 * The countdown is tested at the top of the loop and decremented inside the
 * body, after the call.  A post-decrement test would move the subtract ahead
 * of the call.
 */
void OverlayObject_WaitUntilField12BelowLimit(u8 *o, s32 limit)
{
    s32 frames = 60;

    while (frames != 0) {
        Func_02001fc0(1);
        frames--;
        if (*(s32 *)(o + 12) <= limit) break;
    }
}

s32 SceneData_SelectTableByScene59To5c(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000059) {
        return (s32)Data_0200a174;
    }
    if (v == (s32)&Value_0000005a) {
        return (s32)Data_0200a1d4;
    }
    if (v == (s32)&Value_0000005b) {
        return (s32)Data_0200a234;
    }
    if (v == (s32)&Value_0000005c) {
        return (s32)Data_0200a2dc;
    }
    return (s32)Data_0200a12c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetTablea36c(void)
{
    return (void *)0x0200a36c;
}

void FieldScene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Func_02002200();
    if (Value1(Func_020021e6, 0x8b2) == 0) {
        if (Value1(Func_020021f0, 0x8b3) == 0) {
            Call1(Func_02002202, 0x8b3);
            Call1(Func_02002208, 0x8b2);
        }
    }
    Func_02002306(123);
    Func_020022c4(3);
    Func_02002238();
}

void *SceneData_GetTableA500(void)
{
    return (void *)0x0200a500;
}

void FieldScene_RunScene3a5_02000c38(void)
{
    extern u8 Data_02000240[];

    Func_02002a04(8, 2);
    Func_02002a54((s32)Data_0000005b, 5);
    do {
        Data_02000240[0x22b] = 3;
    } while (0);
    Func_02002a5e(53, 5);
}

void FieldScene_RunScene3a5_02000c6c(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Value0(Func_0200291c);
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = Value1(Func_020029d4, v5);
        if (record == 0) {
            Func_020029e6(v5);
            break;
        }
    }
    Func_02002a02();
    Func_02002a8a((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Func_02002a9a((v6 + 1), 0);
    Func_02002a26();
}

s32 CalculatePlanarDistance(s32 *position_a, s32 *position_b)
{
    s32 dx = (*position_b++ - *position_a++) >> 16;
    s32 dz = (*position_b - position_a[1]) >> 16;
    s32 dz_squared = dz *dz;
    s32 dx_squared = dx *dx;

    return ((IwramSqrt)0x030001d8)(dx_squared + dz_squared);
}

void FieldScene_RunScene3a5_020014b0(void)
{
    s32 rec8;
    s32 record;
    s32 rect[3];
    s32 shown;
    u16 *shown_addr;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02002512();
    if (Value1(Func_02003208, 0x90a) != 0) {
    } else {
        rec8 = Value1(Func_02003216, 0x200);
        if (rec8 != 0) {
        } else {
            Call1(Func_0200322e, 0x200);
            Func_02003160(1);
            shown_addr = (u16 *)(p5 + 0xcba);
            shown = 0x258;
            *shown_addr = shown;
            record = Value1(Func_02003270, 0);
            *(s32 *)(record + 36) = rec8;
            record = Value1(Func_02003278, 0);
            *(s32 *)(record + 44) = rec8;
            record = Func_02003280(0);
            *(s32 *)(record + 56) = -0x80000000;
            record = Func_0200328c(0);
            *(s32 *)(record + 64) = -0x80000000;
            Func_020032ce(0, 1);
            Func_020032f8(0, 8, 0);
            Func_0200328e(40);
            Func_020032f6(0, 2);
            Call2(Func_02003338, 0, 0x102);
            Func_020032a6(40);
            *(u8 *)(Func_020032c4(0) + 90) &= 254;
            rect[0] = rec8;
            rect[1] = rec8;
            rect[2] = rec8;
            record = Func_020032dc(0);
            Call3(Func_02003216_a, -0x100000, *(u16 *)(record + 6), (s32)rect);
            Call3(Func_020032fc, 0, 0x20000, 0x10000);
            Func_02003334(0, 2);
            Func_0200333a(0, rect[0] / 0x10000, rect[2] / 0x10000);
            Func_02003348(0);
            Func_0200330e(2);
            *(u8 *)(Func_0200332c(0) + 90) |= 1;
            Func_02003324(30);
            Func_02003412(148);
            Func_0200339a(8, 2);
            Func_02003338_a(20);
            Call3(Func_02003366, 8, 0x28000, 0x14000);
            Func_02003380(8, 168, 104);
            Call3(Func_0200337e, 8, 0x8000, 0x4000);
            Func_02003398(8, 168, 92);
            *shown_addr = shown;
            Func_0200328e_a(0);
        }
    }
    /* unlifted: 0x02001590..0x02001594 (2), 0x0200159c..0x020015a0 (2) */
}

void FieldScene_RunScene3a5_02001874(void)
{
    u32 i;
    s32 record;

    Call3_02001874(Func_020035fe, 8, 0x8000, 0x4000);
    Func_02003636(8, 1);
    Func_02003620(8, 168, 96);
    Func_02003648(8, 2);
}

void SceneState_SetHalfwordB030(u16 value)
{
    *(u16 *)0x0200b030 = value;
}

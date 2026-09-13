#include "types.h"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define AcquireOverlayObject      Func_02001aca
#define RunOverlayObjectCommand0  Func_02001b20
#define RunOverlayObjectCommand1  Func_02001b38
#define RunOverlayObjectCommand14 Func_02001bd0
#define CreateOverlayObject Func_02001b22
#define SetOverlayObjectMode Func_02001b7c
#define SetOverlayObjectSlot Func_02001c2c
#define OverlayObject_PrepareSpawnedObject Func_02000048
#define OverlayObject_CreateConfigured Func_020000a0
#define SceneData_GetTablea5cc Func_02000314
#define SceneData_GetTablea9d4 Func_0200031c
#define SceneData_GetTableaa14 Func_02000324
#define SceneData_GetTableaa58 Func_0200032c
#define SceneData_GetTableAae8 Func_02000694
#define FieldScene_RunStep200 Func_02000494
#define FieldScene_RunStep201 Func_020004a8
#define FieldScene_RunStep202 Func_020004c0
#define FieldScene_RunStep203 Func_020004d8
#define FieldScene_RunStep204 Func_020004f0
#define FieldScene_RunStep205 Func_02000504
#define FieldScene_RunStep206 Func_0200051c
#define FieldScene_RunStep207 Func_02000534
#define FieldScene_RunStep208 Func_0200054c
#define FieldScene_RunStep209 Func_02000560
#define FieldScene_RunStep20a Func_02000578
#define FieldScene_RunStep20b Func_02000590
#define FieldScene_RunStep20c Func_020005a8
#define FieldScene_RunStep20d Func_020005bc
#define FieldScene_RunStep20e Func_020005d4
#define FieldScene_RunStep20f Func_020005ec
#define FieldScene_RunStep210 Func_02000604
#define FieldScene_RunStep211 Func_02000618
#define FieldScene_RunStep212 Func_02000630
#define FieldScene_RunStep213 Func_02000648
#define FieldScene_RunStep214 Func_02000660
#define SceneState_ClearStoryVariantWhenIdle Func_02000674
#define SceneState_ApplyRectsByFlag844 Func_02000f58
#define FieldScene_RunFourActorEncounter Func_02000918
#define SceneEffect_RegisterPaletteFadeCallback Func_0200184c
#define EffectDescriptorTable Data_02009da8
#define GetPartyEffect Func_02001c56
#define SpawnEffect Func_02001bf4
#define SetEffectVariant Func_02001bfe
#define SetEffectDescriptor Func_02001c18
#define SetEffectMode Func_02001d62
#define ScaleEffectDeltaFromAccumulated Func_02001c8c
#define ScaleEffectDeltaFromOrigin Func_02001ca4
#define ScaleEffectVerticalDelta Func_02001cb2
#define SetEffectCallbackMode Func_02001d18
#define SetEffectCallbackArgument Func_02001d28
#define SpawnConfiguredEffect Func_0200013c

#include "create_configured_overlay_object.h"
#include "configured_effect_spawn.h"

/* Shared 22-byte head leaf proved identical for this overlay family. */
struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

/*
 * Per-frame integrator for an effect record -- resource_396. It adds the
 * velocity at +68/+72/+76 into the position at +8/+12/+16, decays two of the
 * three velocities, accumulates the rates at +48/+52, and turns the sprite.
 */

extern s16 *Data_0200add0;
extern u8 Data_02009ec8[];
extern u8 Data_0200adc0[];
extern u8 Data_0200adc4[];
extern u8 Data_0200adc8[];
extern u8 Data_0200adcc[];
extern u8 Data_03001ebc[];

void *Func_02001aca(s32, s32, s32, s32);
void Func_02001b20(void *, s32);
void Func_02001b38(void *, s32);
void Func_02001bd0(void *, s32);
void Func_020007d6(int, int, int, int);
void Func_020007e8(int, int, int, int);
void Func_02000800(int, int, int, int);
void Func_02000818(int, int, int, int);
void Func_02000832(int, int, int, int);
void Func_02000844(int, int, int, int);
void Func_0200085c(int, int, int, int);
void Func_02000874(int, int, int, int);
void Func_0200088e(int, int, int, int);
void Func_020008a0(int, int, int, int);
void Func_020008b8(int, int, int, int);
void Func_020008d0(int, int, int, int);
void Func_020008ea(int, int, int, int);
void Func_020008fc(int, int, int, int);
void Func_02000914(int, int, int, int);
void Func_0200092c(int, int, int, int);
void Func_02000946(int, int, int, int);
void Func_02000958(int, int, int, int);
void Func_02000970(int, int, int, int);
void Func_02000988(int, int, int, int);
void Func_020009a2(int, int, int, int);
s32 Func_02002238(void);
s32 Func_02002a2c(s32 flag);
void Func_02001884(void);
void Func_02002992(s32 frames);
s32 Func_02002a40(s32 flag);
void Func_02002a20(s32, s32, s32, s32, s32, s32);
void Func_02002a34(s32, s32, s32, s32, s32, s32);
void Func_02002a4e(s32, s32, s32, s32, s32, s32);
void Func_02002a60(s32, s32, s32, s32, s32, s32);
void Func_02002a6a(s32, s32, s32, s32, s32, s32);
void Func_02002a88(s32, s32, s32, s32, s32, s32);
void Func_02001e14();
void Func_02001f34();
void Func_02001f46();
void Func_02001fc6();
void Func_0200205e();
void Func_020020b2();
void Func_02002124();
void Func_02002144();
void Func_0200222a();
void Func_02002240();
s32 Func_020023e8();
void Func_02002416();
void Func_02002418();
s32 Func_02002420();
s32 Func_0200243a();
void Func_02002442();
void Func_0200246c();
void Func_02002476();
void Func_02002480();
void Func_0200248a();
void Func_02002490();
s32 Func_02002496();
void Func_020024a8();
s32 Func_020024b0();
s32 Func_020024b0_a();
void Func_020024b4();
void Func_020024c0();
s32 Func_020024c6();
void Func_020024ca();
void Func_020024ca_a();
void Func_020024d8();
s32 Func_020024e0();
s32 Func_020024e0_a();
void Func_020024f0();
void Func_020024f2();
void Func_020024fa();
void Func_020024fa_a();
void Func_02002512();
void Func_02002516();
void Func_0200252a();
void Func_02002542();
void Func_02002556();
void Func_020025b2();
void Func_020025c6();
void Func_020025cc();
void Func_020025d8();
void Func_020025e2();
void Func_020025ee();
void Func_020025f0();
void Func_020025fc();
void Func_02002600();
void Func_02002608();
void Func_0200260e();
void Func_02002610();
void Func_02002620();
void Func_02002630();
void Func_0200263a();
void Func_0200263c();
void Func_0200266a();
void Func_02002680();
void Func_02002694();
void Func_02002694_a();
void Func_02002698();
void Func_0200269c();
void Func_020026a4();
void Func_020026a4_a();
void Func_020026b4();
void Func_020026b6();
void Func_020026b8();
void Func_020026bc();
void Func_020026c0();
void Func_020026ca();
void Func_020026d4();
void Func_020026d4_a();
void Func_02002704();
void Func_02002714();
void Func_02002718();
void Func_0200271a();
void Func_02002720();
void Func_0200273a();
void Func_02002744();
void Func_0200274e();
void Func_02002758();
void Func_02002762();
void Func_02002764();
void Func_02002764_a();
void Func_02002770();
void Func_02002772();
void Func_02002782();
void Func_0200278a();
void Func_0200278e();
void Func_02002792();
void Func_0200279a();
void Func_0200279c();
void Func_020027a2();
s32 Func_020027a6();
void Func_020027b2();
void Func_020027d2();
void Func_020027e4();
void Func_020027ec();
s32 Func_020027ee();
s32 Func_020027ee_a();
void Func_020027f8();
void Func_020027fe();
void Func_02002806();
void Func_0200280c();
void Func_02002812();
void Func_0200281a();
void Func_0200281e();
void Func_02002820();
void Func_02002822();
void Func_0200283a();
void Func_02002844();
void Func_02002854();
void Func_02002856();
void Func_02002864();
void Func_02002878();
void Func_0200287c();
void Func_02002882();
void Func_02002884();
void Func_02002890();
void Func_020028bc();
void Func_020028c6();
void Func_020028d6();
void Func_020028e4();
void Func_02002900();
void Func_02002900_a();
void Func_0200290a();
void Func_02002916();
void Func_02002918();
void Func_02002922();
void Func_0200293c();
s32 Func_02002944();
void Func_0200297e();
void Func_02002984();
void Func_020029a6();
void Func_020029ba();
void Func_020029bc();
void Func_020029be();
void Func_020029be_a();
void Func_020029d0();
void Func_020029d2();
void Func_020029d2_a();
void Func_020029d4();
void Func_020029d6();
void Func_020029e0();
void Func_020029e2();
void Func_020029e2_a();
void Func_020029e6();
void Func_020029e8();
void Func_020029ea();
void Func_020029ee();
void Func_020029f0();
void Func_020029f6();
void Func_020029f8();
s32 Func_020029fa();
void Func_02002a00();
void Func_02002a06();
void Func_02002a08();
void Func_02002a0a();
void Func_02002a20_scene_primary_script();
void Func_0200962d();
void Func_02003282(void (*callback)(void));
void Func_02009244(void);
s32 Func_0200328e();

/* Contiguous unnamed leaf-owner run for resource_396. */

/* Return the overlay's first persistent workspace block. */

/* Return the overlay's second persistent workspace block. */

/* Return the overlay's third persistent workspace block. */

/* Return the overlay's fourth persistent workspace block. */

/* Return the overlay's presentation workspace block. */

/* Clear the active story variant once the scene controller is idle. */

/*
 * A loader-relocated call word rather than a runtime address; it turns a value
 * and a duration into a per-frame step. Declared without a prototype, and the
 * call site passes two arguments.
 */

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

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

/*
 * Register this overlay's palette-fade callback.
 *
 * Both addresses are named at their decoded sites: the reference bl's raw
 * decode is 0x02003282 (the registration import as this stream reaches it,
 * semantically Func_080000d8), and the pool word is 0x02009245 -- the
 * callback lives at 0x02009244 in a later-loaded RAM segment, not at this
 * stream's own same-shaped 0x02001244.
 */
void *OverlayObject_PrepareSpawnedObject(s32 x, s32 y, s32 z, s32 kind)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(kind, x, y, z);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        RunOverlayObjectCommand0(obj, 0);
        RunOverlayObjectCommand14(obj, 0xE);
        RunOverlayObjectCommand1(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "configured_effect_spawn_body.inc"
}

void *SceneData_GetTablea5cc(void)
{
    return (void *)0x0200a5cc;
}

void *SceneData_GetTablea9d4(void)
{
    return (void *)0x0200a9d4;
}

void *SceneData_GetTableaa14(void)
{
    return (void *)0x0200aa14;
}

void *SceneData_GetTableaa58(void)
{
    return (void *)0x0200aa58;
}

void FieldScene_RunStep200(void) { Func_020007d6(0x200, 64, 35, 21); }

void FieldScene_RunStep201(void) { Func_020007e8(0x201, 65, 35, 22); }

void FieldScene_RunStep202(void) { Func_02000800(0x202, 66, 35, 23); }

void FieldScene_RunStep203(void) { Func_02000818(0x203, 67, 35, 24); }

void FieldScene_RunStep204(void) { Func_02000832(0x204, 68, 35, 25); }

void FieldScene_RunStep205(void) { Func_02000844(0x205, 69, 35, 26); }

void FieldScene_RunStep206(void) { Func_0200085c(0x206, 70, 35, 27); }

void FieldScene_RunStep207(void) { Func_02000874(0x207, 71, 35, 28); }

void FieldScene_RunStep208(void) { Func_0200088e(0x208, 72, 35, 29); }

void FieldScene_RunStep209(void) { Func_020008a0(0x209, 73, 35, 31); }

void FieldScene_RunStep20a(void) { Func_020008b8(0x20a, 74, 35, 32); }

void FieldScene_RunStep20b(void) { Func_020008d0(0x20b, 79, 35, 50); }

void FieldScene_RunStep20c(void) { Func_020008ea(0x20c, 75, 35, 51); }

void FieldScene_RunStep20d(void) { Func_020008fc(0x20d, 76, 35, 52); }

void FieldScene_RunStep20e(void) { Func_02000914(0x20e, 77, 35, 53); }

void FieldScene_RunStep20f(void) { Func_0200092c(0x20f, 78, 35, 54); }

void FieldScene_RunStep210(void) { Func_02000946(0x210, 80, 35, 55); }

void FieldScene_RunStep211(void) { Func_02000958(0x211, 81, 35, 56); }

void FieldScene_RunStep212(void) { Func_02000970(0x212, 82, 35, 57); }

void FieldScene_RunStep213(void) { Func_02000988(0x213, 83, 35, 58); }

void FieldScene_RunStep214(void) { Func_020009a2(0x214, 84, 35, 59); }

void SceneState_ClearStoryVariantWhenIdle(void)
{
    if (Func_02002238() == 0)
        *Data_0200add0 = -1;
}

void *SceneData_GetTableAae8(void)
{
    return (void *)0x0200aae8;
}

void FieldScene_RunFourActorEncounter(void)
{
    u32 i;
    s32 rec;
    s32 record;
    u8 *work;
    s32 v6;
    s32 v5;
    s32 base5_200962d;
    s32 base5_2009ec8;

    rec = Value1(Func_020023e8, 3);
    *(u8 *)(Func_02002420(3) + 35) &= 254;
    Func_020024b4(3, 2);
    *(u8 *)(Func_0200243a(0) + 35) &= 254;
    Func_020024ca(0, 2);
    Func_02002418(184);
    Func_02002556(17);
    Func_02002442();
    Call3(Func_0200246c, 0, 0xcccc, 0x6666);
    Call3(Func_02002476, 1, 0xcccc, 0x6666);
    Call3(Func_02002480, 2, 0xcccc, 0x6666);
    Call3(Func_0200248a, 3, 0xcccc, 0x6666);
    Call3(Func_020024b0, 0, 0xa60000, 0x500000);
    v6 = 192;
    record = Func_02002496(0);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Call3(Func_020024ca_a, 1, 0x940000, 0x5a0000);
    record = Func_020024b0_a(1);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    Call3(Func_020024e0, 2, 0xb60000, 0x5a0000);
    record = Value1(Func_020024c6, 2);
    *(volatile u16 *)(record + 6) = (v6 << 8);
    if (rec != 0) {
        Call3(Func_020024fa, 3, 0xa60000, 0x680000);
        record = Value1(Func_020024e0_a, 3);
        *(volatile u16 *)(record + 6) = (v6 << 8);
    }
    Func_02001e14(0);
    Func_02002416(10);
    work = *(u8 *volatile *)Data_03001ebc;
    *(volatile s32 *)(((s32)work + 0x1c0)) = 0x100;
    *(volatile s32 *)(((s32)work + 0x1c8)) = 48;
    Func_020025f0();
    Func_020025fc();
    Func_020024f2(20);
    Call2(Func_020025b2, 0x13333, 0x2666);
    Call4(Func_020025cc, 0xa80000, -1, 0x980000, 1);
    Func_020025d8();
    Func_02002516(10);
    v5 = 10;
    Func_0200263c(123);
    Call6(Func_020024f0, 26, 3, 1, 2, v5, 8);
    Call6(Func_020024fa_a, 26, 38, 1, 1, v5, 43);
    Func_02002490(4);
    Call6(Func_02002512, 26, 37, 1, 2, v5, 42);
    Func_020024a8(4);
    Call6(Func_0200252a, 26, 36, 1, 3, v5, 41);
    Func_020024c0(4);
    Call6(Func_02002542, 26, 35, 1, 4, v5, 40);
    Func_020024d8(80);
    Call1(Func_0200260e, 0x14d3);
    Call3(Func_02002630, 0x8009, 0, 20);
    Func_02002600(0, 2);
    Func_02002608(1, 2);
    Func_02002610(3, 2);
    Func_02002620(2, 2);
    Func_020025c6(20);
    Call4(Func_02002698, 0xa80000, -1, 0x5a0000, 1);
    Func_020026a4();
    Func_020025e2(40);
    Func_02001f34(1);
    Func_020025ee(60);
    Func_02002714(21);
    Func_02001f46(4);
    Call3(Func_02002694, 0x8009, 0, 20);
    Call3(Func_020026b6, 0, 0x101, 0);
    Call3(Func_020026c0, 1, 0x101, 0);
    Call3(Func_020026ca, 3, 0x101, 0);
    Call3(Func_020026d4, 2, 0x101, 80);
    Call2(Func_020026bc, 0x8009, 0);
    Func_0200263a(40);
    Call3(Func_020026d4_a, 0x8009, 0, 20);
    Func_02002694_a(0, 3);
    Func_0200269c(1, 3);
    Func_020026a4_a(3, 3);
    Func_020026b4(2, 3);
    Func_0200266a(20);
    Call3(Func_02002704, 0x8009, 0, 20);
    Func_02001fc6(0);
    Func_02002680(40);
    Call3(Func_0200271a, 0x8009, 0, 20);
    Call2(Func_02002744, 0, 0x102);
    Call2(Func_0200274e, 1, 0x102);
    Call2(Func_02002758, 3, 0x102);
    Call2(Func_02002762, 2, 0x102);
    Func_020026b8(60);
    Func_02002720(1, 2);
    Call3(Func_02002764, 1, 0xe000, 10);
    Call3(Func_02002770, 0, 0x6000, 10);
    Call3(Func_02002772, 0x8001, 0, 10);
    Func_0200273a(2, 4);
    Call3(Func_0200278e, 0, 0x2000, 0);
    Call3(Func_0200279a, 2, 0xa000, 0);
    Call3(Func_0200279c, 0x8002, 0, 20);
    Func_0200205e(0);
    Func_02002718(40);
    Call3(Func_020027b2, 0x8009, 0, 10);
    Func_02002782(0, 2);
    Func_0200278a(1, 2);
    Func_02002792(3, 2);
    Func_020027a2(2, 2);
    Func_020027e4(0, (v6 << 8), 0);
    ((void (*)())Func_020027ee)(1, (v6 << 8), 0);
    Func_020027f8(2, (v6 << 8), 40);
    Func_020020b2(4);
    Value2(Func_020027ee_a, 0x8009, 0);
    Call3(Func_02002812, 1, 0xe000, 0);
    Call3(Func_0200281e, 2, 0xa000, 0);
    if (Value2(Func_020027a6, 0, 0) != 0) {
        Call3(Func_02002844, 1, 0x103, 20);
        Func_020027ec(1, 4);
        Call1(Func_02002822, 0x14dd);
        Call2(Func_0200283a, 0x8001, 0);
        Call3(Func_02002864, 2, 0x103, 10);
        Func_0200280c(2, 3);
        Call2(Func_02002854, 0x8002, 0);
    }
    Func_020027d2(20);
    Func_02002124(4);
    Call1(Func_02002856, 0x14df);
    Call3(Func_02002878, 0x8009, 0, 20);
    Call3(Func_02002882, 0x8009, 0, 10);
    Func_02002144(0);
    Func_020027fe(20);
    Call2(Func_02002900, 0x10000, 0);
    Call2(Func_02002900_a, 0x406218, 1);
    Func_02002916(20);
    Func_02002764_a(40);
    Func_0200287c(0, 2);
    Func_02002884(1, 2);
    Func_020028c6(3, 2);
    Func_020028d6(2, 2);
    Func_02002918(1, (v6 << 8), 0);
    Func_02002922(2, (v6 << 8), 20);
    Func_02002890(20);
    *(volatile s32 *)Data_0200adcc = 0;
    {
        volatile s32 *bank = (volatile s32 *)Data_0200adc0;
        bank[0] = 0xa80000;
        bank[1] = 0x200000;
        base5_200962d = (s32)Func_0200962d;
        bank[2] = 0x340000;
    }
    Call2(Func_02002806, base5_200962d, 0xc80);
    Func_020028bc(220);
    Func_0200281a(base5_200962d);
    Call2(Func_020029bc, 0x10000, 1);
    Func_020029d2(20);
    Func_02002820(40);
    Func_0200222a(4);
    Func_020028e4(20);
    Call3(Func_0200297e, 0x8009, 0, 10);
    Func_02002240(0);
    Call2(Func_02002984, 0x8009, 0);
    Call2(Func_0200293c, 8, 0x2009e54);
    Func_0200290a(40);
    Call3(Func_020029be, 1, 0x102, 60);
    Call2(Func_020029a6, 0x8001, 0);
    Call3(Func_020029d2_a, 2, 0x102, 10);
    Call2(Func_020029ba, 0x8002, 0);
    Call3(Func_020029d6, 1, 0xe000, 0);
    Call3(Func_020029e2, 2, 0xa000, 10);
    Call3(Func_020029ee, 0, 0x4000, 10);
    Func_020029be_a(1, 1);
    Call3(Func_020029f8, 0x8001, 0, 10);
    Func_020029d0(2, 1);
    Call3(Func_02002a0a, 0x8002, 0, 10);
    if (rec != 0) {
        Func_020029e6(3, 1);
        Call3(Func_02002a20_scene_primary_script, 0x8003, 0, 10);
    }
    Func_020029e0(0, 3);
    Func_020029e8(1, 3);
    Func_020029f0(3, 3);
    Func_02002a00(2, 3);
    base5_2009ec8 = (s32)Data_02009ec8;
    Func_020029ea(1, base5_2009ec8);
    if (rec != 0) {
        Func_020029f6(3, base5_2009ec8);
    }
    Func_02002a06(2, base5_2009ec8);
    Func_020029d4(20);
    *(u8 *)(Func_020029fa(0) + 35) |= 1;
    Call1(Func_020029e2_a, 0x844);
    Value2(Func_02002944, 0x2009245, 0xc80);
    Func_02002a08();
}

void SceneState_ApplyRectsByFlag844(s32 flag)
{
    if (flag != 0 && Func_02002a2c(0x109) == 0)
        Func_02001884();

    Func_02002992(1);
    if (Func_02002a40(0x844) != 0) {
        s32 w1 = 10;
        Func_02002a20(121, 34, 3, 1, 93, w1);
        {
            s32 w2 = 30;
            Func_02002a34(46, 38, 1, 1, w2, 43);
            Func_02002a4e(0, 0, 1, 2, w2, 9);
        }
        Func_02002a60(26, 3, 1, 2, w1, 8);
        Func_02002a6a(26, 35, 1, 4, w1, 40);
    } else {
        s32 w1 = 10;
        s32 w2 = 8;
        Func_02002a88(11, 8, 1, 2, w1, w2);
    }
}

void SceneEffect_RegisterPaletteFadeCallback(void)
{
    Func_02003282(Func_02009244);
}

/*
 * The decay of the Z velocity stays a signed divide by sixteen: that shape is
 * what reproduces the negative bias and arithmetic shift in the reference.
 */
void Func_0200185c(struct Effect *effect)
{
    s32 velocity_z;
    struct Sprite *sprite;
    s32 velocity_x;

    /* This block orders the Z load after the Y store; do not flatten it. */
    do {
        velocity_x = effect->velocity_x;
        effect->position[0] += velocity_x;
        effect->position[1] += effect->velocity_y;
    } while (0);
    velocity_z = effect->velocity_z;
    effect->position[2] += velocity_z;

    effect->velocity_x = velocity_x - Func_0200328e(velocity_x, 18);
    effect->velocity_z = velocity_z - velocity_z / 16;

    effect->accum18 += effect->rate30;
    effect->accum1c += effect->rate34;

    sprite = effect->sprite;
    sprite->angle += effect->step64;
}

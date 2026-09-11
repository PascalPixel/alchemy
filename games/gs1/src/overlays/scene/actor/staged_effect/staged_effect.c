#include "types.h"
#include "create_configured_overlay_object.h"
#include "create_configured_overlay_object_body.inc"
#include "configured_effect_spawn.h"
#include "staged_actor.h"
#include "configured_effect_spawn_body.inc"

/* overlays/scene/actor/staged_effect/actor_facing.c */
#define Actor_FaceLeaderWhileGrounded Func_02000d58
u8 *Func_0200324c(s32 id);
u16 Func_0200317c(s32 deltaZ, s32 deltaX);
void Func_020032f2(s32 id, s32 angle, s32 frames);

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 Actor_FaceLeaderWhileGrounded(u8 *object)
{
    u8 *leader = Func_0200324c(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Func_0200317c(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Func_020032f2(3, 0xc000, 0);
    }
    return 0;
}
#undef Actor_FaceLeaderWhileGrounded

/* overlays/scene/actor/staged_effect/effect.c */
#define Effect_AdvanceAnchoredRiseFrame Func_02001c20
/*
 * resource_39c owner at 0x020051b0, 64 bytes.
 * Advances a frame counter and, while it is still within 31, reseeds the two
 * accumulators from the counter and re-anchors the position on the source
 * record. Returns whether the step ran.
 */
struct Src_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
};

struct Work_39c {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    s32 f12;                    /* +12 */
    s32 f16;                    /* +16 */
    u8 pad14[4];
    s32 f24;                    /* +24 */
    s32 f28;                    /* +28 */
    u8 pad20[68];
    s16 f100;                   /* +100 */
    u8 pad66[2];
    struct Src_39c *f104;       /* +104 */
};

s32 Func_02004054();

s32 Effect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
{
    struct Src_39c *source = work->f104;
    s16 step = ++work->f100;
    s32 seed;

    if (step > 31) {
        return 0;
    }

    seed = Func_02004054(step << 10);
    work->f24 = seed;
    work->f28 = seed;
    work->f8 = source->f8;
    work->f12 += 0x10000;
    work->f16 = source->f16;
    return 1;
}
#undef Effect_AdvanceAnchoredRiseFrame

/* overlays/scene/actor/staged_effect/overlay_object.c */
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OvObj_CreateConfigured      Func_02000048
#define AcquireOverlayObject      Func_020024a6
#define RunOverlayObjectCommand0  Func_0200250c
#define RunOverlayObjectCommand1  Func_02002524
#define RunOverlayObjectCommand14 Func_020025e4
#define CreateOverlayObject Func_020024fe
#define SetOverlayObjectMode Func_02002568
#define SetOverlayObjectSlot Func_02002640
#define OvObj_CreateConfiguredObject Func_020000a0
#define OvObj_SpawnKind24AtActor Func_02001bc8
#define OvObj_RampWords24And28Over16Frames Func_02001c60
#define OvObj_AdvanceScaleCounter Func_02001c88
#define OvObj_ReleasePublishedAttachment Func_020023b8


void *Func_020024a6(s32, s32, s32, s32);
void Func_0200250c(void *, s32);
void Func_02002524(void *, s32);
void Func_020025e4(void *, s32);
u8 *Func_02004020();
void Func_02004026();
void Func_02004038();
u8 **Func_020047ec(s32, s32);
void Func_02004824(u8 *);

void *OvObj_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = AcquireOverlayObject(fourth, first, second, third);
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

void *OvObj_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
}

/* Spawn and configure the compact companion object at a source position. */
void OvObj_SpawnKind24AtActor(u8 *src)
{
    u8 *obj = Func_02004020(24, *(int *)(src + 8),
                              *(int *)(src + 12), *(int *)(src + 16));
    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        Func_02004026(obj, (void *)0x0200a7b8);
        obj[85] = 0;
        obj[34] = 1;
        obj[35] = 2;
        if (rec != 0) {
            Func_02004038(rec, 2);
            rec[38] = 0;
            rec[9] |= 0x0c;
        }
    }
}

s32 OvObj_RampWords24And28Over16Frames(u8 *obj)
{
    u16 *cnt = (u16 *)(obj + 100);
    s16 n;

    *cnt = *cnt + 1;
    n = (s16)*cnt;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(obj + 24) = (n * 3) << 10;
    *(s32 *)(obj + 28) = (n * 3) << 10;
    return 1;
}

s32 OvObj_AdvanceScaleCounter(u8 *o)
{
    u16 *c = (u16 *)(o + 100);
    s16 n;

    *c = *c + 1;
    n = (s16)*c;
    if (n > 16) {
        return 0;
    }
    *(s32 *)(o + 24) = (n << 11) + 0x10000;
    *(s32 *)(o + 28) = (n << 11) + 0x10000;
    return 1;
}

/* Release the optional published attachment; complete owner, no pool. */
void OvObj_ReleasePublishedAttachment(void)
{
    u8 **pub = Func_020047ec(35, 4);
    u8 *state;
    u8 *obj;

    if (pub == 0)
        return;
    state = *pub;
    obj = *(u8 **)(state + 20);
    if (obj == 0)
        return;
    Func_02004824(obj);
    *(u8 **)(state + 20) = 0;
}
#undef NULL
#undef FIELD_AT_OFFSET
#undef OvObj_CreateConfigured
#undef AcquireOverlayObject
#undef RunOverlayObjectCommand0
#undef RunOverlayObjectCommand1
#undef RunOverlayObjectCommand14
#undef CreateOverlayObject
#undef SetOverlayObjectMode
#undef SetOverlayObjectSlot
#undef OvObj_CreateConfiguredObject
#undef OvObj_SpawnKind24AtActor
#undef OvObj_RampWords24And28Over16Frames
#undef OvObj_AdvanceScaleCounter
#undef OvObj_ReleasePublishedAttachment

/* overlays/scene/actor/staged_effect/scene_audio.c */
#define SceneAudio_PlayCue118AndReturnZero Func_02000d48
void Func_02003362(s32 cue);

/* Play the footprint-motion completion cue. */

s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Func_02003362(118);
    return 0;
}
#undef SceneAudio_PlayCue118AndReturnZero

/* overlays/scene/actor/staged_effect/scene_data.c */
#define SceneData_GetTablea928 Func_02000e90
#define SceneData_ReturnZero Func_02000e98
#define SceneData_GetTableabf8 Func_02000e9c
#define SceneData_GetTableac58 Func_02000ea4
#define Scene_RunStepWithValue1632 Func_02000eac
void Func_0200338c();
void Func_020033ec();
void Func_0200336c();
void Func_020033a8();

/* Contiguous unnamed leaf-owner run for resource_39b. */
void *SceneData_GetTablea928(void) { return (void *)0x0200a928; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetTableabf8(void) { return (void *)0x0200abf8; }

void *SceneData_GetTableac58(void) { return (void *)0x0200ac58; }

/* Apply the overlay's common actor-0 presentation preset. */
void Scene_RunStepWithValue1632(void)
{
    Func_0200338c();
    Func_020033ec(0, 1);
    Func_0200336c(0x1632, 1);
    Func_020033a8();
}
#undef SceneData_GetTablea928
#undef SceneData_ReturnZero
#undef SceneData_GetTableabf8
#undef SceneData_GetTableac58
#undef Scene_RunStepWithValue1632

/* overlays/scene/actor/staged_effect/scene_primary_script.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FieldScene_RunScene39b_02000cc0 Func_02000cc0
#define FieldScene_RunScene39b_02000f48 Func_02000f48
#define Scene_RunSupplementalSequenceTwo Func_02000fd8
#define FieldScene_RunScene39b_0200116c Func_0200116c
#define FieldScene_RunScene39b_02001208 Func_02001208
#define FieldScene_RunScene39b_0200196c Func_0200196c

extern u8 Data_03001ebc[];

void Func_02000e6a();
s32 Func_020030ea();
void Func_0200340a();
void Func_0200342a();
void Func_02003452();
void Func_02003458();
s32 Func_02003464();
void Func_02003470();
void Func_0200348e();
void Func_020034c2();
void Func_020034d2();
void Func_02003568();
void Func_020011e8();
s32 Func_0200345a();
s32 Func_02003466();
s32 Func_0200346c();
s32 Func_02003474();
s32 Func_020034cc();
s32 Func_020034d2_a();
s32 Func_020034e4();
void Func_020034f0();
void Func_020034fe();
void Func_02003512();
void Func_02003560();
void Func_0200357c();
void Func_02003586();
void Func_02003592();
void Func_0200359c();
void Func_020035da();
void Func_020035ea();
void Func_020035ee();
void Func_020035f0();
void Func_020035fe();
void Func_02003604();
void Func_0200366e();
void Func_020036c6();
void Func_020036d6();
void Func_020036dc();
s32 Func_0200365e();
void Func_02003648();
void Func_0200368c();
void Func_02003696();
s32 Func_020036d2();
void Func_020036ee();
s32 Func_020036fa();
void Func_020036fc();
void Func_02003728();
s32 Func_02003748();
void Func_02003772();
s32 Func_0200377a();
s32 Func_02003782();
void Func_020037a8();
void Func_020037aa();
void Func_020037c2();
void Func_020037e4();
void Func_0200383a();
void Func_0200384e();
void Func_02003884();
void Func_02003884_a();
void Func_020038b2();
void Func_020038cc();
s32 Func_02003e30();
void Func_02003e44();
void Func_02003e60();
s32 Func_02003e76();
s32 Func_02003e80();
s32 Func_02003e88();
void Func_02003ea6();
void Func_02003ebe();
void Func_02003ece();
void Func_02003ed8();
void Func_02003edc();
void Func_02003f00();
void Func_02003f08();
void Func_02003f1c();
void Func_02003f20();
void Func_02003f28();
void Func_02003f2c();
void Func_02003f3e();
void Func_02003f46();
void Func_02003f9c();
void Func_02003fb0();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
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
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* A value-returning call sets r0 last of its arguments. */
static __inline__ s32 Value1_0200116c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001208(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3_02001208(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200196c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_0200196c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

s32 FieldScene_RunScene39b_02000cc0(s32 a0)
{
    s32 value;
    s32 magic;
    s32 base5_3001e40;
    s32 none;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    base5_3001e40 = 0x3001e40;
    if ((*(volatile s32 *)base5_3001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    none = 0;
    FIELD(rec, s32, 0) = none;
    value = Func_020030ea();
    magic = -((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) + ((((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) + (((((u32)(value << 3) >> 16) << 1) + ((u32)(value << 3) >> 16)) << 4)) << 8));
    Func_02000e6a((*(volatile s32 *)(a0 + 8) + ((8 - (*(volatile s32 *)base5_3001e40 & 15)) << 16)), (*(volatile s32 *)(a0 + 12) + 0x1a0000), *(volatile s32 *)(a0 + 16), none, magic, 0, 0xb0000, rec);
    return 0;
}

void FieldScene_RunScene39b_02000f48(s32 a0)
{
    u32 i;
    s32 record;

    Func_0200342a();
    Func_02003568(228);
    Call3(Func_02003452, 0, 0x6666, 0x3333);
    Func_020034d2(0, 2);
    Call3(Func_0200348e, 0, 0, -8);
    record = Func_02003464(0);
    Func_0200340a(record, 0);
    Func_02003458(8);
    Func_020034c2(0, ((a0 << 19) + 0x80000), 0);
    Func_02003470(30);
}

void Scene_RunSupplementalSequenceTwo(void)
{
    s32 a;
    s32 b;
    s32 zero;
    s32 counter;
    s32 x;
    s32 y;
    s32 t;
    s32 record;
    u8 *slot;
    u8 slot16[40];

    a = *(volatile s32 *)(Value1(Func_020034d2_a, 0) + 8) / 0x100000;
    b = *(volatile s32 *)(Value1(Func_020034e4, 0) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Func_020034f0();
        Call2(Func_020035da, 0x10000, 0);
        Func_020035f0(60);
        Func_020034fe(120);
        Call2(Func_020035ee, 0x10005, 1);
        Func_02003604(60);
        Func_02003512(40);
        counter = 0;
        slot = slot16;
        zero = 0;
        do {
            *(s32 *)(slot) = 1;
            {
                s32 shown = 0x11e;

                *(u16 *)(slot + 24) = shown;
            }
            *(s32 *)(slot + 28) = 0x200af4c;
            Func_0200366e(246);
            x = 208 - ((u32)(Func_0200345a() << 4) >> 16);
            y = 560 - ((u32)(Func_02003466() << 4) >> 16);
            t = ((u32)(Value0(Func_02003474) << 2) >> 16);
            record = Func_0200346c((((t << 4) - t) << 16) + 0x3c0000, 100);
            Func_020011e8(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Func_02003586(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Func_020036d6(220);
        Func_0200359c(60);
        Call1(Func_02003592, 0x875);
        Value2(Func_020034cc, 0x2008d99, 0xc80);
        Call6(Func_02003560, 37, 98, 10, 97, 5, 3);
        Call6(Func_0200357c, 70, 32, 13, 7, 6, 32);
        Call2(Func_020036c6, 0x10000, 0);
        Func_020036dc(60);
        Func_020035ea(120);
        Func_020035fe();
    }
}

void FieldScene_RunScene39b_0200116c(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Value1_0200116c(Func_0200365e, 0);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    Func_02003648(0x205);
    if (quotient == 7) {
        Func_0200368c(8, 0x200a874);
    } else {
        Func_02003696(8, 0x200a8c8);
    }
}

void FieldScene_RunScene39b_02001208(void)
{
    u32 i;
    u8 *rec7;
    s32 rec8;
    s32 record;

    rec7 = Value1_02001208(Func_020036fa, 0);
    rec8 = Value1_02001208(Func_020036d2, 0x109);
    if (rec8 == 0) {
        Func_020036fc();
        Call4(Func_020037c2, -1, -1, -1, 0);
        rec7[85] = rec8;
        Value3(Func_0200377a, 0, (*(s16 *)((s32)rec7 + 10) << 16), ((*(s16 *)((s32)rec7 + 18) << 16) + -0x100000));
        Func_020037aa(0, 15);
        record = Func_02003748(0);
        Func_020036ee(record, 0);
        Func_0200383a();
        Func_0200384e();
        Func_02003884(228);
        *(volatile s32 *)((s32)rec7 + 108) = 0x2008cc1;
        Call3_02001208(Func_02003772, 0, 0x6666, 0x3333);
        Func_02003884_a(0, 0, 8);
        Func_020037e4(0, 0);
        record = Func_02003782(0);
        Func_02003728(record, 1);
        {
            u8 *rec9 = (u8 *)*(volatile s32 *)((s32)rec7 + 80);
            s32 mask = -13;
            rec9[9] = (mask & rec9[9]) | 4;
        }
        Func_020038b2(0, 0, 10);
        rec7[85] = 3;
        *(volatile s32 *)((s32)rec7 + 108) = rec8;
        Func_020038cc();
        Func_020037a8();
    }
}

void FieldScene_RunScene39b_0200196c(void)
{
    u32 i;
    s32 record;

    if (Value1(Func_02003e30, 0x250) == 0) {
        Call1_0200196c(Func_02003e44, 0x250);
        Func_02003e60();
        record = Func_02003e76(12);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Value1(Func_02003e80, 13);
        *(volatile s32 *)(record + 24) = -0x10000;
        record = Func_02003e88(14);
        *(volatile s32 *)(record + 24) = -0x10000;
        Call3_0200196c(Func_02003ed8, 3, 0x880000, 0x900000);
        Call3_0200196c(Func_02003f1c, 3, 0x4000, 10);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x201;
        Func_02003f9c();
        Func_02003fb0();
        Func_02003ea6(60);
        Func_02003f28(3, 0, 0);
        Func_02003f20(3, 3);
        Func_02003ebe(30);
        Func_02003f00(3, 136, 72);
        Func_02003ece(40);
        Func_02003f46(0, 1);
        Func_02003f2c(3);
        Func_02003f3e(3, 0, 0);
        Call1_0200196c(Func_02003edc, 0x872);
        *(s32 *)(*(u8 *volatile *)Data_03001ebc + 0x1c0) = 0x204;
        Func_02003f08();
    }
}
#undef FIELD
#undef FieldScene_RunScene39b_02000cc0
#undef FieldScene_RunScene39b_02000f48
#undef Scene_RunSupplementalSequenceTwo
#undef FieldScene_RunScene39b_0200116c
#undef FieldScene_RunScene39b_02001208
#undef FieldScene_RunScene39b_0200196c

/* overlays/scene/actor/staged_effect/scene_setup.c */
#define Scene_RunIndexedStep17 Func_02000fac
#define Scene_RunIndexedStep18 Func_02000fb8
#define Scene_RunIndexedStep19 Func_02000fc4
#define SceneData_GetTablead60 Func_02000fd0
#define Scene_RunIndexedStep63 Func_02001144
#define Scene_RunActor8StepWithTableA820 Func_02001150
void Func_02001e82(int page);
void Func_02001e8e(int page);
void Func_02001e9a(int page);
void Func_020036ee_a();
void Func_02003622();
void Func_0200365a();

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */

void Scene_RunIndexedStep17(void)
{
    Func_02001e82(17);
}

void Scene_RunIndexedStep18(void)
{
    Func_02001e8e(18);
}

void Scene_RunIndexedStep19(void)
{
    Func_02001e9a(19);
}

void *SceneData_GetTablead60(void) { return (void *)0x0200ad60; }

void Scene_RunIndexedStep63(void)
{
    Func_020036ee_a(63);
}

void Scene_RunActor8StepWithTableA820(void)
{
    Func_02003622(0x205);
    Func_0200365a(8, (void *)0x0200a820);
}
#undef Scene_RunIndexedStep17
#undef Scene_RunIndexedStep18
#undef Scene_RunIndexedStep19
#undef SceneData_GetTablead60
#undef Scene_RunIndexedStep63
#undef Scene_RunActor8StepWithTableA820

/* overlays/scene/actor/staged_effect/scene_wrapper.c */
#define Scene_CallHelper2e58 Func_02000e6c
#define Scene_CallHelper2d50 Func_02000e84
void Func_02002e58(void);
void Func_02002d50(void);

void Scene_CallHelper2e58(void)
{
    Func_02002e58();
}

void Scene_CallHelper2d50(void)
{
    Func_02002d50();
}
#undef Scene_CallHelper2e58
#undef Scene_CallHelper2d50

/* overlays/scene/actor/staged_effect/shared.c */
#define Scene_Forward31d4 Func_02000e60
#define Scene_RunSingleStep Func_02000e78
void Func_020031d4(void);
void Func_02002eac(void);

void Scene_Forward31d4(void)
{
    Func_020031d4();
}

void Scene_RunSingleStep(void)
{
    Func_02002eac();
}
#undef Scene_Forward31d4
#undef Scene_RunSingleStep

/* overlays/scene/actor/staged_effect/staged_actor_effect_sequence.c */
#define EffectDescriptorTable Data_0200a814
#define GetPartyEffect Func_0200264a
#define SpawnEffect Func_020025d0
#define SetEffectVariant Func_020025da
#define SetEffectDescriptor Func_020025f4
#define SetEffectMode Func_02002776
#define ScaleEffectDeltaFromAccumulated Func_02002658
#define ScaleEffectDeltaFromOrigin Func_02002670
#define ScaleEffectVerticalDelta Func_0200267e
#define SetEffectCallbackMode Func_020026f4
#define SetEffectCallbackArgument Func_02002704
#define StagedActorStepTable Data_0200a700
#define GetStagedActor Func_020028a4
#define FindNextStagedActor Func_0200073e
#define FindBlockingStagedActor Func_0200076a
#define FindElevatedBlockingStagedActor Func_02000796
#define CanStartStagedActorMove Func_02002908
#define SetStagedActorMode Func_020028d8
#define SelectStagedActorSlot Func_02002890
#define StartStagedActorEffect Func_02002abe
#define StartNextStagedActorMove Func_0200291e
#define StartLeadStagedActorMove Func_0200292e
#define FinishStagedActorMove Func_0200293c
#define FinishStagedActorEffect Func_02002ae0
#define SetStagedActorTransition Func_02002944
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define SpawnConfiguredEffect Func_0200013c
#define AdvanceStagedActorPair Func_020003a8


typedef struct { s32 unk0; s32 unk4; s32 unk8; } Desc;

typedef struct { u8 filler0[0x28]; s16 *unk28; } Sub;

typedef struct {
    u8 filler0[6];
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 filler14[0x10];
    s32 unk24;
    u8 filler28[4];
    s32 unk2C;
    u8 filler30[8];
    s32 unk38;
    u8 filler3C[4];
    s32 unk40;
    u8 filler44[0xC];
    Sub *unk50;
} Ent;

extern s32 Data_0200a740[];
extern s32 Data_0200a758[];
extern u8 Value_02008cc1;
extern s32 **Data_03001edc;

struct StagedActor *Func_020028a4(s32 arg0);
struct StagedActor *Func_0200073e(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_0200076a(s32 *arg0, struct StagedActor *arg1);
struct StagedActor *Func_02000796(s32 *arg0, struct StagedActor *arg1);
s32 Func_02002908(struct StagedActor *arg0, s32 *arg1);
void Func_020028d8(struct StagedActor *arg0, s32 arg1);
void Func_02002890(s32 arg0);
void Func_02002abe(s32 arg0);
void Func_0200291e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200292e(struct StagedActor *arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_0200293c(struct StagedActor *arg0);
void Func_02002ae0(void);
void Func_02002944(struct StagedActor *arg0, s32 arg1);
Ent *Func_0200090a(Desc *, Ent *);
s32 Func_02002a8a(Ent *, Desc *);
s32 Func_02000da6(s32 *, s32 *, s32 *);
s32 Func_02002cc4(u8 *, s32 *);
u8 *Func_020033c4(s32);
void Func_020033ba(void);
void Func_020034f8(s32);
void Func_02003428(s32, s32);
void Func_0200341c(s32, s32, s32);
void Func_0200342a_a(s32);
void Func_02003462(s32, s32);
s32 Func_02003400(s32);
void Func_020033a6(s32, s32);
void Func_020033f8(s32);
void Func_02003504(void);
void Func_02003510(void);
void Func_020034d6(s32);
void Func_0200341a(void);
void Func_020044ca(s32 arg0);
void Func_02004456(s32 *object, s32 arg1);
void Func_02004456_a(s32 *object, s32 arg1);

/*
 * Staged actor effect sequence for resource_39b.  Data_03001edc is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
}

void AdvanceStagedActorPair(void)
{
    extern u32 Data_0200a700[];

    s32 destination[3];
    struct StagedActor *lead_actor;
    struct StagedActor *next_actor;
    struct StagedActor *blocking_actor;
    s32 facing_index;
    u32 step;
    s32 move_rate;
    s32 zero;

    lead_actor = GetStagedActor(0);
    facing_index = lead_actor->direction_and_kind >> 12;
    step = StagedActorStepTable[facing_index];
    destination[0] = lead_actor->x.value + (step & 0xffff0000);
    destination[1] = lead_actor->y;
    step <<= 16;
    destination[2] = lead_actor->z.value + step;
    next_actor = FindNextStagedActor(destination, lead_actor);
    if (next_actor == 0) return;

    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    blocking_actor = FindBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    destination[0] = next_actor->x.value;
    destination[1] = next_actor->y + 0x100000;
    destination[2] = next_actor->z.value;
    blocking_actor = FindElevatedBlockingStagedActor(destination, next_actor);
    if (blocking_actor != 0 && (blocking_actor->collision_flags & 1) != 0) return;

    next_actor->transition_mode = 2;
    step = StagedActorStepTable[facing_index];
    destination[0] = next_actor->x.value + (step & 0xffff0000);
    destination[1] = next_actor->y;
    step <<= 16;
    destination[2] = next_actor->z.value + step;
    if (CanStartStagedActorMove(next_actor, destination) > 0) return;

    zero = next_actor->transition_busy;
    if (zero != 0) return;

    SetStagedActorMode(lead_actor, 8);
    move_rate = 0x3333;
    SelectStagedActorSlot(15);
    StartStagedActorEffect(185);
    next_actor->move_rate_x = move_rate;
    next_actor->move_rate_z = move_rate;
    StartNextStagedActorMove(next_actor, destination[0], destination[1], destination[2]);
    lead_actor->move_rate_x = move_rate;
    lead_actor->move_rate_z = move_rate;
    StartLeadStagedActorMove(lead_actor, destination[0], destination[1], destination[2]);
    FinishStagedActorMove(next_actor);
    FinishStagedActorEffect();
    next_actor->x.value = destination[0];
    next_actor->z.value = destination[2];
    next_actor->unknown_24 = zero;
    next_actor->unknown_2c = zero;
    lead_actor->unknown_38 = 0x80000000;
    lead_actor->unknown_40 = 0x80000000;
    lead_actor->unknown_24 = zero;
    lead_actor->unknown_2c = zero;
    lead_actor->x.value = lead_actor->x.parts.cell << 16;
    lead_actor->z.value = lead_actor->z.parts.cell << 16;
    SetStagedActorTransition(lead_actor, 1);
}

s32 Func_0200058c(Ent *a)
{
    extern s32 Data_0200a700[];

    Desc d;
    u32 idx;
    s32 m;
    Ent *r;

    idx = a->unk6 >> 12;
    m = Data_0200a700[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    r = Func_0200090a(&d, a);
    if (r != 0) {
        u32 i = 0;
        s32 v = *r->unk50->unk28;
        s32 *p = Data_0200a740;

        do {
            if (v == *p++) goto done;
            i++;
        } while (i <= 5);
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
    m = Data_0200a700[idx];
    d.unk0 = a->unk8 + (m & 0xffff0000);
    d.unk4 = a->unkC;
    m = m << 16;
    d.unk8 = a->unk10 + m;
    if (Func_02002a8a(a, &d) > 0) {
        a->unk24 = 0;
        a->unk2C = 0;
        a->unk38 = 0x80000000;
        a->unk40 = 0x80000000;
    }
done:
    return 0;
}

s32 Func_02000758(s32 *a)
{
    extern s32 Data_0200a700[];

    s32 sel;
    s32 buf[3];
    u8 *obj;
    s32 cnt;
    s32 ny, nx;
    u8 *p;
    s32 off;
    s32 t, u;
    u8 *E;
    s32 *b;
    s32 ret;
    a[5] = 0;
    obj = (u8 *)Func_02000da6(&sel, a + 1, a);
    if (obj == 0)
        return 0;
    p = obj + 0x22;
    *p = 2;
    cnt = 0;
    E = (u8 *)Data_0200a758;
    off = a[0] << 4;
    {
        s32 o = off + 4;
        t = *(s32 *)(E + o);
        if (t < 0)
            t = -t;
        o = off;
        o += 12;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        ny = (t + u) >> 4;
        t = *(s32 *)(E + off);
        if (t < 0)
            t = -t;
        o = off;
        o += 8;
        u = *(s32 *)(E + o);
        if (u < 0)
            u = -u;
        nx = (t + u) >> 4;
    }
    {
        u8 *oc;
        s32 w;
        s32 d;
        b = buf;
        d = Data_0200a700[sel] & 0xffff0000;
        oc = obj;
        b[0] = *(s32 *)(oc + 8) + d;
        w = *(s32 *)(oc + 12);
        b[1] = w;
        b[2] = *(s32 *)(oc + 16) + (Data_0200a700[sel] << 16);
        a[3] = w;
    }
    for (;;) {
        s32 j, k;
        {
            u8 *E2 = (u8 *)Data_0200a758;
            s32 o2 = a[0] << 4;
            o2 += 4;
            a[4] = b[2] + (*(s32 *)(E2 + o2) << 16);
        }
        j = 0;
        while (j < ny) {
            {
                u8 *E3 = (u8 *)Data_0200a758;
                a[2] = b[0] + (*(s32 *)(E3 + (a[0] << 4)) << 16);
            }
            k = 0;
            while (k < nx) {
                s32 *w2 = a + 2;
                if (Func_02002cc4(obj, w2) == 2)
                    goto found;
                w2[0] = w2[0] + 0x100000;
                k++;
            }
            a[4] = a[4] + 0x100000;
            j++;
        }
        cnt++;
        buf[0] = buf[0] + (Data_0200a700[sel] & 0xffff0000);
        buf[2] = buf[2] + (Data_0200a700[sel] << 16);
    }
found:
    *p = 0;
    ret = 0;
    if (cnt != 0) {
        s32 d = Data_0200a700[sel];
        s32 m = d & 0xffff0000;
        s32 v1 = m *cnt;
        s32 v2 = (d << 16) * cnt;
        u8 *oc = obj;
        a[2] = *(s32 *)(oc + 8) + v1;
        a[3] = *(s32 *)(oc + 12);
        a[4] = *(s32 *)(oc + 16) + v2;
        ret = 1;
    }
    return ret;
}

void Func_02000ed0(s32 a)
{
    u8 *v = Func_020033c4(0);
    Func_020033ba();
    Func_020034f8(0xe4);
    F(v, s32, 0x6c) = (s32)&Value_02008cc1;
    F(v, s32, 0x30) = 0x3333;
    Func_02003428(0, 2);
    Func_0200341c(0, 0, -6);
    Func_0200342a_a(0);
    Func_02003462(0, 15);
    Func_020033a6(Func_02003400(0), 0);
    F(v, s32, 0x6c) = 0;
    Func_020033f8(30);
    Func_02003504();
    Func_02003510();
    Func_020034d6(a);
    Func_0200341a();
}

/*
 * Clear the current record's flag word and, if it has a linked object, reset
 * that object's halfword at +0x64, notify twice and drop the link.  The
 * 72-byte owner includes its three pool words.  Func_02004456 and
 * Func_02004456_a are one import called twice with very different second
 * arguments; its parameter meaning is unverified, so each call is left as
 * compiled rather than unified.
 */
void Func_02001fe8(void)
{
    s32 *record = *Data_03001edc;
    s32 *target;

    if (record[0] == 0) {
        return;
    }

    record[0] = 0;
    Func_020044ca(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Func_02004456(target, (s32)0x0200a7dc);
        Func_02004456_a(target, 7);
        record[5] = 0;
    }
}
#undef EffectDescriptorTable
#undef GetPartyEffect
#undef SpawnEffect
#undef SetEffectVariant
#undef SetEffectDescriptor
#undef SetEffectMode
#undef ScaleEffectDeltaFromAccumulated
#undef ScaleEffectDeltaFromOrigin
#undef ScaleEffectVerticalDelta
#undef SetEffectCallbackMode
#undef SetEffectCallbackArgument
#undef StagedActorStepTable
#undef GetStagedActor
#undef FindNextStagedActor
#undef FindBlockingStagedActor
#undef FindElevatedBlockingStagedActor
#undef CanStartStagedActorMove
#undef SetStagedActorMode
#undef SelectStagedActorSlot
#undef StartStagedActorEffect
#undef StartNextStagedActorMove
#undef StartLeadStagedActorMove
#undef FinishStagedActorMove
#undef FinishStagedActorEffect
#undef SetStagedActorTransition
#undef F
#undef SpawnConfiguredEffect
#undef AdvanceStagedActorPair

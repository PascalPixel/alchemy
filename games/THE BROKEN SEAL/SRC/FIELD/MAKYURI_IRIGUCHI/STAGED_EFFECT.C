#include "TYPES.H"
#include "FIELD_EVENT.H"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_CreateConfigured      Func_02000048
#define AcquireOverlayObject      Func_020024a6
#define RunOverlayObjectCommand0  Func_0200250c
#define RunOverlayObjectCommand1  Func_02002524
#define RunOverlayObjectCommand14 Func_020025e4
#define CreateOverlayObject Func_020024fe
#define SetOverlayObjectMode Func_02002568
#define SetOverlayObjectSlot Func_02002640
#define OverlayObject_CreateConfiguredObject Func_020000a0
#define OverlayObject_SpawnKind24AtActor Func_02001bc8
#define OverlayObject_RampWords24And28Over16Frames Func_02001c60
#define OverlayObject_AdvanceScaleCounter Func_02001c88
#define OverlayObject_ReleasePublishedAttachment Func_020023b8
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
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define SpawnConfiguredEffect Func_0200013c
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define FieldScene_RunScene39b_02000cc0 Func_02000cc0
#define FieldScene_RunScene39b_02000f48 Func_02000f48
#define FieldScene_RunSupplementalSequenceTwo Func_02000fd8
#define FieldScene_RunScene39b_0200116c Func_0200116c
#define FieldScene_RunScene39b_02001208 Func_02001208
#define FieldScene_RunScene39b_0200196c Func_0200196c
#define SceneAudio_PlayCue118AndReturnZero Func_02000d48
#define SceneActor_FaceLeaderWhileGrounded Func_02000d58
#define FieldScene_Forward31d4 Func_02000e60
#define FieldScene_RunSingleStep Func_02000e78
#define FieldScene_CallHelper2e58 Func_02000e6c
#define FieldScene_CallHelper2d50 Func_02000e84
#define SceneData_GetTablea928 Func_02000e90
#define SceneData_ReturnZero Func_02000e98
#define SceneData_GetTableabf8 Func_02000e9c
#define SceneData_GetTableac58 Func_02000ea4
#define FieldScene_RunStepWithValue1632 Func_02000eac
#define FieldScene_RunIndexedStep17 Func_02000fac
#define FieldScene_RunIndexedStep18 Func_02000fb8
#define FieldScene_RunIndexedStep19 Func_02000fc4
#define SceneData_GetTablead60 Func_02000fd0
#define FieldScene_RunIndexedStep63 Func_02001144
#define FieldScene_RunActor8StepWithTableA820 Func_02001150
#define SceneEffect_AdvanceAnchoredRiseFrame Func_02001c20

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"
#include "STAGED_ACTOR.H"

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

extern u8 Value_02008cc1;
extern s32 **Data_03001edc;

void *Func_020024a6(s32, s32, s32, s32);
void Func_0200250c(void *, s32);
void Func_02002524(void *, s32);
void Func_020025e4(void *, s32);
u8 *Func_02004020();
void Func_02004026();
void Func_02004038();
u8 **Func_020047ec(s32, s32);
void Func_02004824(u8 *);
u8 *Func_020033c4(s32);
void Func_020033ba(void);
void Func_020034f8(s32);
void Func_02003428(s32, s32);
void Func_0200341c(s32, s32, s32);
void Func_0200342a(s32);
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
void Func_02000e6a();
s32 Func_020030ea();
void Func_0200340a();
void Func_0200342a_a();
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
void Func_02003362(s32 cue);
u8 *Func_0200324c(s32 id);
u16 Func_0200317c_a(s32 deltaZ, s32 deltaX);
void Func_020032f2(s32 id, s32 angle, s32 frames);
void Func_020031d4(void);
void Func_02002eac(void);
void Func_02002e58(void);
void Func_02002d50(void);
void Func_0200338c();
void Func_020033ec();
void Func_0200336c();
void Func_020033a8();
void Func_02001e82(int page);
void Func_02001e8e(int page);
void Func_02001e9a(int page);
void Func_020036ee_a();
void Func_02003622();
void Func_0200365a();
s32 Func_02004054();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value0(s32 (*f)())
{
    extern u8 Data_03001ebc[];

    return f();
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    f(a0, a1);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call8(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5, a6, a7);
}

/* A value-returning call sets r0 last of its arguments. */
static __inline__ s32 Value1_0200116c(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ s32 Value1_02001208(s32 (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call3_02001208(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200196c(void (*f)(), s32 a0)
{
    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ void Call3_0200196c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u8 Data_03001ebc[];

    f(a0, a1, a2);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
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

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

/*
 * Staged actor effect sequence for resource_39b.  Data_03001edc is the
 * current-object cell: a pointer read once, then dereferenced to reach the
 * live record.
 */
void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
#include "CONFIGURED_EFFECT_SPAWN_BODY.INC"
}


s32 FieldScene_RunScene39b_02000cc0(s32 a0)
{
    extern volatile s32 Data_03001e40;

    s32 value;
    s32 magic;
    u8 storage[40];
    u8 *rec = storage;

    FIELD(rec, s32, 4) = 7;
    if ((Data_03001e40 & 1) == 0) {
        FIELD(rec, s32, 4) = 5;
    }
    FIELD(rec, s32, 8) = 0xcccc;
    FIELD(rec, s32, 12) = 0xcccc;
    FIELD(rec, s32, 0) = 0;
    value = Func_020030ea();
    magic = -(s32)(((u32)(value << 3) >> 16) * 0x3333);
    Func_02000e6a((*(s32 *)(a0 + 8) + ((8 - (Data_03001e40 & 15)) << 16)), (*(s32 *)(a0 + 12) + 0x1a0000), *(s32 *)(a0 + 16), 0, magic, 0, 0xb0000, rec);
    return 0;
}

/* Play the footprint-motion completion cue. */
s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Func_02003362(118);
    return 0;
}

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object)
{
    u8 *leader = Func_0200324c(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Func_0200317c_a(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Func_020032f2(3, 0xc000, 0);
    }
    return 0;
}

void FieldScene_Forward31d4(void)
{
    Func_020031d4();
}

void FieldScene_CallHelper2e58(void)
{
    Func_02002e58();
}

void FieldScene_RunSingleStep(void)
{
    Func_02002eac();
}

void FieldScene_CallHelper2d50(void)
{
    Func_02002d50();
}

/* Contiguous unnamed leaf-owner run for resource_39b. */
void *SceneData_GetTablea928(void) { return (void *)0x0200a928; }

int SceneData_ReturnZero(void) { return 0; }

void *SceneData_GetTableabf8(void) { return (void *)0x0200abf8; }

void *SceneData_GetTableac58(void) { return (void *)0x0200ac58; }

/* Apply the overlay's common actor-0 presentation preset. */
void FieldScene_RunStepWithValue1632(void)
{
    Func_0200338c();
    Func_020033ec(0, 1);
    Func_0200336c(0x1632, 1);
    Func_020033a8();
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
    Func_0200342a(0);
    Func_02003462(0, 15);
    Func_020033a6(Func_02003400(0), 0);
    F(v, s32, 0x6c) = 0;
    Func_020033f8(30);
    Func_02003504();
    Func_02003510();
    Func_020034d6(a);
    Func_0200341a();
}

void FieldScene_RunScene39b_02000f48(s32 a0)
{
    extern u8 Data_03001ebc[];

    u32 i;
    s32 record;

    Func_0200342a_a();
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

/* Contiguous unnamed leaf-owner run for resource_39b. */

/* Clear the scene flag and point actor 8 at its first local path. */
void FieldScene_RunIndexedStep17(void)
{
    Func_02001e82(17);
}

void FieldScene_RunIndexedStep18(void)
{
    Func_02001e8e(18);
}

void FieldScene_RunIndexedStep19(void)
{
    Func_02001e9a(19);
}

void *SceneData_GetTablead60(void) { return (void *)0x0200ad60; }

void FieldScene_RunSupplementalSequenceTwo(void)
{
    extern u8 Data_03001ebc[];

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

    a = *(s32 *)(Value1(Func_020034d2_a, 0) + 8) / 0x100000;
    b = *(s32 *)(Value1(Func_020034e4, 0) + 16) / 0x100000;
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

void FieldScene_RunIndexedStep63(void)
{
    Func_020036ee_a(63);
}

void FieldScene_RunActor8StepWithTableA820(void)
{
    Func_02003622(0x205);
    Func_0200365a(8, (void *)0x0200a820);
}

void FieldScene_RunScene39b_0200116c(void)
{
    extern u8 Data_03001ebc[];

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
    struct FieldActor *actor;
    s32 flag;
    s32 record;

    actor = (struct FieldActor *)Value1_02001208(Func_020036fa, 0);
    flag = Value1_02001208(Func_020036d2, 0x109);
    if (flag == 0) {
        Func_020036fc();
        Call4(Func_020037c2, -1, -1, -1, 0);
        actor->motion_flags = 0;
        Value3(Func_0200377a, 0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
        Func_020037aa(0, 15);
        record = Func_02003748(0);
        Func_020036ee(record, 0);
        Func_0200383a();
        Func_0200384e();
        Func_02003884(228);
        actor->update = (void (*)(union FieldObject *))FieldScene_RunScene39b_02000cc0;
        Call3_02001208(Func_02003772, 0, 0x6666, 0x3333);
        Func_02003884_a(0, 0, 8);
        Func_020037e4(0, 0);
        record = Func_02003782(0);
        Func_02003728(record, 1);
        actor->sprite->priority = 1;
        Func_020038b2(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        Func_020038cc();
        Func_020037a8();
    }
}

void FieldScene_RunScene39b_0200196c(void)
{
    extern u8 *Data_03001ebc;

    s32 record;

    if (Value1(Func_02003e30, 0x250) == 0) {
        Call1_0200196c(Func_02003e44, 0x250);
        Func_02003e60();
        record = Func_02003e76(12);
        *(s32 *)(record + 24) = -0x10000;
        record = Value1(Func_02003e80, 13);
        *(s32 *)(record + 24) = -0x10000;
        record = Func_02003e88(14);
        *(s32 *)(record + 24) = -0x10000;
        Call3_0200196c(Func_02003ed8, 3, 0x880000, 0x900000);
        Call3_0200196c(Func_02003f1c, 3, 0x4000, 10);
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x201;
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
        *(s32 *)(Data_03001ebc + 0x1c0) = 0x204;
        Func_02003f08();
    }
}

/* Spawn and configure the compact companion object at a source position. */
void OverlayObject_SpawnKind24AtActor(u8 *src)
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

s32 SceneEffect_AdvanceAnchoredRiseFrame(struct Work_39c *work)
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

s32 OverlayObject_RampWords24And28Over16Frames(u8 *obj)
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

s32 OverlayObject_AdvanceScaleCounter(u8 *o)
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

/* Release the optional published attachment; complete owner, no pool. */
void OverlayObject_ReleasePublishedAttachment(void)
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

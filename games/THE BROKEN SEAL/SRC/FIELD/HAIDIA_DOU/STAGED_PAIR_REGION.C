#include "TYPES.H"

#define SetEffectRecordMode Func_02000030
#define NULL ((void *)0)
#define CreateOverlayObject Func_02001cf6
#define SetOverlayObjectMode Func_02001d58
#define SetOverlayObjectSlot Func_02001df0
#define OverlayObject_CreateConfigured Func_02000048
#define OverlayObject_CreateConfiguredB Func_020000a0
#define EffectDescriptorTable Data_02009f60
#define GetPartyEffect Func_02001e1a
#define SpawnEffect Func_02001dc8
#define SetEffectVariant Func_02001dd2
#define SetEffectDescriptor Func_02001dec
#define SetEffectMode Func_02001f26
#define ScaleEffectDeltaFromAccumulated Func_02001e90
#define ScaleEffectDeltaFromOrigin Func_02001ea8
#define ScaleEffectVerticalDelta Func_02001eb6
#define SetEffectCallbackMode Func_02001eec
#define SetEffectCallbackArgument Func_02001efc
#define SpawnConfiguredEffect Func_0200013c
#define SceneState_SetValues8_3_4 Func_02000cc0
#define SceneActor_WaitActorDescent Func_02000cd0
#define SceneActor_FaceActorZero Func_02000cf8
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_02009f9c
#define SecondaryOverlayData Data_0200a014
#define TertiaryOverlayData Data_0200a134
#define DefaultOverlayData Data_02009f6c
#define SceneData_SelectByRuntimeSelector Func_02000d20
#define SceneData_ReturnZero Func_02000d74
#define SceneData_GetTablea1dc Func_02000d78
#define SceneState5D Value_0000005d
#define SceneState5E Value_0000005e
#define SceneState5F Value_0000005f
#define SceneDataA234 Data_0200a234
#define SceneDataA2C4 Data_0200a2c4
#define SceneDataA39C Data_0200a39c
#define PrimaryOverlayData_02000d80 Data_0200a420
#define SecondaryOverlayData_02000d80 Data_0200a450
#define TertiaryOverlayData_02000d80 Data_0200a624
#define DefaultOverlayData_02000d80 Data_0200a414
#define SelectSceneDataByState Func_02000d80
#define SceneData_SelectSecondaryByRuntimeSelector Func_0200169c
#define FieldScene_ConfigureRegionAtRow15 Func_02000dcc
#define FieldScene_ConfigureRegionAtRow17 Func_02000de8
#define FieldScene_RunInitBracketThenSequence Func_020010c8
#define FieldScene_RunActor8ZeroStep Func_02001378
#define FieldScene_RunScene3a6SequenceA Func_020011a0
#define FieldScene_RunScene3a6SequenceB Func_02001258
#define FieldScene_RunScene3a6SequenceC Func_02001300
#define FieldScene_RunScene3a6_020014ac Func_020014ac
#define FieldScene_RunShiftAndSetFlag301 Func_020018b4
#define FieldScene_RunActor11Transition301 Func_020018f4
#define SceneAudio_PlayCue123AndDispatchWork364 Func_02001748
#define DialogueLayout_ConfigureRowsByFlag301 Func_02001770
#define SceneActor_PlaceAtTile Func_02001938
#define ActorPresentation_AdvanceActorElevenStates Func_02001bfc

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
#include "CONFIGURED_EFFECT_SPAWN.H"
#include "STAGED_ACTOR.H"
#include "SELECT_OVERLAY_DATA_BY_RUNTIME_SELECTOR.H"

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


typedef struct Obj {
    s32 f00;
    s32 f04;
    s32 f08;
    s32 f0c;
    s32 f10;
    u8 f14[28];
    s32 f30;
    s32 f34;
} Obj;

/* resource_3a6 owner at 0x02001938, 76 bytes.
 * Fetches a record for the caller's id and, when it exists, marks it and
 * writes two 20.12 fixed-point coordinates biased by half a unit. */
struct Rec_3a6 {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[14];
    u8 f34;                     /* +34 */
    u8 f35;                     /* +35 */
};


extern u8 Value_0000005d;
extern u8 Value_0000005e;
extern u8 Value_0000005f;
extern u8 Data_0200a234[];
extern u8 Data_0200a2c4[];
extern u8 Data_0200a39c[];

void *Func_02001c9e(s32, s32, s32, s32);
void Func_02001cfc(void *, s32);
void Func_02001d14(void *, s32);
void Func_02001d94(void *, s32);
Obj *Func_020034b6_a();
Obj *Func_020034b6_b();
Obj *Func_020034be(s32);
void Func_020034b4(void);
void Func_02003496(Obj *, s32, s32, s32);
void Func_0200347e(Obj *, s32);
void Func_020034c2(Obj *, s32, s32, s32);
void Func_020034c0(Obj *, s32);
void Func_020034ee(Obj *);
void Func_0200354a(void);
void Func_02002a16(s32, s32, s32);
void Func_020028fa(s32 cnt);
u8 *Func_020029bc(s32 no);
s32 Func_0200293e(s32 dz, s32 dx);
void Func_02002a44();
void Func_02002a60();
void Func_02002d78();
void Func_02001478();
void Func_02002d88();
void Func_02002050();
void Func_02003028(void);
void Func_02003080(s32 actor, s32 mode);
void Func_0200303c(void);
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
void Func_020034b8(s32 cue);
void Func_020034a6(s32 val);
s32 Func_02003418(s32 flagId);
void Func_020030d2(s32 width, s32 top, s32 bottom);
void Func_020030ee(s32 width, s32 top, s32 bottom);
struct Rec_3a6 *Func_02003602();
void Func_02003670();
void Func_02003900(s32 actor, s32 state);
void Func_02003908(s32 actor, s32 state);

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

    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{

    extern u8 Data_03001ebc[];

    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{

    extern u8 Data_03001ebc[];

    return f(a0, a1, a2);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{

    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_020011a0(s32 (*f)(), s32 a0)
{

    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{

    extern u8 Data_03001ebc[];

    f(a0, a1);
}

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
static __inline__ void Call0(void (*f)())
{

    extern u8 Data_03001ebc[];

    f();
}

static __inline__ void Call1_02001258(void (*f)(), s32 a0)
{

    extern u8 Data_03001ebc[];

    f(a0);
}

static __inline__ s32 Value1_02001258(s32 (*f)(), s32 a0)
{

    extern u8 Data_03001ebc[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    extern u8 Data_03001ebc[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value6(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    extern u8 Data_03001ebc[];

    return f(a0, a1, a2, a3, a4, a5);
}

void Func_020033f2(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);

void Func_02003416(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);

void Func_02003434(s32 left, s32 top, s32 width, s32 height,
                          s32 right, s32 bottom);

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateConfigured(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    u8 *obj = Func_02001c9e(arg3, arg0, arg1, arg2);

    if (obj != NULL) {
        u8 *object = *(u8 **)(obj + 0x50);
        s32 flags;
        s32 mask = 13;

        flags = object[9];
        mask = -mask;
        mask &= flags;
        object[9] = mask;
        obj[0x55] = 0;
        obj[0x59] = 8;
        Func_02001cfc(obj, 0);
        Func_02001d94(obj, 14);
        Func_02001d14(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
}

void SpawnConfiguredEffect(s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz,
                           u32 flags, const struct ConfiguredEffectOptions *options)
{
    extern s32 SceneStateWords[];

#include "CONFIGURED_EFFECT_SPAWN_BODY.INC"
}


/* Open the three-by-four presentation block used by this resource. */
void SceneState_SetValues8_3_4(void)
{
    Func_02002a16(8, 3, 4);
}

/* Give the actor at most sixty frames to descend to its target height, then
 * clamp the live height to the target so the following scene starts exact. */
void SceneActor_WaitActorDescent(u8 *obj)
{
    s32 cnt = 60;

    while (cnt != 0) {
        Func_020028fa(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= *(s32 *)(obj + 20))
            break;
    }
    *(s32 *)(obj + 12) = *(s32 *)(obj + 20);
}

/* Point an object toward actor zero using their fixed-point X/Z delta. */
s32 SceneActor_FaceActorZero(u8 *obj)
{
    u8 *target = Func_020029bc(0);
    s32 dz = *(s32 *)(target + 16) - *(s32 *)(obj + 16);
    s32 dx = *(s32 *)(target + 8) - *(s32 *)(obj + 8);

    *(s16 *)(obj + 6) = (s16)Func_0200293e(dz, dx);
    return 0;
}

s32 SceneData_SelectByRuntimeSelector(void)
{
    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData;
    }
    return (s32)DefaultOverlayData;
}

/* Contiguous unnamed leaf-owner run for resource_3a6. */
signed int SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablea1dc(void)
{
    return (u8 *)0x0200a1dc;
}

s32 SelectSceneDataByState(void)
{
    extern s16 Data_02000240[];

    s16 state = Data_02000240[224];

    if (state == (s32)&SceneState5D) {
        return (s32)SceneDataA234;
    }
    if (state == (s32)&SceneState5E) {
        return (s32)SceneDataA2C4;
    }
    if (state == (s32)&SceneState5F) {
        return (s32)SceneDataA39C;
    }
    return (s32)SceneDataA234;
}

/* Contiguous unnamed leaf-owner run for resource_3a6. */

/* Configure the 16x15 scene rectangle at row 15. */
void FieldScene_ConfigureRegionAtRow15(void)
{
    Func_02002a44(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void FieldScene_ConfigureRegionAtRow17(void)
{
    Func_02002a60(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void FieldScene_RunInitBracketThenSequence(void)
{
    Func_02002d78();
    Func_02001478();
    Func_02002d88();
    Func_02002050();
}

s32 Func_020010e0(s32 a0)
{

    extern u8 Data_03001ebc[];

    s32 box[3];
    u8 *rec;
    u8 *flag;
    u8 *slot;
    s32 saved;

    rec = (u8 *)Value1(Func_02002da8, 0);
    flag = rec + 85;
    saved = *flag;
    slot = (u8 *)box;
    *(s32 *)(slot + 0) = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Func_02002d90, (s32)rec, (s32)slot) == 0) {
        Func_02002dd0();
        Func_02002d60((s32)rec, 6);
        Func_02002d4e(6);
        Func_02002e94(152);
        Func_02002d74((s32)rec, 7);
        *(s32 *)(rec + 48) = 0x30000;
        *(s32 *)(rec + 52) = 0x20000;
        *(s32 *)(rec + 40) = 0x40000;
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

    extern u8 Data_03001ebc[];

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
        *(s32 *)(record + 40) = 0x80000;
        Call3(Func_02002f10_b, 8, 0x318, 0x118);
        Func_02002ede(20);
        Call3(Func_02002f5a, 8, 0xc000, 20);
        Func_02002ef0_b(30);
        Func_02002f04();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{

    extern u8 Data_03001ebc[];

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
            *(s32 *)(record + 108) = 0x2008cf9;
            Call0((void (*)())Func_02002fa2_b);
        }
    }
}

void FieldScene_RunScene3a6SequenceC(void)
{

    extern u8 Data_03001ebc[];

    s32 rec8;
    s32 record;
    s32 idx;
    s32 tbl;
    s32 idx4;
    s32 off24a;
    u8 *p5;

    p5 = *(u8 **)Data_03001ebc;
    if (Value1(Func_02002f96, 0x302) != 0) {
        off24a = 0x24a;
        if (*(s16 *)((s32)Data_02000240 + off24a) != 8) {
            idx = *(s16 *)(p5 + 0x16c);
            rec8 = Value1(Func_02002fe8, 8);
            record = Value1(Func_02002ff0, 0);
            *(s32 *)(rec8 + 48) = *(s32 *)(record + 48);
            rec8 = Value1(Func_02002ffa, 8);
            record = Value1(Func_02003002, 0);
            *(s32 *)(rec8 + 52) = *(s32 *)(record + 52);
            idx -= 45;
            tbl = 0x02009f00;
            idx <<= 3;
            idx4 = idx + 4;
            Func_02003030(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void FieldScene_RunActor8ZeroStep(void)
{
    Func_02003028();
    Func_02003080(8, 0);
    Func_0200303c();
}

void FieldScene_RunScene3a6_020014ac(void)
{

    extern u8 Data_03001ebc[];

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
    *(s32 *)(record + 68) = zero;
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

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
    extern s16 Data_02000240[];

    extern u8 PrimaryOverlayData_02000d80[];
    extern u8 SecondaryOverlayData_02000d80[];
    extern u8 TertiaryOverlayData_02000d80[];
    extern u8 DefaultOverlayData_02000d80[];

    s16 selector = RuntimeSelectorTable[224];

    if (selector == (s32)&PrimaryRuntimeSelector) {
        return (s32)PrimaryOverlayData_02000d80;
    }
    if (selector == (s32)&SecondaryRuntimeSelector) {
        return (s32)SecondaryOverlayData_02000d80;
    }
    if (selector == (s32)&TertiaryRuntimeSelector) {
        return (s32)TertiaryOverlayData_02000d80;
    }
    return (s32)DefaultOverlayData_02000d80;
}

/*
 * resource_3a6 owner at 0x02001748, complete 40-byte span through its one-word
 * pool: play cue 123, then dispatch the signed scene value at workspace +364.
 */
void SceneAudio_PlayCue123AndDispatchWork364(void)
{
    extern u8 *Data_03001ebc;

    s32 val = *(s16 *)(Data_03001ebc + 364);

    Func_020034b8(123);
    Func_020034a6(val);
}

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */
void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Func_020033f2(0, 34, 13, 3, 23, 34);

    if (Func_02003418(0x301) != 0) {
        Func_020030d2(11, 35, 35);
        Func_02003416(24, 34, 1, 3, 23, 34);
    } else {
        Func_020030ee(11, 23, 35);
        Func_02003434(24, 34, 1, 3, 35, 34);
    }
}

void Func_020017d8(s32 a0, s32 a1, s32 a2)
{
    extern s32 SceneStateWords[];

    Obj *p;
    Obj *q;
    s32 x;
    s32 y;

    p = Func_020034b6_a(SceneStateWords[125]);
    q = Func_020034be(a0);
    Func_020034b4();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003496(p, x, p->f0c, y);
    }
    Func_0200347e(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Func_020034c2(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Func_020034b6_b(q, 4);
    } else {
        Func_020034c0(q, 3);
    }
    Func_020034ee(p);
    Func_0200354a();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void FieldScene_RunShiftAndSetFlag301(void)
{

    extern u8 Data_03001ebc[];

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

    extern u8 Data_03001ebc[];

    Func_02003656(241);
    Func_020030de(11, -112, 0);
    Func_020030ea(11, -80, 0);
    Func_020035b4(0x301);
    Func_0200353a(2);
    Func_02003092();
    Func_02003684(0x121);
}

void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Func_02003602(id);

    if (rec != 0) {
        Func_02003670(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Advance actor eleven through the two presentation states used at scene end. */
void ActorPresentation_AdvanceActorElevenStates(void)
{
    Func_02003900(11, 1);
    Func_02003908(11, 2);
}

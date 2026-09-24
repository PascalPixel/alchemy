#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define CreateOverlayObject Func_02001cf6
#define SetOverlayObjectMode Func_02001d58
#define SetOverlayObjectSlot Func_02001df0
#define RuntimeSelectorTable Data_02000240
#define PrimaryRuntimeSelector Value_0000005d
#define SecondaryRuntimeSelector Value_0000005e
#define TertiaryRuntimeSelector Value_0000005f
#define PrimaryOverlayData Data_02009f9c
#define SecondaryOverlayData Data_0200a014
#define TertiaryOverlayData Data_0200a134
#define DefaultOverlayData Data_02009f6c
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

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
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
Obj *Func_020034b6_a();
Obj *Func_020034b6_b();
Obj *Func_020034be(s32);
void Func_02003496(Obj *, s32, s32, s32);
void Func_020034c2(Obj *, s32, s32, s32);
void Func_020034ee(Obj *);
void Func_02002a16(s32, s32, s32);
u8 *Func_020029bc(s32 no);
s32 Func_0200293e(s32 dz, s32 dx);
void Func_02001478();
void Func_02002050();
s32 Func_02002d90();
s32 Func_02002da8();
s32 Func_02002edc();
s32 Func_02002fa2_a();
s32 Func_02002fe8();
s32 Func_02002ff0();
s32 Func_02002ffa();
s32 Func_02003002();
s32 Func_02001ac2();
void Func_02001ad2();
void Func_0200223c();
s32 Func_020031c6();
s32 Func_020031d2();
s32 Func_0200320e();
void Func_02003224();
s32 Func_0200323e();
void Func_0200309c();
void Func_020030a6();
void Func_0200304e();
void Func_020030de(s32 actorId, s32 cellX, s32 cellZ);
void Func_020030ea(s32 actorId, s32 cellX, s32 cellZ);
void Func_02003092(void);
void Func_020030d2(s32 width, s32 top, s32 bottom);
void Func_020030ee(s32 width, s32 top, s32 bottom);
struct Rec_3a6 *Func_02003602();

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
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 14);
        Object_SetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredB(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "CREATE_CONFIGURED_OVERLAY_OBJECT_BODY.INC"
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
        Task_Wait(1);
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
    s16 state = gGameState.scene;

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
    Map_CopyCells(16, 15, 1, 1, 15, 15);
}

/* Configure the matching 16x15 scene rectangle at row 17. */
void FieldScene_ConfigureRegionAtRow17(void)
{
    Map_CopyCells(16, 17, 1, 1, 15, 15);
}

/*
 * resource_3a6 owner at 0x020010c8, 24 bytes: open the scene scheduler,
 * initialize it, close it, then run the preceding tile-27 rain sequence.
 */
void FieldScene_RunInitBracketThenSequence(void)
{
    Event_Begin();
    Func_02001478();
    Event_End();
    Func_02002050();
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
    *(s32 *)(slot + 0) = (*(s32 *)(rec + 8) & -0x100000) + 0x80000;
    *(s32 *)(slot + 4) = *(s32 *)(rec + 12);
    *(s32 *)(slot + 8) = (*(s32 *)(rec + 16) & -0x100000) + 0x280000;
    if (Value2(Func_02002d90, (s32)rec, (s32)slot) == 0) {
        Event_Begin();
        Object_SetAnimation((s32)rec, 6);
        Task_Wait(6);
        Audio_PlayCue(152);
        Object_SetAnimation((s32)rec, 7);
        *(s32 *)(rec + 48) = 0x30000;
        *(s32 *)(rec + 52) = 0x20000;
        *(s32 *)(rec + 40) = 0x40000;
        *flag = *flag & 126;
        Actor_SetSpriteFlags((s32)rec, 0);
        Value3(Engine_ActorMoveToAndWait, 0, *(s16 *)(slot + 2), *(s16 *)(slot + 10));
        Object_SetAnimation((s32)rec, 6);
        Actor_SetSpriteFlags((s32)rec, 1);
        *flag = (u8)saved;
        ((void (*)())Engine_EventEnd)();
        return 1;
    }
    return 0;
}

void FieldScene_RunScene3a6SequenceA(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) == 0) {
        GameFlag_Set(0x200);
        Event_Begin();
        Camera_SetSpeed(0x10000, 0x2000);
        Camera_FollowActor(8, 1);
        Camera_WaitForMove();
        Event_Wait(60);
        Actor_FaceDirection(8, 0xc000, 20);
        Actor_SetAttachedEffect(8, 0x102);
        Actor_RunRepeatedMotion(8, 2);
        Event_Wait(20);
        Actor_SetSpeed(8, 0x10000, 0x8000);
        Actor_WalkToAndWait(8, 0x318, 248);
        Audio_PlayCue(152);
        record = Func_02002edc(8);
        *(s32 *)(record + 40) = 0x80000;
        Actor_WalkToAndWait(8, 0x318, 0x118);
        Event_Wait(20);
        Actor_FaceDirection(8, 0xc000, 20);
        Event_Wait(30);
        Event_End();
    }
}

void FieldScene_RunScene3a6SequenceB(void)
{

    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x200) != 0) {
        if (GameFlag_IsSet(0x201) == 0) {
            GameFlag_Set(0x201);
            GameFlag_Set(0x302);
            Event_Begin();
            Actor_SetAttachedEffect(8, 0x102);
            Actor_RunRepeatedMotion(8, 2);
            Event_Wait(20);
            Actor_SetSpeed(8, 0x20000, 0x10000);
            Actor_WalkToAndWait(8, 0x2f8, 0x118);
            Actor_WalkToAndWait(8, 0x2f8, 0x138);
            Actor_WalkToAndWait(8, 0x318, 0x138);
            Event_Wait(10);
            Actor_FaceDirection(8, 0xc000, 20);
            record = Func_02002fa2_a(8);
            *(s32 *)(record + 108) = 0x2008cf9;
            Call0((void (*)())Engine_EventEnd);
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
    if (GameFlag_IsSet(0x302) != 0) {
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
            Actor_WalkTo(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
        }
    }
}

/* Actor-8 presentation reset at 0x02001378, including alignment to 0x1390. */
void FieldScene_RunActor8ZeroStep(void)
{
    Event_Begin();
    Actor_SetAnimation(8, 0);
    Event_End();
}

void FieldScene_RunScene3a6_020014ac(void)
{

    u32 i;
    s32 record;
    s32 zero;

    Event_Begin();
    Event_Wait(10);
    Actor_SetSpeed(0, 0x8000, 0x1999);
    Actor_SetAnimation(0, 8);
    Event_Wait(15);
    Actor_SetDestinationOffset(0, 8, 0);
    Event_Wait(4);
    Audio_PlayCue(0x120);
    Audio_PlayCue(239);
    Actor_SetSpeed(9, 0x8000, 0x1999);
    Actor_SetAnimation(9, 2);
    zero = 0;
    *(u8 *)(Func_020031c6(9) + 85) = zero;
    record = Func_020031d2(9);
    *(s32 *)(record + 68) = zero;
    Actor_SetDestinationOffset(9, 12, 0);
    Actor_WaitForMove(0);
    Actor_SetAnimation(0, 1);
    Actor_WaitForMove(9);
    Audio_PlayCue(0x120);
    Audio_PlayCue(213);
    Actor_SetAnimation(9, 3);
    *(u8 *)(Func_0200320e(9) + 85) = 3;
    Actor_SetDestinationOffset(9, 6, 0);
    Func_02003224(9);
    Func_0200223c();
    Actor_SetAnimation(9, 8);
    Actor_SetSpritePriority(9, 3);
    *(u8 *)(Func_0200323e(9) + 35) = 2;
    Value6(Func_02001ac2, 0, 12, 16, 1, 4, 0);
    Call6(Func_02001ad2, 0, 13, 16, 1, 4, 0);
    GameFlag_Set(0x202);
    Audio_PlayCue(240);
    Event_End();
}

s32 SceneData_SelectSecondaryByRuntimeSelector(void)
{
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
    s32 val = gEventWork->touched_trigger;

    Audio_PlayCue(123);
    Event_RequestExit(val);
}

/*
 * resource_3a6 owner at 0x02001770, complete 104-byte span through its
 * one-word pool. It installs the common window rectangle, then selects one of
 * two row layouts from story flag 0x301.
 */
void DialogueLayout_ConfigureRowsByFlag301(void)
{
    Map_CopyCellAttributes(0, 34, 13, 3, 23, 34);

    if (GameFlag_IsSet(0x301) != 0) {
        Func_020030d2(11, 35, 35);
        Map_CopyCellAttributes(24, 34, 1, 3, 23, 34);
    } else {
        Func_020030ee(11, 23, 35);
        Map_CopyCellAttributes(24, 34, 1, 3, 35, 34);
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
    Event_Begin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Func_02003496(p, x, p->f0c, y);
    }
    Object_SetAnimation(p, 27);
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
        Object_SetAnimation(q, 3);
    }
    Func_020034ee(p);
    Event_End();
}

/*
 * The 54-byte owner at 0x020018b4 includes its two pool words: 0x301 and
 * 0x121 are identifiers passed as arguments, never dereferenced as
 * addresses.  0x301 is this scene's event flag.  The two shift calls carry
 * a displacement and its opposite, not two unrelated magnitudes.
 */
void FieldScene_RunShiftAndSetFlag301(void)
{

    Audio_PlayCue(241);
    Func_0200309c(11, 112, 0);
    Func_020030a6(11, 80, 0);
    GameFlag_Set(0x301);
    Task_Wait(2);
    Func_0200304e();
    Audio_PlayCue(0x121);
}

void FieldScene_RunActor11Transition301(void)
{

    Audio_PlayCue(241);
    Func_020030de(11, -112, 0);
    Func_020030ea(11, -80, 0);
    GameFlag_Clear(0x301);
    Task_Wait(2);
    Func_02003092();
    Audio_PlayCue(0x121);
}

void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y)
{
    struct Rec_3a6 *rec = Func_02003602(id);

    if (rec != 0) {
        Actor_SetSpritePriority(id, 3);
        rec->f34 = 2;
        rec->f35 |= 2;
        rec->f8 = (x << 20) + 0x80000;
        rec->f16 = (y << 20) + 0x80000;
    }
}

/* Advance actor eleven through the two presentation states used at scene end. */
void ActorPresentation_AdvanceActorElevenStates(void)
{
    Actor_SetAnimation(11, 1);
    Actor_SetAnimation(11, 2);
}

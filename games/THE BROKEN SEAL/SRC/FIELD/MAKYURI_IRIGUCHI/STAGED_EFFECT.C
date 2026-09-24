#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_CreateConfigured      Func_02000048
#define CreateOverlayObject Func_020024fe
#define SetOverlayObjectMode Func_02002568
#define SetOverlayObjectSlot Func_02002640
#define F(base, type, off) (*(type *)((u8 *)(base) + (off)))
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"
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

void Func_02004038();
u8 **Func_020047ec(s32, s32);
void Func_02000e6a();
void Func_020011e8();
s32 Func_020034cc();
void Func_020038cc();
u16 Func_0200317c_a(s32 deltaZ, s32 deltaX);
void Func_020031d4(void);
void Func_02002eac(void);
void Func_02002e58(void);
void Func_02002d50(void);
void Func_02001e82(int page);
void Func_02001e8e(int page);
void Func_02001e9a(int page);

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

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_CreateConfigured(s32 first, s32 second, s32 third, s32 fourth)
{
    void *obj;
    void *rec;
    s32 mask;

    obj = Object_Create(fourth, first, second, third);
    if (obj != NULL) {
        rec = FIELD_AT_OFFSET(obj, void *, 0x50);
        mask = -0xD;
        FIELD_AT_OFFSET(rec, u8, 9) = (u8)(mask & FIELD_AT_OFFSET(rec, u8, 9));
        FIELD_AT_OFFSET(obj, u8, 0x55) = 0;
        FIELD_AT_OFFSET(obj, u8, 0x59) = 8;
        Actor_SetSpriteFlags(obj, 0);
        Object_SetPalette(obj, 0xE);
        Object_SetBlendMode(obj, 1);
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
    value = Random_Next();
    magic = -(s32)(((u32)(value << 3) >> 16) * 0x3333);
    Func_02000e6a((*(s32 *)(a0 + 8) + ((8 - (Data_03001e40 & 15)) << 16)), (*(s32 *)(a0 + 12) + 0x1a0000), *(s32 *)(a0 + 16), 0, magic, 0, 0xb0000, rec);
    return 0;
}

/* Play the footprint-motion completion cue. */
s32 SceneAudio_PlayCue118AndReturnZero(void)
{
    Audio_PlayCue(118);
    return 0;
}

/* Keep this object facing actor 0 while the actor remains near ground level. */
s32 SceneActor_FaceLeaderWhileGrounded(u8 *object)
{
    u8 *leader = Actor_Get(0);

    if ((*(s32 *)(leader + 16) >> 19) <= 22) {
        *(u16 *)(object + 6) = Func_0200317c_a(
            *(s32 *)(leader + 16) - *(s32 *)(object + 16),
            *(s32 *)(leader + 8) - *(s32 *)(object + 8));
    } else if (*(u16 *)(object + 6) != 0xc000) {
        Actor_FaceDirection(3, 0xc000, 0);
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
    Event_Begin();
    Actor_SetAnimation(0, 1);
    Message_ShowCentered(0x1632, 1);
    Event_End();
}

void Func_02000ed0(s32 a)
{
    u8 *v = Actor_Get(0);
    Event_Begin();
    Audio_PlayCue(0xe4);
    F(v, s32, 0x6c) = (s32)&Value_02008cc1;
    F(v, s32, 0x30) = 0x3333;
    Actor_SetAnimation(0, 2);
    Actor_SetDestinationOffset(0, 0, -6);
    Actor_WaitForMove(0);
    Actor_SetChildValue(0, 15);
    Actor_SetSpriteFlags(Actor_Get(0), 0);
    F(v, s32, 0x6c) = 0;
    Event_Wait(30);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(a);
    Event_End();
}

void FieldScene_RunScene39b_02000f48(s32 a0)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(228);
    Actor_SetSpeed(0, 0x6666, 0x3333);
    Actor_SetSpritePriority(0, 2);
    Actor_SetDestinationOffset(0, 0, -8);
    record = Actor_Get(0);
    Actor_SetSpriteFlags(record, 0);
    Event_Wait(8);
    Actor_SetPosition(0, ((a0 << 19) + 0x80000), 0);
    Event_Wait(30);
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

    a = *(s32 *)(Value1(Engine_ActorGet, 0) + 8) / 0x100000;
    b = *(s32 *)(Value1(Engine_ActorGet, 0) + 16) / 0x100000;
    if (a == 12 && b == 32) {
        Event_Begin();
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        ColorBuffer_ApplyTarget(0x10005, 1);
        ColorBuffer_Interpolate(60);
        Event_Wait(40);
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
            Audio_PlayCue(246);
            x = 208 - ((u32)(Random_Next() << 4) >> 16);
            y = 560 - ((u32)(Random_Next() << 4) >> 16);
            t = ((u32)(Value0(Engine_RandomNext) << 2) >> 16);
            record = Math_Divide((((t << 4) - t) << 16) + 0x3c0000, 100);
            Func_020011e8(x << 16, 0, y << 16, 0, record, zero, 0x320001, slot);
            Event_Wait(4);
            counter = counter + 1;
        } while ((u32)counter <= 14);
        Audio_PlayCue(220);
        Event_Wait(60);
        GameFlag_Set(0x875);
        Value2(Func_020034cc, 0x2008d99, 0xc80);
        Map_CopyCellsTo(37, 98, 10, 97, 5, 3);
        Map_CopyCellAttributes(70, 32, 13, 7, 6, 32);
        ColorBuffer_ApplyTarget(0x10000, 0);
        ColorBuffer_Interpolate(60);
        Event_Wait(120);
        Event_End();
    }
}

void FieldScene_RunIndexedStep63(void)
{
    Event_RequestExit(63);
}

void FieldScene_RunActor8StepWithTableA820(void)
{
    GameFlag_Clear(0x205);
    Engine_ActorEnableActionCallback(8, (void *)0x0200a820);
}

void FieldScene_RunScene39b_0200116c(void)
{
    s32 record;
    s32 field8;
    s32 quotient;

    record = Value1_0200116c(Engine_ActorGet, 0);
    field8 = *(s32 *)(record + 8);
    quotient = field8 / 0x100000;
    GameFlag_Set(0x205);
    if (quotient == 7) {
        Engine_ActorEnableActionCallback(8, 0x200a874);
    } else {
        Engine_ActorEnableActionCallback(8, 0x200a8c8);
    }
}

void FieldScene_RunScene39b_02001208(void)
{
    struct FieldActor *actor;
    s32 flag;
    s32 record;

    actor = (struct FieldActor *)Value1_02001208(Engine_ActorGet, 0);
    flag = GameFlag_IsSet(0x109);
    if (flag == 0) {
        Event_Begin();
        Camera_MoveTo(-1, -1, -1, 0);
        actor->motion_flags = 0;
        Value3(Engine_ActorSetPosition, 0, actor->x.part.pixel << 16, (actor->z.part.pixel << 16) + -0x100000);
        Actor_SetChildValue(0, 15);
        record = Actor_Get(0);
        Actor_SetSpriteFlags(record, 0);
        Event_OpenScreen();
        Event_WaitForScreen();
        Audio_PlayCue(228);
        actor->update = (void (*)(union FieldObject *))FieldScene_RunScene39b_02000cc0;
        Actor_SetSpeed(0, 0x6666, 0x3333);
        Actor_WalkByAndWait(0, 0, 8);
        Actor_SetChildValue(0, 0);
        record = Actor_Get(0);
        Actor_SetSpriteFlags(record, 1);
        actor->sprite->priority = 1;
        Actor_WalkByAndWait(0, 0, 10);
        actor->motion_flags = 3;
        actor->update = NULL;
        Func_020038cc();
        Event_End();
    }
}

void FieldScene_RunScene39b_0200196c(void)
{
    s32 record;

    if (GameFlag_IsSet(0x250) == 0) {
        GameFlag_Set(0x250);
        Event_Begin();
        record = Actor_Get(12);
        *(s32 *)(record + 24) = -0x10000;
        record = Value1(Engine_ActorGet, 13);
        *(s32 *)(record + 24) = -0x10000;
        record = Actor_Get(14);
        *(s32 *)(record + 24) = -0x10000;
        Actor_SetPosition(3, 0x880000, 0x900000);
        Actor_FaceDirection(3, 0x4000, 10);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
        Event_OpenScreen();
        Event_WaitForScreen();
        Event_Wait(60);
        Actor_FaceActor(3, 0, 0);
        Actor_SetAnimationAndWait(3, 3);
        Event_Wait(30);
        Actor_WalkTo(3, 136, 72);
        Event_Wait(40);
        Actor_RunRepeatedMotion(0, 1);
        Actor_WaitForMove(3);
        Actor_SetPosition(3, 0, 0);
        GameFlag_Set(0x872);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
        Event_End();
    }
}

/* Spawn and configure the compact companion object at a source position. */
void OverlayObject_SpawnKind24AtActor(u8 *src)
{
    u8 *obj = Object_Create(24, *(int *)(src + 8),
                              *(int *)(src + 12), *(int *)(src + 16));
    if (obj != 0) {
        u8 *rec = *(u8 **)(obj + 80);
        Object_SetScript(obj, (void *)0x0200a7b8);
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

    seed = Math_Sin(step << 10);
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
 * 72-byte owner includes its three pool words.  Engine_ObjectSetScript and
 * Engine_ObjectSetAnimation are one import called twice with very different second
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
    GameFlag_Clear(0x161);

    target = (s32 *)record[5];
    if (target != 0) {
        *(short *)((u8 *)target + 0x64) = 0;
        Engine_ObjectSetScript(target, (s32)0x0200a7dc);
        Object_SetAnimation(target, 7);
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
    Engine_ObjectDispatchRelease(obj);
    *(u8 **)(state + 20) = 0;
}

#include "TYPES.H"
#include "FIELD_EVENT.H"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
#define OverlayObject_PrepareObject      Func_02000048
#define CreateOverlayObject Func_02001166
#define SetOverlayObjectMode Func_020011a0
#define SetOverlayObjectSlot Func_020012a0

#include "CREATE_CONFIGURED_OVERLAY_OBJECT.H"

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

struct OverlayActorPosition {
    u8 pad00[8];
    s32 depth_fixed;
};

struct OverlayActorState {
    u8 pad00[35];
    u8 flags;
};

extern u8 Data_03001ebc[];

struct OverlayActorPosition *Func_02001e54_a();
void Func_02001af6();
void Func_020016d8();
void Func_020016c2();
void Func_020018a6();
void Func_02001b90();
void Func_02001bc8();
void Func_02001d32_a();
u8 *Func_0200207c(s32 group);
s32 Func_02002096(s32 group, s32 value);
void Func_020020b2(s32 group, s32 index);

/* Wait at most sixty frames for the object to reach the requested height. */

/*
 * resource_387: issue a scene request and then wait.
 */

/* Old-style: the two imports are called with different arities. */

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

/* resource_387 three-call story-flag wrapper at 0x020004d4. */

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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void SetEffectRecordMode(struct EffectWork *work, s32 mode)
{
    work->record->mode = mode;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
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
 * The owner exists for the argument shuffle: the frame count is saved before
 * the first call clobbers its register, so it survives to reach the second.
 * The twenty-two byte owner loads no literal and has no pool.
 */
void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Event_ShowMessage(selector, 0);
    Event_Wait(frames);
}

/* Contiguous unnamed leaf-owner run for resource_387. */

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable92f8(void)
{
    return (void *)0x020092f8;
}

/* resource_387 zero-return leaf at 0x02000334. */
int SceneData_ReturnZero(void)
{
    return 0;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9358(void)
{
    return (void *)0x02009358;
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9368(void)
{
    return (void *)0x02009368;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;
    s32 v3;

    record = Value1(Engine_ActorGet, 9);
    v3 = *(s32 *)(record + 8) / 0x100000;
    GameFlag_Clear(0x861);
    GameFlag_Clear(0x862);
    if (v3 == 15) {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Map_CopyCellAttributes(48, 18, 1, 2, v3, 18);
        GameFlag_Set(0x861);
    } else {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
        GameFlag_Set(0x862);
    }
}

void FieldScene_RunScene387SequenceC(void)
{
    struct FieldActor *actor;
    s32 tile_x;

    actor = (struct FieldActor *)Value1(Engine_ActorGet, 10);
    tile_x = actor->x.fixed / 0x100000;
    if (tile_x == 23) {
        Event_Wait(10);
        Actor_Get(10)->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        Actor_Get(10)->motion_flags = 0;
        Actor_SetSpriteFlags(Actor_Get(10), 0);
        Map_CopyCellAttributes(54, 17, 1, 1, tile_x, 17);
        GameFlag_Set(0x863);
    }
}

void FieldScene_RunScene387SequenceD(void)
{
    u8 *p5;
    s32 v5;

    p5 = *(u8 **)Data_03001ebc;
    Event_Begin();
    Actor_SetAnimation(0, 8);
    Event_Wait(20);
    Actor_SetSpeed(0, 0x3333, 0x1999);
    Actor_SetSpeed(9, 0x3333, 0x1999);
    Audio_PlayCue(185);
    v5 = (11 - (*(s16 *)(p5 + 0x16c) << 1)) << 4;
    Actor_SetDestinationOffset(0, v5, 0);
    Actor_SetDestinationOffset(9, v5, 0);
    Actor_WaitForMove(0);
    Actor_WaitForMove(9);
    Event_Wait(20);
    Actor_SetAnimation(0, 1);
    FieldScene_RunOpeningAuxiliarySequence();
    Func_020016d8();
    Event_End();
}

/* resource_387 deliberate no-op leaf at 0x020004cc. */
void Resource387_NoOpCallbackA(void)
{
}

/* resource_387 deliberate no-op leaf at 0x020004d0. */
void Resource387_NoOpCallbackB(void)
{
}

void FieldScene_RunStepWithValue866(void)
{
    Event_Begin();
    GameFlag_Set(0x866);
    Event_End();
}

/* resource_387 prologue-less table getter, including its one-word pool. */
void *SceneData_GetTable9488(void)
{
    return (void *)0x02009488;
}

void FieldScene_RunScene387SequenceA(void)
{
    u32 i;
    s32 record;

    Func_020018a6();
    Event_Begin();
    Event_Wait(30);
    Event_SetMessage(0x138f);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_SetSpeed(1, 0xcccc, 0x6666);
    Actor_FaceDirection(0, 0xe000, 20);
    record = Value1(Engine_ActorGet, 0);
    if (record != 0) {
        Actor_SetPosition(1, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_WalkToAndWait(1, 0x108, 168);
    Actor_FaceDirection(1, 0x6000, 20);
    Event_Wait(20);
    Actor_SetAnimationAndWait(1, 4);
    Event_Wait(20);
    Event_ShowMessageAndWait(1, 0, 20);
    Actor_SetAnimationAndWait(0, 3);
    Event_Wait(20);
    if (GameFlag_IsSet(0x855) == 0) {
        Actor_SetAnimation(1, 2);
        record = Value1(Engine_ActorGet, 0);
        if (record != 0) {
            Actor_SetDestination(1, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(1);
        Actor_SetPosition(1, 0, 0);
        Event_End();
    } else {
        Actor_SetPosition(2, 0x1680000, 0xf80000);
        Actor_SetSpeed(2, 0xcccc, 0x6666);
        Actor_WalkToAndWait(2, 0x110, 248);
        Actor_WalkToAndWait(2, 0x110, 208);
        Actor_FaceDirection(2, 0xa000, 20);
        Actor_RunRepeatedMotion(2, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(2, 0, 20);
        Actor_FaceDirection(0, 0x2000, 0);
        Actor_FaceDirection(1, 0x4000, 0);
        Event_Wait(20);
        Actor_ShowEmote(0, 0x100, 0);
        Actor_ShowEmote(1, 0x100, 60);
        Actor_SetAnimationAndWait(2, 3);
        Actor_WalkToAndWait(2, 0x108, 200);
        Actor_WalkTo(0, 248, 168);
        Actor_WalkToAndWait(2, 248, 184);
        Actor_WaitForMove(0);
        Actor_FaceDirection(0, 0x6000, 0);
        Actor_FaceDirection(1, 0x6000, 0);
        Actor_WalkToAndWait(2, 232, 184);
        Event_Wait(20);
        Actor_ShowEmote(2, 0x105, 60);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_SetAnimationAndWait(2, 4);
        Event_Wait(20);
        Event_ShowMessageAndWait(2, 0, 20);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Actor_FaceDirection(2, 0x8000, 20);
        Event_ShowMessageAndWait(2, 0, 120);
        Actor_ShowEmote(0, 0x105, 0);
        Actor_ShowEmote(1, 0x105, 60);
        Actor_FaceEachOther(0, 1, 0);
        Event_Wait(60);
        Actor_FaceDirection(0, 0x6000, 0);
        Actor_FaceDirection(1, 0x6000, 0);
        Event_Wait(60);
        Actor_ShowEmote(2, 0x106, 0);
        Actor_StartRepeatedMotion(2, 1);
        Event_Wait(30);
        Event_ShowMessageAndWait(2, 0, 30);
        Actor_FaceDirection(2, 0xe000, 20);
        Actor_StartRepeatedMotion(0, 2);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(20);
        Event_ShowMessageAndWait(2, 0, 20);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(20);
        Actor_FaceDirection(2, 0x8000, 20);
        Func_020016c2();
        Actor_SetAnimation(2, 1);
        Event_Wait(20);
        Func_02001b90();
        Actor_ShowEmote(0, 0x100, 0);
        Actor_ShowEmote(1, 0x100, 60);
        Actor_Jump(1, 2, 0);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_WalkToAndWait(2, 0x108, 184);
        Event_Wait(10);
        Actor_FaceActor(2, 1, 0);
        Actor_FaceActor(1, 2, 0);
        Actor_FaceActor(0, 2, 0);
        Event_Wait(20);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 60);
        Actor_ShowEmote(0, 0x105, 0);
        Actor_ShowEmote(1, 0x105, 60);
        Actor_ShowEmote(0, 0x101, 0);
        Actor_ShowEmote(1, 0x101, 0);
        Event_Wait(60);
        Actor_FaceDirection(1, 0x8000, 0);
        Actor_FaceDirection(0, 0, 0);
        Event_Wait(60);
        Actor_FaceDirection(1, 0x4000, 0);
        Actor_FaceDirection(0, 0x2000, 0);
        Event_Wait(10);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_ShowEmote(2, 0x105, 0);
        Event_Wait(60);
        Actor_SetAnimationAndWait(2, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceActor(0, 2, 0);
        Actor_FaceActor(1, 2, 0);
        Actor_ShowEmote(0, 0x102, 0);
        Actor_ShowEmote(1, 0x102, 0);
        Event_Wait(60);
        Actor_RunRepeatedMotion(2, 2);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_ShowEmote(0, 0x101, 0);
        Actor_ShowEmote(1, 0x101, 0);
        Event_Wait(80);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_StartRepeatedMotion(0, 1);
        Actor_StartRepeatedMotion(1, 1);
        Actor_SetAttachedEffect(0, 0x102);
        Actor_SetAttachedEffect(1, 0x102);
        Event_Wait(60);
        Actor_SetAnimationAndWait(2, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(0, 0, 0);
        Actor_FaceDirection(1, 0x8000, 0);
        Event_Wait(80);
        Actor_FaceActor(0, 2, 0);
        Actor_FaceActor(1, 2, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_SetAnimationAndWait(2, 4);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_StartRepeatedMotion(0, 2);
        Actor_RunRepeatedMotion(1, 2);
        Event_Wait(20);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 40);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimationAndWait(1, 3);
        Event_Wait(20);
        Func_02001d32_a(2, 1);
        Event_Wait(60);
        Func_02001bc8();
        Actor_RunRepeatedMotion(2, 1);
        Event_Wait(20);
        Actor_WalkToAndWait(2, 248, 184);
        Event_Wait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(0, 0x8000, 0);
        Actor_FaceDirection(1, 0x8000, 0);
        Event_Wait(120);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_FaceActor(0, 2, 0);
        Actor_FaceActor(1, 2, 0);
        Actor_FaceActor(2, 0, 0);
        Event_Wait(20);
        Actor_SetAnimation(0, 3);
        Actor_SetAnimation(1, 3);
        Actor_SetAnimationAndWait(2, 3);
        Event_Wait(50);
        Actor_SetSpeed(1, 0xcccc, 0x6666);
        Actor_SetSpeed(2, 0xcccc, 0x6666);
        Actor_WalkTo(1, 248, 168);
        Actor_WalkToAndWait(2, 248, 168);
        Actor_SetPosition(2, 0, 0);
        Actor_WaitForMove(1);
        Actor_SetPosition(1, 0, 0);
        Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
        GameFlag_Set(0x865);
        Event_End();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Event_Begin();
    depth = Func_02001e54_a(8)->depth_fixed >> 20;
    if (depth == 11) {
        Func_02001af6(8);
        state = Actor_Get(8);
        state->flags |= 2;
        span = 12;
        Map_CopyCellAttributes(39, 12, 3, 1, 8, span);
        Map_CopyCellAttributes(43, 11, 3, 1, span, depth);
        GameFlag_Set(2144);
    }
    Event_End();
}

/* Turn the object's attached presentation state by one eighth-turn. */
void OverlayObject_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

void OverlayObject_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Task_Wait(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}

/* Apply a value to every matching member of a fifteen-slot group. */
void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = Func_0200207c(group);
    s32 i;
    Func_02002096(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Func_020020b2(group, i);
    }
}

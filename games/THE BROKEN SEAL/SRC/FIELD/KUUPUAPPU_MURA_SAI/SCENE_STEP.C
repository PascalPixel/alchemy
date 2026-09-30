/*
 * Scene steps: actor 16's question, actor 21's arrival and the leader's
 * arrival in scenes five to ten.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuGrownUpsAlways[];
extern u8 MsgKuupuappuGuysCheckJail[];
extern u8 MsgKuupuappuNotLikeEasy[];

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

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

union MotionWork {
  struct {
    u32 unk_00[2];
    s32 x, y, z;
    u32 unk_14;
    s32 accum_x, accum_y;
    u32 unk_20[4];
    s32 rate_x, rate_y;
    u32 unk_38[3];
    s32 velocity_x, velocity_y, velocity_z;
    u16 *record;
    u8 unk_54[16];
    u16 angle_step;
  } fields;
  u8 bytes[102];
};

struct SceneActor {
    u8 unk_00[6];
    u16 facing;
    u8 unk_08[92];
    u16 state_flags;
};

struct SceneActor_02000350 {
    u8 unk_00[6];
    u16 facing;
    s32 x, y, z;
    u8 unk_14[71];
    u8 active;
};

/* Complete actor-13 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_020007d4 {
    u8 reserved00[91];
    u8 accepted;
};

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_020004b4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct Presentation_02000c1c {
    u8 unk_00[9];
    u8 flags;
};

struct SceneActor_02000c1c {
    u8 unk_00[35];
    u8 state_23;
    u8 unk_24[44];
    struct Presentation_02000c1c *presentation;
};

/*
 * Complete actor-18 dialogue/restoration scene.  If cue 231 remains available
 * and its movement scene has not set flag 0x858, the shared scene marker at
 * +370 is enabled before the dialogue scene closes.
 */
struct SceneWork_02000e90 {
    u8 reserved000[370];
    u16 actor18_marker;
};

/* Complete actor-16 conditional-counter dialogue scene through its pool. */
struct SceneWork_020006b4 {
    u8 reserved000[472];
    u16 branch_counter;
};

/* Complete actor-11 temporary-acceptance dialogue wrapper through its pool. */
struct Actor_02000754 {
    u8 reserved00[91];
    u8 accepted;
};

/* Complete actor-15 facing-preserving dialogue scene through its two-word pool. */
struct Actor_02000640 {
    u8 reserved00[6];
    u16 facing;
    u8 reserved08[92];
    u16 state_flags;
};

extern u8 *gWork;

/* The scene's tables, laid out after the code. */
extern const u8 KuupuappuMuraSai_Scripts[];
extern const u8 KuupuappuMuraSai_Messages[];
extern const u8 KuupuappuMuraSai_Actors[];
extern const u8 KuupuappuMuraSai_Extras[];

/* Map cell steps played as the leader arrives in each scene. */
extern const u16 KuupuappuMuraSai_Scene5Cells[];
extern const u16 KuupuappuMuraSai_Scene6Cells[];
extern const u16 KuupuappuMuraSai_Scene7Cells[];
extern const u16 KuupuappuMuraSai_Scene8Cells[];
extern const u16 KuupuappuMuraSai_Scene9Cells[];
extern const u16 KuupuappuMuraSai_Scene10Cells[];
extern const u16 KuupuappuMuraSai_Scene12Cells[];

s32 SceneActor_GetPositionDistance(s32 *, s32 *);
u32 ArcTan2(s32, s32);
s32 PartyInventory_FindOwner(s32 item);
void ActorPresentation_MoveActorToPositionAndWait();
void BattleFx_RunPageEffectForSlot(s32 actor, s32 mode, s32 value);
void PartyInventory_Discard();
s32 UpdateActorProximity(u8 *actor);
void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay);

static __inline__ s32 Scene_QueryFlag(s32 (*func)(s32), s32 flag)
{
    return func(flag);
}

static __inline__ void Scene_SetFlag(void (*func)(s32), s32 flag)
{
    func(flag);
}

static __inline__ void Scene_Call3(void (*func)(s32, s32, s32), s32 a, s32 b, s32 c)
{
    func(a, b, c);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call0(void (*f)())
{
    f();
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

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

static __inline__ void PlaceActor(void (*place)(s32, s32, s32),
                                 s32 actor, s32 x, s32 z)
{

    place(actor, x, z);
}

static __inline__ void UpdateRect(void (*update)(s32, s32, s32, s32, s32, s32),
                                 s32 x, s32 z, s32 width, s32 height,
                                 s32 sourceX, s32 sourceZ)
{

    update(x, z, width, height, sourceX, sourceZ);
}

void SceneDialogue_RunActorFifteenDialogue(void)
{
    {
        struct SceneActor *actor = Actor_Get(15);
        actor->state_flags |= 2;
    }
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuGrownUpsAlways);
    ActorPresentation_RunActorModeOneThenZero(15);
    Event_End();
    {
        s32 clear = 0;
        struct SceneActor *actor = Actor_Get(15);
        actor->state_flags = clear;
    }
}

void FieldScene_RunScene385SequenceA(void)
{
    u32 i;
    s32 rec7;
    s32 record;
    s32 v5;

    rec7 = GameFlag_IsSet(0x308);
    if (rec7 == 0) {
        Event_Begin();
        *((u8 *)Actor_Get(16) + 91) = 1;
        Actor_SetAnimation(16, 1);
        Call2((void (*)())Engine_ActorRunRepeatedMotion, 16, 1);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKuupuappuGuysCheckJail);
        Call3((void (*)())Engine_ActorFaceEachOther, 16, 0, 2);
        Event_OpenMessage(16, 0);
        if (Event_ChooseYesNo(0, 0) != 0) {
            bump_step(1);
        }
        Event_ShowMessage(16, 0);
        *((u8 *)Actor_Get(16) + 91) = rec7;
        Actor_EnableActionCallback(16, 2);
        Event_End();
        GameFlag_Set(0x308);
    } else {
        Event_SetMessage((s32)MsgKuupuappuNotLikeEasy);
        *((u8 *)Actor_Get(16) + 91) = 1;
        ActorPresentation_RunActorModeOneThenZero(16);
        v5 = 0;
        *((u8 *)Actor_Get(16) + 91) = v5;
    }
}

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    u8 *record = Actor_Get(actor); int frames;
    Actor_SetSpeed(actor, 0x30000, 0x18000); *(s32 *)(record + 72) = 0x8000;
    *(s32 *)(record + 68) = 0; *(s32 *)(record + 40) = field40; Actor_SetSpriteFlags(record, 0);
    Actor_MoveToAndWait(actor, x, z); Actor_SetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Task_Wait(1); if (*(s16 *)(record + 42) == 0) break; }
    Actor_SetSpriteFlags(record, 1); *(s32 *)(record + 72) = 0x10000;
}

void FieldScene_RunActor21SequenceOnce(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Audio_PlayCue(100);
    Event_Wait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(21, 0x102);
        Actor_Jump(21, 4, 0);
        Event_Wait(12);
        Actor_Jump(21, 4, 0);
        Event_Wait(20);
        Call4(ActorPresentation_MoveActorToPositionAndWait, 21, 0x188, 104, 0x70000);
        Event_Wait(20);
        Actor_WalkToAndWait(21, 0x198, 104);
        Actor_WalkToAndWait(21, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Event_End();
}

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 delay)
{
    s32 zero = 0;

    SetScale(zero, 0x8000, 0x4000);
    Actor_WalkTo(zero, x, y);
    gEventWork->transition_frames = 16;
    Event_RequestExit(delay);
}

void FieldScene_SetupScene5(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene5Cells, 56, 19);
    SceneActor_PlaceAndSetSceneDelay(408, 320, 5);
}

void ActorPresentation_SetupActorEighteenAt312_304(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene6Cells, 50, 18);
    SceneActor_PlaceAndSetSceneDelay(312, 304, 6);
}

void FieldScene_SetupScene7(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene7Cells, 44, 17);
    SceneActor_PlaceAndSetSceneDelay(216, 288, 7);
}

void ActorPresentation_SetupActorZeroForSceneEight(void)
{
    struct SceneActor_020004b4 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene8Cells, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNine(void)
{
    struct SceneActor_020004b4 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene9Cells, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10(void)
{
    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMuraSai_Scene10Cells, 38, 6);
    SceneActor_PlaceAndSetSceneDelay(120, 144, 10);
}

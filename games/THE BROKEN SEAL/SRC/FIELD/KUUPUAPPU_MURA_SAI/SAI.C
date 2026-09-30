#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuRumorsThievesLunpa[];
extern u8 MsgKuupuappuTalkingMayorStrong[];
extern u8 MsgKuupuappuVolcanoStaysCalm[];
extern u8 MsgKuupuappuWarriorsWhoCaptured[];
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
extern u8 *Data_03001e8c[];

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void SetScale(s32 actor, s32 horizontal, s32 vertical)
{
    Actor_SetSpeed(actor, horizontal, vertical);
}

extern u8 MsgKuupuappuWarriorGuy[];

extern u8 MsgKuupuappuGreatEverythingSolved[];
extern u8 MsgKuupuappuGuysCheckJail[];
extern u8 MsgKuupuappuOnceDodonpaTook[];
extern u8 MsgKuupuappuWishVaultsElders[];

extern u8 MsgKuupuappuWatchingGuysMakes[];
void ActorPresentation_RunActorModeOneThenZero(s32 actor);
void SceneDialogue_RunActorFourteenFlagDialogue(void);

extern u8 MsgKuupuappuGrownUpsAlways[];
extern u8 MsgKuupuappuNotLikeEasy[];

/*
 * Actor proximity, the scene tables and the first villagers' lines.
 */
s32 SceneActor_GetPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_UpdateProximity(struct SceneActor_02000350 *actor, struct SceneActor_02000350 *target,
                  s32 range, s32 force)
{
    s32 result = 0;
    s32 *targetPos = &target->x;
    s32 *actorPos = &actor->x;

    if (SceneActor_GetPositionDistance(targetPos, actorPos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z - actor->z,
                                      *targetPos - *actorPos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;

        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->active = 1;
            Object_SetAnimation(actor, 1);
            result = 1;
        }
    } else {
        actor->active = 0;
        Object_SetAnimation(actor, 2);
    }
    return result;
}

s32 UpdateActorProximity(u8 *actor)
{
    u8 **globals = Data_03001e8c;
    u8 *scene = globals[0];
    u8 *work = globals[12];
    u16 *flags = (u16 *)(actor + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    if ((*flags & 1) != 0) {
        partner = Actor_Get(15);
    } else {
        partner = Actor_Get(14);
    }
    if (SceneActor_UpdateProximity(actor, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_Get(ACTOR_PARTY_LEADER);

    if (*(s16 *)(work + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_UpdateProximity(actor, player, range, force);
    return 0;
}

const void *SceneData_GetScriptTable(void)
{
    return KuupuappuMuraSai_Scripts;
}

/* Complete zero-return leaf; no calls and no argument read. */
int SceneData_ReturnZero(void)
{
    return 0;
}

const void *SceneData_GetMessageTable(void)
{
    return KuupuappuMuraSai_Messages;
}

const void *SceneData_GetActorTable(void)
{
    return KuupuappuMuraSai_Actors;
}

void ActorPresentation_RunActorModeOneThenZero(s32 actor)
{
    Event_Begin();
    Actor_SetAnimation(actor, 1);
    Event_ShowMessage(actor, 0);
    Event_End();
}

void SceneDialogue_RunActor8FlagScene(void)
{
    Event_Begin();
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 2);
    GameFlag_Set(0x305);
    Event_SetMessage((s32)MsgKuupuappuWarriorsWhoCaptured);
    Event_ShowMessage(8, 0);
    Event_End();
}

void SceneDialogue_RunActor11Line(void)
{

    Event_SetMessage((s32)MsgKuupuappuRumorsThievesLunpa);
    Actor_FaceEachOther(11, ACTOR_PARTY_LEADER, 2);
    ActorPresentation_RunActorModeOneThenZero(11);
}

void SceneDialogue_RunActor12TwoFlagScene(void)
{

    Event_Begin();
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 2);
    GameFlag_Set(0x306);
    GameFlag_Set(0x868);
    Event_SetMessage((s32)MsgKuupuappuTalkingMayorStrong);
    Event_ShowMessage(12, 0);
    Event_End();
}

void SceneDialogue_ShowLine1CB0ForActor13(void)
{

    Event_SetMessage((s32)MsgKuupuappuVolcanoStaysCalm);
    Actor_FaceEachOther(13, ACTOR_PARTY_LEADER, 2);
    ActorPresentation_RunActorModeOneThenZero(13);
}

void SceneDialogue_RunActorFourteenFlagDialogue(void)
{
    struct SceneActor *actor = Actor_Get(14);
    u16 facing = actor->facing;
    s32 text;

    actor->state_flags |= 2;
    Event_Begin();
    text = (s32)MsgKuupuappuWarriorGuy;
    Event_SetMessage(text);
    Actor_SetAnimation(14, 0);
    Actor_FaceEachOther(14, ACTOR_PARTY_LEADER, 2);
    if (GameFlag_IsSet(0x300) == 0) {
        Actor_ShowEmote(14, 256, 60);
        Event_ShowMessageAndWait(14, 0, 10);
        Event_ShowMessageAndWait(14, 0, 10);
        GameFlag_Set(0x300);
    }
    Event_SetMessage(text + 2);
    Event_ShowMessageAndWait(14, 0, 10);
    actor->facing = facing;
    Task_Wait(1);
    Event_End();
    actor->state_flags = 1;
    GameFlag_Set(0x307);
}

/*
 * Actor dialogue: facing-preserving and timed lines, the acceptance
 * lines and the counted question.
 */
void SceneDialogue_RunActorFifteenFacingPreservedDialogue(void)
{
    struct Actor_02000640 *actor;
    s16 facing0;

    actor = Actor_Get(15);
    facing0 = (s16)actor->facing;
    actor->state_flags |= 2;
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuWishVaultsElders);
    Actor_SetAnimation(15, 0);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 2);
    Event_ShowMessageAndWait(15, 0, 10);
    actor->facing = (u16)facing0;
    Task_Wait(1);
    Event_End();
    actor->state_flags = 0;
}

void SceneDialogue_RunActor16CountedDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuGuysCheckJail);
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 2);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        ((struct SceneWork_020006b4 *)gWork)->branch_counter += 1;
    }
    Event_ShowMessage(16, 0);
    GameFlag_Set(0x308);
    Event_End();
}

void SceneDialogue_RunActorEightTimedDialogue(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 20);
    GameFlag_Set(0x305);
    Event_SetMessage((s32)MsgKuupuappuWarriorsWhoCaptured);
    Event_ShowMessageAndWait(8, 0, 20);
    Event_End();
}

void SceneDialogue_RunActor11AcceptanceDialogue(void)
{
    Event_SetMessage((s32)MsgKuupuappuOnceDodonpaTook);
    ((struct Actor_02000754 *)Actor_Get(11))->accepted = 1;
    ActorPresentation_RunActorModeOneThenZero(11);
    ((struct Actor_02000754 *)Actor_Get(11))->accepted = 0;
}

void SceneDialogue_RunActor12TimedTwoFlagScene(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 20);
    GameFlag_Set(0x306);
    GameFlag_Set(0x868);
    Event_SetMessage((s32)MsgKuupuappuTalkingMayorStrong);
    Event_ShowMessageAndWait(12, 0, 20);
    Event_End();
}

void ActorPresentation_RunActor13AcceptanceDialogue(void)
{
    Event_SetMessage((s32)MsgKuupuappuGreatEverythingSolved);
    ((struct Actor_020007d4 *)Actor_Get(13))->accepted = 1;
    ActorPresentation_RunActorModeOneThenZero(13);
    ((struct Actor_020007d4 *)Actor_Get(13))->accepted = 0;
}

void KuupuappuMuraSai_RunActor14Talk(void)
{
    ((struct SceneActor *)Actor_Get(14))->state_flags |= 2;
    Event_Begin();
    if (GameFlag_IsSet(0x307) != 0) {
        Event_SetMessage((s32)MsgKuupuappuWatchingGuysMakes);
        ActorPresentation_RunActorModeOneThenZero(14);
    } else {
        SceneDialogue_RunActorFourteenFlagDialogue();
        GameFlag_Set(0x307);
    }
    Event_End();
    ((struct SceneActor *)Actor_Get(14))->state_flags = 1;
}

/*
 * Scene steps: actor 16's question, actor 21's arrival and the leader's
 * arrival in scenes five to ten.
 */
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
        ((void (*)())Engine_ActorRunRepeatedMotion)(16, 1);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKuupuappuGuysCheckJail);
        ((void (*)())Engine_ActorFaceEachOther)(16, 0, 2);
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
        ActorPresentation_MoveActorToPositionAndWait(21, 0x188, 104, 0x70000);
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

/*
 * Actor dialogue: facing-preserving and timed lines, the acceptance
 * lines and the counted question.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuGreatEverythingSolved[];
extern u8 MsgKuupuappuGuysCheckJail[];
extern u8 MsgKuupuappuOnceDodonpaTook[];
extern u8 MsgKuupuappuTalkingMayorStrong[];
extern u8 MsgKuupuappuWarriorsWhoCaptured[];
extern u8 MsgKuupuappuWishVaultsElders[];

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

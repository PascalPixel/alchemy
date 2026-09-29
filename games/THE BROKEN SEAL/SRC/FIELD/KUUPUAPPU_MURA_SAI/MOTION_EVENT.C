/*
 * Actor proximity, the scene tables and the first villagers' lines.
 */

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

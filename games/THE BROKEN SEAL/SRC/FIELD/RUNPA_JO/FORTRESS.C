#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ITEM_IDS.H"

#define Scene_GetRecord_1(args...) Func_02007e76(args)
#define ObjectMotion_ResetAndSetPositionInMode2_1(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_SetSpeedLimitAndAcceleration_1(a0, a1) Value2(Engine_CameraSetSpeed, a0, a1)
#define ObjectMotion_ResetAndSetPositionInMode2_2(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_6(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_1(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_2(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define SharedWorkData_SetFirstAndSecondFields_1(args...) Func_020080e6_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1(args...) Func_020080e6_b(args)
#define Scene_GetRecord_2(args...) Func_0200804e(args)
#define ObjectMotion_ResetAndSetPositionInMode2_3(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_2(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_4(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_8(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ArmCallback_9(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_3(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_5(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_SetHorizontalPositionWithTerrain_2(a0, a1, a2) Value3(Engine_ActorSetPosition, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_8(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_9(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_7(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_11(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_8(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_12(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_10(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_9(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_15(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define BattleEffect_SpawnLinkedResourceObject_16(a0, a1, a2) Value3(Engine_ActorShowEmote, a0, a1, a2)
#define ObjectMotion_ResetAndSetPositionInMode2_10(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_ArmCallback_13(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define ObjectMotion_OffsetPositionAndResetMotion_3(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define SharedWorkData_SetFirstAndSecondFields_2(args...) Func_02008788_a(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_2(args...) Func_02008788_b(args)
#define STATE_TABLE_022B Data_02000240[0x22B]
#define SHARED_RECORD_FIELD_448 (*(u32 *)(*(u8 **)0x03001ebc + 448))
#define SharedWorkData_SetFirstAndSecondFields_1_02001e94(args...) Func_02007778(args)
#define BattleEffect_ComputeWeightedResultAndDispatch_1_02001e94(args...) Func_02007782(args)
#define Scene_GetRecord_1_02003054(args...) Func_020086d6(args)
#define Scene_GetRecord_2_02003054(args...) Func_02009a00(args)
#define Scene_GetRecord_3(args...) Func_02009a30(args)
#define Scene_GetRecord_4(args...) Func_02009a60_b(args)
#define SCENE_PHASE_02003054 (*(s32 *)(*(u8 **)0x03001ebc + 0x1c0))
#define PRIMARY_ID 24
#define DERIVED_ID 25
#define ObjectMotion_SetVariantCallbackAndInvokeObject_1_020049a0(a0, a1) Value2(Engine_ActorRunRepeatedMotion, a0, a1)
#define BattleRuntime_WaitIfModeZero_1_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_1_020049a0(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_1_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_1_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_2_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_SetAngleToward_1_020049a0(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_2_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define SceneWork_SetStepValue_2_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_SetAngleToward_2_020049a0(a0, a1, a2) Value3(Engine_ActorFaceActor, a0, a1, a2)
#define SceneWork_SetStepValue_3_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define BattleEvent_RunActionAndWait_3_020049a0(a0, a1) Value2(Engine_EventShowMessage, a0, a1)
#define BattleRuntime_WaitIfModeZero_3_020049a0(a0) Value1(Engine_EventWait, a0)
#define Object_SetModeById_2_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_4_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_1_020049a0(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_3_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define ObjectMotion_ArmCallback_1_020049a0(a0, a1, a2) Value3(Engine_ActorFaceDirection, a0, a1, a2)
#define SceneWork_SetStepValue_4_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define Object_SetModeById_3_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define BattleRuntime_WaitIfModeZero_5_020049a0(a0) Value1(Engine_EventWait, a0)
#define ObjectMotion_OffsetPositionAndResetMotion_3_020049a0(a0, a1, a2) Value3(Engine_ActorSetDestinationOffset, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_4_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_SetModeById_4_020049a0(a0, a1) Value2(Engine_ActorSetAnimation, a0, a1)
#define SceneWork_SetStepValue_5_020049a0(a0) Value1(Engine_EventSetMessage, a0)
#define ObjectMotion_ResetAndSetPositionInMode2_2_020049a0(a0, a1, a2) Value3(Engine_ActorWalkTo, a0, a1, a2)
#define ObjectMotion_CommitCurrentPositionAndActivate_5_020049a0(a0) Value1(Engine_ActorWaitForMove, a0)
#define Object_LinkPair_1_020049a0(a0, a1, a2) Value3(Engine_ActorFaceEachOther, a0, a1, a2)
#define BattleRuntime_WaitIfModeZero_6_020049a0(a0) Value1(Engine_EventWait, a0)
#define BattleRuntime_WaitIfModeZero_7_020049a0(a0) Value1(Engine_EventWait, a0)

#include "STAGED_ACTOR.H"
#include "OBJECT_RUNTIME.H"

typedef struct SceneActor {
    u8 pad0[8];
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} SceneActor;

typedef struct DirectionalSceneActor {
    u8 pad0[6];
    u16 dir;        /* 0x06 */
    s32 x;          /* 0x08 */
    s32 y;          /* 0x0c */
    s32 z;          /* 0x10 */
} DirectionalSceneActor;

extern s32 Value_00000f2e;
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200e2a4[];
extern u8 Data_0200e4b4[];
extern u8 Data_0200e754[];
extern u8 Data_0200e814[];
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];
extern u8 Value_0000006a;
extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];
extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];
extern s32 LinkedMessage_DoorTightlyLocked;
extern s32 Value_00000f2a;
extern s32 Value_00000f2b;
extern s32 Value_00000f2c;
extern s32 Value_00000f2d;
extern u8 LinkedMessage_AllRightAllRightWe;
extern u8 LinkedMessage_DoYouKnowWhereDodonpa;
extern u8 Value_0000256c;
extern s32 Data_0200f714[];
extern s32 Data_0200f754[];
extern s32 Data_0200f764[];
extern u8 LinkedMessage_CouldSwearJustHeardSomeone[];
extern u32 Data_03001e40;
extern u8 Value_00002092;
extern u8 Value_0000240d;
extern u8 Value_0000241e;
extern u8 Value_00002428;
extern u8 Value_0000247d;
extern u8 Value_00002464;
extern u8 LinkedMessage_DodonpaPulledLever;
extern u8 LinkedMessage_WhWhWhoGoesThere[];
extern u8 LinkedMessage_YouDontLookNearlyAs[];
extern u8 LinkedMessage_MyShiftUpAlready[];
extern u8 Data_000000a1[];
extern u8 Data_00002438[];
extern u8 LinkedMessage_WhoYou[];
extern u8 LinkedMessage_UhnnGetOffMe[];
extern u8 LinkedMessage_ThereYouShouldAbleGet[];
extern u8 LinkedMessage_Dad[];
extern u8 LinkedMessage_ShhhPleaseDontWakeDonpa[];
extern u8 LinkedMessage_DonpaKnowsHeCoddledDodonpa[];
extern u8 Data_02009151[], Data_0200938d[], Data_02009a45[];
extern u8 Data_02009719[], Data_0200975d[], Data_020097bd[];
extern u8 Data_02009529[], Data_020099e9[], Data_0200969d[];
s32 Func_020065a2(s32);
void Func_0200a30a(s32, s32, s32);
s32 Func_0200a36e(s32);
s32 Func_020067d2(s32, s32, s32);
SceneActor *Func_0200675e(s32);
SceneActor *Func_0200697a(s32);
SceneActor *Func_02006982(s32);
SceneActor *Func_02006b22(s32);
SceneActor *Func_02006cb6(s32);
SceneActor *Func_02006e70(void);
SceneActor *Func_02006e78(s32);
SceneActor *Func_02006ed4(void);
SceneActor *Func_02006edc(s32);
s32 Func_02003114(s32);
s32 Func_02003186(s32);
SceneActor *Func_02006f6c(void);
SceneActor *Func_02006f74(s32);
SceneActor *Func_02006fd4(void);
SceneActor *Func_020070da(void);
SceneActor *Func_0200703e(s32);
SceneActor *Func_0200709a(s32);
u8 *Func_020061ca(s32);
s32 Func_02006244();
s32 Func_0200629c();
s32 Func_020062f2();
s32 Func_020063de();
s32 Func_02006542();
s32 Func_02006492();
struct ObjectRuntime *Func_020067ac();
struct ObjectRuntime *Func_020067b4();
double Func_02006ce6(s32);
double Func_02006c7a(double, double);
s32 Func_02006d72(double);
s32 Func_02002b3e();
struct ObjectRuntime *Func_020069e6();
void Func_02006afa();
s32 Func_02002d5a();
struct ObjectRuntime *Func_02006b82();
s32 Func_02002eec();
void Func_02006cd4(s32);
s32 Func_02003054_scene_scripts();
void Func_02006d28();
void Func_02006d2e();
void Func_02006d88();
void Func_02006d8e();
void Func_020057ec();
s32 Func_02007e76();
s32 Func_0200804e();
void Func_020080e6_a();
void Func_020080e6_b();
void Func_02008788_a();
void Func_02008788_b();
void Func_0200a2ac();
void Func_02007564(s32, s32);
void Func_020075cc(s32, s32);
void Func_0200786c_a(s32, s32);
void Func_02007778();
void Func_02007782();
s32 Func_02007b82();
s32 Func_02007b96();
s32 Func_02007baa();
u8 *Func_020086d6();
u8 *Func_02009a00();
u8 *Func_02009a30();
u8 *Func_02009a60_b();
struct ObjectRuntime *Func_0200aa16();
struct ObjectRuntime *Func_0200aa2a();
struct ObjectRuntime *Village_GetActor();
struct ObjectRuntime *Func_0200aa52();
struct ObjectRuntime *Func_0200aa6c();
struct ObjectRuntime *Func_0200aa84();
void Village_FinishActorRestore();
void Func_0200a7f8();
void Func_0200a816();
struct ObjectRuntime *Func_0200a874();
void Func_0200a854();
void Func_0200a7f6();
void Func_0200a844();
void Func_0200a8c4();
struct ObjectRuntime *Func_0200a922();
struct ObjectRuntime *Func_0200a92a();
s32 Func_0200a572();
s32 Func_0200a57c();
s32 Func_0200a586();
s32 Func_0200a5b2();
s32 Func_0200a5bc();
void Func_0200a650();
s32 Func_0200a602();
void Func_0200a682();

/*
 * Distance between two three-component 16.16 fixed-point positions.
 *
 * Each argument walks three consecutive 16.16 words in x, y, z order. The
 * per-axis deltas are taken in fixed point, shifted down to integers, squared,
 * and summed; the total is passed to the resident IWRAM integer square root.
 *
 * Expressions are preserved exactly as reconstructed: the walking-pointer form
 * is load-bearing for byte-identity and must not become struct field access.
 */

/* No scene data is available for this slot. */

/* External Func aliases name loader-relocated call words, not runtime addresses. */

/* Audited retained supplemental scene body.
 * The complete production span preserves 49 calls, 0 loop(s), and 0 explicit memory operation(s).
 * Approved GCC changes scheduling, allocation, control-flow lowering, and
 * literal placement, so the reviewed assembly remains authoritative. */

/* Two id arguments recur across most of the calls below: 24 is passed as the
 * first argument throughout the main sequence, and 25 is introduced only
 * after ObjectMotion_SetAngleToward_2_020049a0(25, 24, 0), whose second argument is 24 -- so 25 is
 * derived from 24 partway through the sequence. */

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id);

s32 IsActorInteractionAvailable(s32 actor_id);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Actor_SetPosition(actor, x, y);
}

static __inline__ void Call1(void (*f)(), s32 a0)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    f(a0, a1, a2);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void SetSceneValue(s16 *field, s32 value)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    *field = value;
}

/*
 * Actor script -- resource_3bf.  It walks a run of consecutive resource
 * identifiers starting at 0x241e, re-supplying the actor handle to every
 * service that takes one, and returns nothing.
 */

/*
 * Symbol names use the offsets as printed while absolute data addresses keep
 * their raw pool values; the module's own pool constants sit 0x8000 above
 * the printed offsets.  Func_02007376 is reached with two different argument
 * shapes -- once as (handle, 0) and once as (identifier) -- so these services
 * keep old-style declarations rather than an invented common prototype.
 */

/* Byte 0x22B of Data_02000240, a flag/state table indexed by other code. */

/* Word at +448 of the record pointed to by the global at 0x03001ebc. */
static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

static __inline__ void Call1_0200169c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_0200169c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/*
 * Keeping this call in its own helper preserves the interleaving of the
 * handle move with the two-instruction constant.
 */
static __inline__ void Call_02007388(s32 handle)
{
    Actor_ShowEmote(handle, 258, 60);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1_0200238c(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6_0200169c(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call3_0200169c(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02004bfc(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call3_02001cf0(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001cf0(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1_02001dc4(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1_02001dc4(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6_02001dc4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call1_02001e94(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02001e94(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2_0200206c(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call1_020021c4(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020021c4(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ s32 Value1_0200252c(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200252c(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1_020025f8(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value3_020025f8(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020025f8(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1_0200269c(void (*f)(), s32 a0)
{
    f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_0200269c(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call3_02002718(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02002718(s32 amount)
{
    gEventWork->message += amount;
}

static __inline__ void Call1_02003054(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call2_02003054(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value2_02003054(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02003054(s32 amount)
{
    u8 *work = *(u8 **)0x03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

static __inline__ void Call1_02004794(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value2_02004794(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_02004794(s32 amount)
{
    gEventWork->message += amount;
}

/* Loader-relocated ROM calls: each site names the pre-relocation call word the image holds. */
static __inline__ void Call0_020049a0(void (*f)())
{
    f();
}

static __inline__ void Call2_020049a0(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_020049a0(s32 (*f)())
{
    return f();
}

static __inline__ s32 Value2_020049a0(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ s32 Value3_020049a0(s32 (*f)(), s32 a0, s32 a1, s32 a2)
{
    return f(a0, a1, a2);
}

static __inline__ void Call1_02004f60(void (*f)(), s32 value)
{
    f(value);
}

/* Scheduler_AddOrUpdateCallback returns an index even when it is ignored. */
static __inline__ s32 Value2_02004f60(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

#if defined(TBS_EDITION_JA)
#define Value_00002421 Value_000025aa
#elif defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define Value_00002421 Value_00002403
#endif

extern u8 Value_00002421;

s32 SelectPrimarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200e2a4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e4b4;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200e754;
    }
    return (s32)Data_0200e814;
}

s32 GetEmptySceneData(void) { return 0; }

s32 SelectSecondarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == (s32)&Value_0000006a) {
        return (s32)Data_0200e9d0;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200ee08;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200ec28;
    }
    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eac0;
    }
    if (scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}

s32 SelectQuaternarySceneData(void)
{
    s16 scene_variant = gGameState.scene;

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eff4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200f258;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}

void ConfigureSceneActor12(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    s32 actor_slot = 15;
    u8 *actor;

    Map_CopyCellAttributes(15, 20, 1, 1, actor_slot, 22);
    Map_CopyCellAttributes(17, 23, 1, 3, actor_slot, 23);
    actor = Func_020061ca(12);
    if (actor != 0) {
        Actor_SetSpriteFlags(actor, 0);
        actor[0x55] = 0;
        actor[0x23] = 2;
    }
}

void RunSceneObjectSetup(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    StagedActor_AdvancePair();
}

void FieldScene_StartActorTwelveTransition(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    Actor_SetSpeed(12, 0x10000, 0x8000);
    Actor_SetDestination(12, 248, 0x178);
    Actor_WaitForMove(12);
    Audio_PlayCue(215);
    Event_Wait(60);
    ConfigureSceneActor12();
    GameFlag_Set(0x943);
}

void FieldScene_UpdateActorTwelveTransition(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Value1(Func_02006244, 12);
    if ((actor->z.fixed >> 20) > 22) {
        Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        Audio_PlayCue(144);
        Map_CopyCellAttributes(15, 20, 1, 1, 15, 22);
        Map_CopyCellAttributes(17, 23, 1, 3, 15, 23);
        actor = (struct FieldActor *)Value1(Func_0200629c, 12);
        if (actor != NULL) {
            Actor_SetSpriteFlags(actor, 0);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        }
        GameFlag_Set(0x943);
    }
}

void PlaceActorTwelveAndFinishScene(void)
{
    PlaceActor(12, 0x00f80000, 0x01780000);
    ConfigureSceneActor12();
}

void PlaceSceneObjectPairFromTableA(s32 table_index)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    s32 position_x = Data_0200f714[table_index * 2];
    s32 position_z = Data_0200f714[table_index * 2 + 1];

    Map_CopyCells(0, 0x4d, 1, 3, position_x, position_z);
    Map_CopyCells(1, 0x4d, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x30, 1, 1, position_x, position_z - 0x2e);
}

void FieldScene_UpdateObjectPairA(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;
    s32 index;

    work = Data_03001ebc;
    if (Func_020062f2(234) != -1) {
        trigger = work->touched_trigger;
        index = trigger - 40;
        if (GameFlag_IsSet(0x941) == 0 || index != 4) {
            PlaceSceneObjectPairFromTableA(index);
            Audio_PlayCue(157);
            Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
            Work_SetValuesIfNonNegative(-1, -1, 0xe666);
            GameFlag_Set(trigger + 0x328);
        }
    }
}

void FieldScene_UpdateObjectPairB(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_020063de(234) != -1) {
        trigger = work->touched_trigger;
        FieldScene_SetPositionPairs(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x32d);
    }
}

void PlaceSceneObjectPairFromTableB(s32 table_index)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    s32 position_x = Data_0200f754[table_index * 2];
    s32 position_z = Data_0200f754[table_index * 2 + 1];

    Map_CopyCells(0x37, 0x79, 1, 3, position_x, position_z);
    Map_CopyCells(0x38, 0x79, 1, 1, position_x + 1, position_z);
    Map_CopyCells(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateTableBObjectPair(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_02006492(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableB(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x330);
    }
}

void PlaceSceneObjectPairFromTableC(s32 table_index)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    s32 position_x = Data_0200f764[table_index * 2];
    s32 position_z = Data_0200f764[table_index * 2 + 1];

    Map_CopyCells(1, 0x50, 1, 3, position_x, position_z);
    Map_CopyCells(2, 0x50, 1, 1, position_x + 1, position_z);
    Map_CopyCellAttributes(position_x, position_z - 0x3f, 1, 1, position_x, position_z - 0x3e);
}

void FieldScene_UpdateObjectPairC(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;
    s32 trigger;

    work = Data_03001ebc;
    if (Func_02006542(234) != -1) {
        trigger = work->touched_trigger;
        PlaceSceneObjectPairFromTableC(trigger - 40);
        Audio_PlayCue(157);
        Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
        Work_SetValuesIfNonNegative(-1, -1, 0xe666);
        GameFlag_Set(trigger + 0x332);
    }
}

void CellDoor_Touch(void)
{
    if (Func_020065a2(ITEM_CELL_KEY) == -1) {
        Message_ShowCentered((s32)&LinkedMessage_DoorTightlyLocked, 1);
    }
}

void LockedDoor_Touch(void)
{
    Message_ShowCentered((s32)&LinkedMessage_DoorTightlyLocked, 1);
}

void Actor8_Interact(void)
{
    if (TryStartActorInteraction(8, 8) != 0) {
        GameFlag_Set((s32)&Value_00000f2a);
    }
}

void Actor9_Interact(void)
{
    if (TryStartActorInteraction(9, 7) != 0) {
        GameFlag_Set((s32)&Value_00000f2b);
    }
}

void Actor10_Interact(void)
{
    if (TryStartActorInteraction(10, 6) != 0) {
        GameFlag_Set((s32)&Value_00000f2c);
    }
}

void Actor11_Interact(void)
{
    if (TryStartActorInteraction(11, 5) != 0) {
        GameFlag_Set((s32)&Value_00000f2d);
    }
}

s32 TryStartActorInteraction(s32 actor_id, s32 interaction_id)
{
    s32 started = 0;
    s32 interaction;

    Event_Begin();
    interaction = Func_020067d2(0, actor_id, interaction_id);
    if (Party_GiveItem(interaction_id, 0) != -1) {
        Actor_SetAnimation(actor_id, 2);
        started = 1;
    } else {
        Audio_PlayCue(0x7d);
        Actor_SetAnimation(actor_id, 5);
    }
    Engine_ObjectDispatchRelease(interaction);
    Event_End();
    return started;
}

void NoOpSceneCallbackA(void)
{
}

void NoOpSceneCallbackB(void)
{
}

void NoOpSceneCallbackC(void)
{
}

void NoOpSceneCallbackD(void)
{
}

void CellKey_PickUp(void)
{
    extern s32 Data_0200dfa4;

    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x108, 0x318);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Item_ShowFound(ITEM_CELL_KEY, 3);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Party_GiveItem(ITEM_CELL_KEY, 0);
    GameFlag_Set((s32)&Value_00000f2e);
    Actor_SetPosition(8, 0, 0);
}

s32 IsPlayerInAccidentTriggerArea(void)
{
    SceneActor *player = Func_0200675e(0);
    s32 z = player->z;
    s32 x;
    s32 zz, xx;

    if (z < 0) {
        z += 0xfffff;
    }
    x = player->x;
    zz = z >> 20;
    if (x < 0) {
        x += 0xfffff;
    }
    xx = x >> 20;
    if ((u32)(zz - 5) <= 2 && xx <= 10) {
        return 1;
    }
    if ((u32)(xx - 8) <= 1 && zz > 22) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorPairInteraction(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    struct ObjectRuntime *actor = Func_020067ac(9);
    struct ObjectRuntime *other = Func_020067b4(10);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
        other->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        other->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = Func_02006d72(Func_02006c7a(8912896.0, Func_02006ce6(actor->x)));
        }
        if (!IsPlayerInAccidentTriggerArea()) {
            if (gGameState.cloaked != 0) {
                if (Func_02002b3e(9) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
                if (Func_02002b3e(10) && gGameState.cloaked != 0) {
                    SetSceneValue(&scene[191], (s32)&Value_00002092);
                    return;
                }
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(9)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
                if (IsActorInteractionAvailable(10)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 91);
            }
        }
    }
}

void ConfigureSceneActor9(void)
{
    Event_Begin();
    Actor_Stop(9);
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_SetAnimation(9, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(9, 256, 0);
    RunActorScriptedSequenceA(10);
    Event_End();
}

s32 AreSceneActorsInPassingLane(void)
{
    SceneActor *player = Func_0200697a(0);
    SceneActor *passing_actor = Func_02006982(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorSeventeenInteraction(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    struct ObjectRuntime *actor = Func_020069e6(17);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    Func_02006afa(actor);
    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!AreSceneActorsInPassingLane()) {
            IsSceneActorWithinTriggerBox(17);
            if (Func_02002d5a(17) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(17)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void ActivateSceneActor17(void)
{
    RunActorScriptedSequenceA(17);
    Event_End();
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    SceneActor *player = Func_02006b22(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorEighteenInteraction(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    struct ObjectRuntime *actor = Func_02006b82(18);
    s32 *work = (s32 *)(*(u8 **)Data_03001e70 + 0x164);
    s16 *scene = *(s16 **)(Data_03001e70 + 0x4c);

    if (Data_03001e40 & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!IsPlayerInSecondaryTriggerArea()) {
            if (Func_02002eec(18) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], (s32)&Value_00002092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(18)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void ActivateSceneActor18(void)
{
    RunActorScriptedSequenceA(18);
    Event_End();
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Func_02006cb6(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

void FieldScene_RunScene3bfSequenceA(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (GameFlag_IsSet(0x214) == 0) {
        if (Value0(IsPlayerOutsideSceneRectangle) == 0) {
            if (gGameState.cloaked == 0) {
                if (Value1_0200169c(IsActorInteractionAvailable, 17) != 0) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214) != 0) {
                work->raised_trigger = 94;
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    RunActorScriptedSequenceA(17);
    Event_End();
}

void TriggerSceneStage95FromActor12(void)
{
    extern u8 *Data_03001ebc;

    u8 *scene_state = Data_03001ebc;

    if (IsActorInteractionAvailable(12) != 0 && gGameState.cloaked == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Func_02006cd4(0x02009719);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void FieldScene_RunScene3bfSequenceB(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (GameFlag_IsSet(0x225) == 0) {
        if (Value1_0200169c(Func_02003054_scene_scripts, 13) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Call1_0200169c(Func_02006d28, 0x200975d);
                Call1_0200169c(Func_02006d2e, 0x20097bd);
                work->raised_trigger = 96;
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{
    extern struct EventWork *Data_03001ebc;

    struct EventWork *work;

    work = Data_03001ebc;
    if (GameFlag_IsSet(0x225) == 0) {
        if (Value1_0200169c(IsActorInteractionAvailable, 21) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Call1_0200169c(Func_02006d88, 0x20097bd);
                Call1_0200169c(Func_02006d8e, 0x200975d);
                work->raised_trigger = 96;
            }
        }
    }
}

s32 IsSceneActorVerticallyNearPlayer(void)
{
    SceneActor *scene_actor = Func_02006e70();
    SceneActor *player = Func_02006e78(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(void)
{
    SceneActor *scene_actor = Func_02006ed4();
    SceneActor *player = Func_02006edc(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (IsSceneActorWithinTriggerBox() == 0) {
        return 0;
    }
    if (Func_02003114(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = Func_02003186(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(void)
{
    SceneActor *scene_actor = Func_02006f6c();
    SceneActor *player = Func_02006f74(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(void)
{
    SceneActor *scene_actor = Func_02006fd4();
    SceneActor *player = Func_020070da();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{
    extern u8 *Data_03001ebc;

    SceneActor *player = Func_0200703e(0);

    if (GameFlag_IsSet(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 41;

            *q = v;
            FieldScene_UpdateObjectPairC();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{
    extern u8 *Data_03001ebc;

    DirectionalSceneActor *player = Func_0200709a(0);

    if (GameFlag_IsSet(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(Data_03001ebc + 364);
            s32 v = 40;

            *q = v;
            FieldScene_UpdateTableBObjectPair();
        }
    }
}

void RunActor9ScriptedSequence(void)
{
    Event_Begin();
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_EnableActionCallback(9, 1);
    Actor_Stop(9);
    Actor_SetAnimation(9, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        u8 *t = &Value_0000240d;

        Event_SetMessage((s32)t);
        Event_ShowMessage(9, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(9, 0);
    Event_RequestExit(60);
    Event_CloseScreen();
    Event_End();
}

void RunActorScriptedSequenceA(s32 actor_id)
{
    Event_Begin();
    Event_Begin();
    Actor_ShowEmote(actor_id, 256, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Actor_SetAnimation(actor_id, 0);
    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetDestinationOffset(actor_id, 0, 0);
    Actor_EnableActionCallback(actor_id, 1);
    Actor_Stop(actor_id);
    Actor_SetAnimation(actor_id, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    {
        u8 *t = &Value_0000240d;

        Event_SetMessage((s32)t);
        Event_ShowMessage(actor_id, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 60);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(actor_id, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
}

void TurnActorToSceneDirection(s32 actor_id)
{
    extern s32 Data_0200dfa4;

    Actor_FaceActor(actor_id, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, actor_id, 0);
    switch (Data_0200dfa4 & 3) {
    case 0:
        RunActorScriptedSequenceB(actor_id);
        break;
    case 1:
        RunActorScriptedSequenceC(actor_id);
        break;
    case 2:
        FieldScene_RunScene3bf_02001cf0(actor_id);
        break;
    case 3:
        RunActorScriptedSequenceD(actor_id);
        break;
    default:
        RunActorScriptedSequenceC(actor_id);
        break;
    }
}

/*
 * The resource run is taken as the address of Value_0000241e rather than as
 * an integer constant, which preserves its pointer identity and materialises
 * it after the first call.
 */
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Actor_RunRepeatedMotion(handle, 1);
    id = &Value_0000241e;
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
    Call_02007388(handle);
    Event_SetMessage((s32)(id + 1));
    Event_ShowMessage(handle, 0);
    id += 2;
    Actor_SetAnimationAndWait(handle, 4);
    Event_SetMessage((s32)id);
    Event_ShowMessage(handle, 0);
}

void RunActorScriptedSequenceC(s32 actor_id)
{
    u8 *t = &Value_00002421;

    Event_SetMessage((s32)t);
    Event_ShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Event_SetMessage((s32)(t + 1));
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Event_SetMessage((s32)(t + 2));
    Event_ShowMessage(actor_id, 0);
}

void FieldScene_RunScene3bf_02001cf0(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2424;

    base6_2424 = (s32)LinkedMessage_MyShiftUpAlready;
    Event_SetMessage(base6_2424);
    Event_ShowMessage(a0, 0);
    Event_Wait(120);
    Actor_ShowEmote(a0, 0x101, 60);
    Event_SetMessage((base6_2424 + 1));
    Event_ShowMessage(a0, 0);
    Actor_RunRepeatedMotion(a0, 1);
    Event_SetMessage((base6_2424 + 2));
    Event_ShowMessage(a0, 0);
    Actor_SetAnimationAndWait(a0, 4);
    Event_SetMessage((base6_2424 + 3));
    Event_ShowMessage(a0, 0);
}

void RunActorScriptedSequenceD(s32 actor_id)
{
    u8 *t = &Value_00002428;

    Event_SetMessage((s32)t);
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 4);
    Event_SetMessage((s32)(t + 1));
    Event_ShowMessage(actor_id, 0);
    Actor_RunRepeatedMotion(actor_id, 1);
    Event_SetMessage((s32)(t + 2));
    Event_ShowMessage(actor_id, 0);
    Actor_SetAnimationAndWait(actor_id, 3);
    Event_SetMessage((s32)(t + 3));
    Event_ShowMessage(actor_id, 0);
}

void InspectOrdinaryObject(void)
{
    extern u8 Data_02000240[];

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(15, 256, 60);
    TurnActorToSceneDirection(15);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Data_02000240[0x22b] = 3;
    Func_02007564(98, 2);
    Actor_SetPosition(15, 0, 0);
    Event_End();
    GameFlag_Set(2380);
}

void InspectEmptyChest(void)
{
    extern u8 Data_02000240[];

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(11, 256, 60);
    TurnActorToSceneDirection(11);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Data_02000240[0x22b] = 3;
    Func_020075cc(98, 2);
    Actor_SetPosition(11, 0, 0);
    Event_End();
    GameFlag_Set(2377);
}

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at Data_00002438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at Data_02000240
 * before handing off to the next step. */
void FieldScene_RunSupplementalSequenceOne(void)
{
    extern u8 Data_02000240[];

    s32 sequence_2438;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Actor_SetAnimation(14, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 0x100, 0);
    Event_Wait(30);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    sequence_2438 = (s32)Data_00002438;
    Event_SetMessage(sequence_2438);
    Event_ShowMessage(12, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Event_Wait(65);
    Actor_FaceDirection(13, 0x5000, 0);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_SetMessage((sequence_2438 + 1));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((sequence_2438 + 2));
    Event_ShowMessage(14, 0);
    Event_SetMessage((sequence_2438 + 3));
    Event_ShowMessage(12, 0);
    Actor_RunRepeatedMotion(13, 1);
    Event_SetMessage((sequence_2438 + 4));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((sequence_2438 + 5));
    Event_ShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(60);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(70);
    Actor_WalkTo(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    Actor_WaitForMove(12);
#if !defined(TBS_EDITION_JA)
    /* The localized sequence adds this turn after actor 12 is placed. */
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
#endif
    Actor_SetAnimationAndWait(12, 3);
    Event_Wait(30);
    Event_SetMessage((sequence_2438 + 6));
    Event_ShowMessage(12, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    SharedWorkData_SetFirstAndSecondFields_1_02001e94((s32)Data_000000a1, 31);
    Data_02000240[0x22b] = 3;
    BattleEffect_ComputeWeightedResultAndDispatch_1_02001e94(98, 3);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Event_End();
    GameFlag_Set(0x94a); /* main:080770c8 */
}

void FieldScene_RunSequenceTail(void)
{
    Event_Begin();
    Actor_SetPosition(12, 45088768, 5767168); /* object_id 12, x, z */
    Actor_SetPosition(13, 46137344, 5767168); /* object_id 13, x, z */
    Actor_SetPosition(14, 47185920, 6291456); /* object_id 14, x, z */
    Actor_SetAnimation(12, 5); /* object_id 12, action 5 */
    Actor_SetAnimation(13, 5); /* object_id 13, action 5 */
    Actor_SetAnimation(14, 5); /* object_id 14, action 5 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Event_End();
    Event_OpenScreen(); /* main:0808a360 */
}

void InspectEmptySceneObject(void)
{
    extern u8 Data_02000240[];

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(16, 256, 60);
    TurnActorToSceneDirection(16);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Data_02000240[0x22b] = 3;
    Func_0200786c_a(98, 2);
    Actor_SetPosition(16, 0, 0);
    Event_End();
    GameFlag_Set(2379);
}

void RunActor12InteractionSequence(void)
{
    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 256, 60);
    {
        u8 *t = &Value_0000240d;

        Event_SetMessage((s32)t);
        Event_ShowMessage(12, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 50);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(12, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(548);
}

void FieldScene_RunScene3bf_020021c4(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 60);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    base5_240d = (s32)LinkedMessage_WhoYou;
    Event_SetMessage(base5_240d);
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 30);
    Event_SetMessage((base5_240d + 1));
    Event_ShowMessage(13, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(0x225);
}

void ConfigureInteractionRegionA(void)
{
    Map_CopyCells(2, 82, 1, 2, 21, 81);
    Map_CopyCellAttributes(21, 32, 1, 1, 21, 34);
}

void ConfigureInteractionRegionB(void)
{
    Map_CopyCells(2, 84, 1, 2, 6, 55);
    Map_CopyCellAttributes(5, 9, 1, 1, 6, 10);
}

void ConfigureInteractionRegionC(void)
{
    Map_CopyCells(2, 86, 1, 2, 27, 62);
    Map_CopyCellAttributes(26, 16, 1, 1, 27, 17);
}

void InspectVillageWell(void)
{
    extern u8 *Data_03001ebc;

    if (*(s16 *)(Data_03001ebc + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x947) == 0) {
            Message_ShowCentered(0x1528, 1);
            Audio_PlayCue(188);
            Event_Wait(1);
            Map_CopyCells(6, 77, 1, 2, 17, 82);
            Event_Wait(5);
            Map_CopyCells(7, 77, 1, 2, 17, 82);
            Event_Wait(1);
            ConfigureInteractionRegionA();
            GameFlag_Set(0x947);
        }
    }
}

void RunSecondaryMapInteraction(void)
{
    extern u8 *Data_03001ebc;

    if (*(s16 *)(Data_03001ebc + 0xcb8) != 0) {
        if (GameFlag_IsSet(0x948) == 0) {
            Message_ShowCentered(0x1528, 1);
            Audio_PlayCue(188);
            Event_Wait(1);
            Map_CopyCells(6, 77, 1, 2, 3, 55);
            Event_Wait(5);
            Map_CopyCells(7, 77, 1, 2, 3, 55);
            Event_Wait(1);
            ConfigureInteractionRegionB();
            GameFlag_Set(0x948);
        }
    }
}

void ConfigurePrimaryInteractionRegions(void)
{
    Map_CopyCells(5, 77, 1, 2, 17, 82);
    Map_CopyCells(5, 77, 1, 2, 3, 55);
    Map_CopyCellAttributes(15, 33, 1, 1, 17, 35);
    Map_CopyCellAttributes(3, 8, 1, 1, 3, 10);
}

void ConfigureSecondaryInteractionRegions(void)
{
    Map_CopyCells(8, 77, 1, 2, 17, 82);
    Map_CopyCells(8, 77, 1, 2, 3, 55);
    Map_CopyCellAttributes(18, 35, 1, 1, 17, 35);
    Map_CopyCellAttributes(2, 10, 1, 1, 3, 10);
}

void InspectWardrobe(void)
{
    GameFlag_Set(2372);
    GameFlag_Clear(535);
    Actor_SetPosition(8, 0, 0);
}

void InspectFirewood(void)
{
    GameFlag_Set(2373);
    ConfigureInteractionRegionC();
    Actor_SetPosition(9, 0, 0);
}

void InspectBooks(void)
{
    GameFlag_Set(2374);
    GameFlag_Clear(536);
    Actor_SetPosition(10, 0, 0);
}

void NoOpInteractionCallback(void)
{
}

void FieldScene_RunScene3bf_0200252c(void)
{
    struct FieldActor *actor;

    actor = (struct FieldActor *)Value1_0200252c(Func_02007b82, 0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Value1_0200252c(Func_02007b96, 0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
    }
    actor = (struct FieldActor *)Value1_0200252c(Func_02007baa, 0);
    if (actor != NULL) {
        Actor_SetPosition(ACTOR_GERALD, actor->x.fixed, actor->z.fixed);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_IVAN, 0x1c8, 192);
    Actor_SetSpeed(ACTOR_MIA, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 184);
    Actor_SetSpeed(ACTOR_GERALD, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 240);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceActor(ACTOR_IVAN, 12, 0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(ACTOR_MIA, 12, 0);
    Event_Wait(15);
}

void FieldScene_RunScene3bf_020025f8(void)
{
    u32 i;
    s32 record;

    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    Audio_PlayCue(141);
    Event_Wait(80);
    Audio_PlayCue(0x120);
    Event_Wait(5);
    Audio_PlayCue(145);
    Map_CopyCells(16, 75, 7, 4, 26, 55);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 0);
    Value3_020025f8(Engine_ActorShowEmote, 12, 0x100, 0);
    Event_Wait(60);
}

void FieldScene_RunScene3bf_0200269c(void)
{
    u32 i;
    s32 record;

    Camera_MoveToActor(11, 1);
    Camera_WaitForMove();
    Event_Wait(60);
    Event_SetMessage(0x247c);
    Event_ShowMessage(13, 0);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_SetSpeed(15, 0x10000, 0x8000);
    Actor_WalkTo(11, 0x1d8, 180);
    Actor_WalkTo(15, 0x1d8, 180);
    Camera_FollowActor(11, 1);
    Actor_WaitForMove(11);
    Actor_SetAnimation(11, 4);
    Event_Wait(30);
}

void FieldScene_RunScene3bf_02002718(void)
{
    u32 i;
    s32 record;

    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_IVAN, 0x1f8, 216);
    Actor_SetSpeed(ACTOR_MIA, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_MIA, 0x1b8, 232);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_GERALD, 0x1e0, 224);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xc000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
}

/* Runs one of two long fixed sequences (chosen by a status check), each
 * placing actors, panning/animating them, and running dialogue lines built
 * from a base text pointer plus a running line offset. */
void PlayStoryScene(void)
{
    extern u8 Data_02000240[];

    s32 text_line;

    if (GameFlag_IsSet(769) != 0) {
        Func_020057ec();
    }
    GameFlag_Set(624);
    Event_Begin();
    if (GameFlag_IsSet(2370) != 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        FieldScene_RunScene3bf_0200252c();
        Actor_ShowEmote(12, 256, 60);
        Actor_FaceDirection(12, 32768, 0);
        Actor_Jump(12, 4, 0);
        Actor_SetSpriteFlags(Scene_GetRecord_1(12), 1);
        Event_Wait(30);
        Actor_SetSpeed(ACTOR_IVAN, 45875, 22937);
        ObjectMotion_ResetAndSetPositionInMode2_1(2, 464, 192);
        Actor_WaitForMove(ACTOR_IVAN);
        Event_Wait(30);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 16384, 0);
        Actor_FaceDirection(ACTOR_IVAN, 16384, 0);
        Actor_FaceDirection(ACTOR_GERALD, 16384, 0);
        Actor_FaceDirection(ACTOR_MIA, 16384, 0);
        Actor_SetPosition(13, 29884416, 20971520);
        ObjectMotion_SetSpeedLimitAndAcceleration_1(131072, 16384);
        text_line = (s32)&Value_0000247d;
        Event_SetMessage(text_line);
        Event_ShowMessage(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_2(13, 458, 272);
        Actor_WaitForMove(13);
        ObjectMotion_ArmCallback_6(13, 20480, 0);
        Event_Wait(40);
        ObjectMotion_OffsetPositionAndResetMotion_1(13, -8, 8);
        Actor_WaitForMove(13);
        Event_Wait(60);
        Audio_PlayCue(155);
        Message_ShowCentered(text_line + 1, 1);
        ObjectMotion_OffsetPositionAndResetMotion_2(13, 8, -8);
        FieldScene_RunScene3bf_020025f8();
        Event_Wait(120);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
        Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
        Actor_StartRepeatedMotion(ACTOR_MIA, 2);
        Event_Wait(20);
        text_line += 2;
        FieldScene_RunScene3bf_02002718();
        Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
        Event_SetMessage(text_line);
        Event_ShowMessage(13, 0);
        FieldScene_RunScene3bf_0200269c();
        SHARED_RECORD_FIELD_448 = 512;
        Event_Wait(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_1((s32)&Value_000000a3, 4);
        BattleEffect_ComputeWeightedResultAndDispatch_1(98, 4);
    } else {
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
        Audio_PlayCue(17);
        Event_Wait(30);
        text_line = (s32)&Value_00002464;
        Event_SetMessage(text_line);
        Event_ShowMessage(12, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
        Event_Wait(140);
        Actor_FaceDirection(12, 32768, 0);
        Actor_Jump(12, 4, 0);
        Actor_SetSpriteFlags(Scene_GetRecord_2(12), 1);
        Event_SetMessage(text_line + 1);
        Event_ShowMessage(12, 0);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 456, 216);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        FieldScene_RunScene3bf_0200252c();
        Actor_SetSpeed(ACTOR_IVAN, 45875, 22937);
        ObjectMotion_ResetAndSetPositionInMode2_3(2, 464, 192);
        Actor_WaitForMove(ACTOR_IVAN);
        Event_Wait(30);
        Event_SetMessage(text_line + 2);
        Event_ShowMessage(ACTOR_IVAN, 0);
        BattleEffect_SpawnLinkedResourceObject_2(12, 256, 0);
        Event_Wait(110);
        Audio_PlayCue(60);
        Event_SetMessage(text_line + 3);
        Event_ShowMessage(12, 0);
        Event_Wait(30);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 1);
        Actor_StartRepeatedMotion(12, 1);
        Event_Wait(20);
        Actor_SetSpeed(12, 26214, 13107);
        ObjectMotion_ResetAndSetPositionInMode2_4(12, 520, 208);
        Actor_WaitForMove(12);
        Actor_SetAnimation(12, 1);
        Event_Wait(20);
        ObjectMotion_ArmCallback_8(12, 45056, 0);
        Event_Wait(30);
        ObjectMotion_ArmCallback_9(12, 20480, 0);
        Event_Wait(30);
        Actor_FaceActor(12, ACTOR_IVAN, 0);
        Event_Wait(20);
        Event_SetMessage(text_line + 4);
        Event_ShowMessage(12, 0);
        Event_Wait(40);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(20);
        BattleEffect_SpawnLinkedResourceObject_3(12, 264, 0);
        Event_Wait(120);
        Event_SetMessage(text_line + 5);
        Event_ShowMessage(12, 0);
        Event_Wait(25);
        Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
        Event_Wait(30);
        Actor_SetAnimationAndWait(12, 3);
        Event_Wait(40);
        ObjectMotion_ResetAndSetPositionInMode2_5(2, 480, 200);
        Actor_WaitForMove(ACTOR_IVAN);
        Actor_FaceEachOther(ACTOR_IVAN, 12, 0);
        Event_Wait(60);
        Event_SetMessage(text_line + 6);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_Wait(20);
        Actor_SetAnimation(12, 4);
        Event_Wait(80);
        Event_SetMessage(text_line + 7);
        Event_ShowMessage(12, 0);
        ObjectMotion_SetHorizontalPositionWithTerrain_2(13, 29884416, 20971520);
        Audio_PlayCue(19);
        Event_SetMessage(text_line + 8);
        Event_ShowMessage(13, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Event_Wait(5);
        Actor_FaceDirection(ACTOR_MIA, 16384, 0);
        Actor_FaceActor(12, 13, 0);
        Event_Wait(30);
        Audio_PlayCue(61);
        Camera_SetSpeed(131072, 16384);
        Camera_MoveToActor(13, 1);
        Camera_WaitForMove();
        Actor_SetSpeed(13, 52428, 26214);
        Actor_WalkTo(13, 456, 304);
        Camera_FollowActor(13, 1);
        Actor_WaitForMove(13);
        Camera_FollowActor(ACTOR_GERALD, 1);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
        Actor_ShowEmote(ACTOR_IVAN, 258, 0);
        Actor_ShowEmote(ACTOR_GERALD, 258, 0);
        Actor_ShowEmote(ACTOR_MIA, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_8(12, 258, 0);
        Event_Wait(60);
        Actor_FaceActor(12, 13, 0);
        Actor_StartRepeatedMotion(12, 2);
        Event_Wait(60);
        Event_SetMessage(text_line + 9);
        Event_ShowMessage(12, 0);
        Actor_FaceActor(13, 12, 0);
        Event_SetMessage(text_line + 10);
        Event_ShowMessage(13, 0);
        Event_Wait(60);
        Actor_FaceActor(13, ACTOR_IVAN, 0);
        Event_Wait(30);
        Event_SetMessage(text_line + 11);
        Event_ShowMessage(13, 0);
        Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
        Event_Wait(60);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Actor_StartRepeatedMotion(13, 1);
        Event_Wait(60);
        Event_SetMessage(text_line + 12);
        Event_ShowMessage(13, 0);
        BattleEffect_SpawnLinkedResourceObject_9(1, 259, 0);
        Event_Wait(60);
        Actor_SetAnimation(13, 4);
        Event_SetMessage(text_line + 13);
        Event_ShowMessage(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_7(1, 456, 248);
        Actor_WaitForMove(ACTOR_GERALD);
        ObjectMotion_ArmCallback_11(1, 16384, 0);
        Event_SetMessage(text_line + 14);
        Event_ShowMessage(ACTOR_GERALD, 0);
        ObjectMotion_ResetAndSetPositionInMode2_8(2, 472, 216);
        Actor_WaitForMove(ACTOR_IVAN);
        ObjectMotion_ArmCallback_12(2, 16384, 0);
        Event_Wait(10);
        Event_SetMessage(text_line + 15);
        Event_ShowMessage(ACTOR_IVAN, 0);
        BattleEffect_SpawnLinkedResourceObject_10(12, 261, 0);
        Event_Wait(60);
        Event_SetMessage(text_line + 16);
        Event_ShowMessage(12, 0);
        ObjectMotion_ResetAndSetPositionInMode2_9(3, 440, 216);
        Actor_WaitForMove(ACTOR_MIA);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Actor_SetAnimationAndWait(ACTOR_MIA, 3);
        Event_Wait(10);
        Event_SetMessage(text_line + 17);
        Event_ShowMessage(ACTOR_MIA, 0);
        Actor_SetAnimation(13, 4);
        Event_SetMessage(text_line + 18);
        Event_ShowMessage(13, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
        Actor_ShowEmote(ACTOR_GERALD, 258, 0);
        Actor_ShowEmote(ACTOR_MIA, 258, 0);
        Actor_ShowEmote(ACTOR_IVAN, 258, 0);
        BattleEffect_SpawnLinkedResourceObject_15(13, 264, 0);
        Event_Wait(60);
        Event_SetMessage(text_line + 19);
        Event_ShowMessage(13, 0);
        Event_Wait(20);
        BattleEffect_SpawnLinkedResourceObject_16(1, 259, 0);
        Event_Wait(60);
        Event_SetMessage(text_line + 20);
        Event_ShowMessage(ACTOR_GERALD, 0);
        Actor_StartRepeatedMotion(13, 1);
        Event_Wait(60);
        Event_SetMessage(text_line + 21);
        Event_ShowMessage(13, 0);
        ObjectMotion_ResetAndSetPositionInMode2_10(13, 456, 280);
        Actor_WaitForMove(13);
        ObjectMotion_ArmCallback_13(13, 20480, 0);
        Event_Wait(80);
        ObjectMotion_OffsetPositionAndResetMotion_3(13, -8, 8);
        Actor_WaitForMove(13);
        Event_Wait(60);
        Audio_PlayCue(155);
        Message_ShowCentered((s32)&LinkedMessage_DodonpaPulledLever, 1);
        Actor_SetDestinationOffset(13, 8, -8);
        Actor_FaceActor(13, 11, 0);
        FieldScene_RunScene3bf_020025f8();
        Audio_PlayCue(52);
        Event_SetMessage(text_line + 23);
        Event_ShowMessage(13, 0);
        Event_Wait(60);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
        Actor_FaceActor(ACTOR_GERALD, 11, 0);
        Actor_FaceActor(ACTOR_IVAN, 11, 0);
        Actor_FaceActor(ACTOR_MIA, 11, 0);
        Actor_FaceActor(12, 11, 0);
        FieldScene_RunScene3bf_02002718();
        FieldScene_RunScene3bf_0200269c();
        GameFlag_Set(2370);
        SHARED_RECORD_FIELD_448 = 512;
        Event_Wait(1);
        do {
            STATE_TABLE_022B = 3;
        } while (0);
        SharedWorkData_SetFirstAndSecondFields_2((s32)&Value_000000a3, 4);
        BattleEffect_ComputeWeightedResultAndDispatch_2(98, 4);
    }
    Event_End();
}

/* Long fixed sequence of setup, positioning, and per-actor animation calls
 * against actor slots 0-3, 11-15, driven by three script line tables, with
 * two two-way branches on the outcome of a query call. Ends by writing the
 * scene phase word and issuing a final batch of calls. */
void FieldScene_RunMainScriptSequence(void)
{
    u32 i;
    u8 *record;
    s32 script_a;
    s32 script_b;
    s32 script_c;

    GameFlag_Set(0x301);
    GameFlag_Set(0x941);
    Map_CopyCells(16, 75, 7, 4, 26, 55);
    PlaceSceneObjectPairFromTableA(4);
    Event_Begin();
    record = Scene_GetRecord_1_02003054(12);
    Actor_SetSpriteFlags(record, 1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1c80000, 0xb80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_SetPosition(ACTOR_GERALD, 0x1b80000, 0xc00000);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_SetPosition(ACTOR_MIA, 0x1e80000, 0xb80000);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Actor_SetPosition(ACTOR_IVAN, 0x1d80000, 0xb80000);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_SetPosition(12, 0x2080000, 0xe00000);
    Actor_FaceDirection(12, 0xb000, 0);
    Actor_SetPosition(11, 0x1c00000, 0xed0000);
    Actor_FaceDirection(11, 0x8000, 0);
    Actor_SetPosition(15, 0x1c00000, 0xee0000);
    Actor_SetSpritePriority(15, 3);
    Actor_SetAnimation(11, 3);
    Actor_SetPosition(13, 0x1ca0000, 0xf30000);
    Actor_FaceDirection(13, 0x4000, 0);
    Actor_SetAnimation(13, 5);
    Event_OpenScreen();
    Event_Wait(120);
    /* Script line bases are overlay data symbols: an integer base would be
     * constant-propagated into every offset instead of staying in r5. */
    script_a = (s32)LinkedMessage_UhnnGetOffMe;
    Event_SetMessage(script_a);
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 60);
    Event_SetMessage((script_a + 1));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 1);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 1);
    Event_Wait(60);
    Actor_FaceActor(12, 13, 0);
    Event_Wait(60);
    Event_SetMessage((script_a + 2));
    Event_ShowMessage(12, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Actor_FaceActor(ACTOR_IVAN, 12, 0);
    Actor_FaceActor(ACTOR_MIA, 12, 0);
    Event_Wait(60);
    Actor_WalkTo(12, 0x200, 232);
    Actor_SetAnimation(12, 4);
    Event_Wait(60);
    Event_SetMessage((script_a + 3));
    Event_ShowMessage(12, 0);
    Event_Wait(15);
    Actor_StartRepeatedMotion(13, 2);
    Event_SetMessage((script_a + 4));
    Event_ShowMessage(13, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_SetMessage((script_a + 5));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x105, 0);
    Actor_SetAnimation(ACTOR_MIA, 4);
    Event_Wait(80);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Event_SetMessage((script_a + 6));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(12, 0x102, 65);
    Actor_StartRepeatedMotion(12, 2);
    Event_Wait(100);
    Event_SetMessage((script_a + 7));
    Event_ShowMessage(13, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(10);
    Event_SetMessage((script_a + 8));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(30);
    Event_SetMessage((script_a + 9));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((script_a + 10));
    Event_OpenMessage(ACTOR_MIA, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_MIA, 3);
        Actor_FaceActor(12, 13, 0);
        Event_Wait(60);
    } else {
        Event_SetMessage((script_a + 11));
        Event_ShowMessage(ACTOR_MIA, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        Actor_FaceActor(ACTOR_GERALD, 12, 0);
        Actor_FaceActor(ACTOR_IVAN, 12, 0);
        Actor_FaceActor(ACTOR_MIA, 12, 0);
        Event_Wait(20);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
        Event_Wait(60);
        Actor_FaceActor(12, 13, 0);
        Event_Wait(80);
        Actor_SetAnimationAndWait(12, 3);
        Event_Wait(30);
        Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
        Event_SetMessage((script_a + 12));
        Event_ShowMessage(12, 0);
        Event_Wait(60);
        Actor_FaceActor(ACTOR_IVAN, 12, 0);
        Actor_FaceActor(ACTOR_GERALD, 12, 0);
        Actor_FaceActor(ACTOR_MIA, 12, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 12, 0);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        Actor_SetAnimation(ACTOR_GERALD, 3);
        Actor_SetAnimation(ACTOR_MIA, 3);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        Event_Wait(80);
        Actor_FaceActor(12, 13, 0);
        Actor_FaceActor(ACTOR_IVAN, 13, 0);
        Actor_FaceActor(ACTOR_GERALD, 13, 0);
        Actor_FaceActor(ACTOR_MIA, 13, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_MIA, 0x8000, 0x4000);
    Actor_SetSpeed(ACTOR_IVAN, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_GERALD, 0x1a0, 216);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1a0, 248);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 248);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1b8, 216);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 248);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_WalkTo(ACTOR_MIA, 0x1c8, 248);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Actor_WalkTo(ACTOR_IVAN, 0x1c8, 216);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0x8000, 0);
    Actor_FaceEachOther(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Event_Wait(60);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(100);
    Audio_PlayCue(226);
    Actor_SetAnimation(13, 7);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, -24, 0);
    Actor_SetSpeed(ACTOR_GERALD, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_GERALD, -24, 0);
    Actor_SetSpeed(ACTOR_MIA, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_MIA, -24, 0);
    Actor_SetSpeed(ACTOR_IVAN, 0x1999, 0xccc);
    Actor_SetDestinationOffset(ACTOR_IVAN, -24, 0);
    Actor_SetSpeed(11, 0x1999, 0xccc);
    Actor_SetSpeed(15, 0x1999, 0xccc);
    Actor_SetDestinationOffset(11, -24, 0);
    Actor_SetDestinationOffset(15, -24, 0);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Audio_PlayCue(0x120);
    Event_Wait(60);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_FaceDirection(ACTOR_MIA, 0xc000, 0);
    Event_Wait(60);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(100);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_GERALD, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_IVAN, 0xb333, 0x5999);
    Actor_SetSpeed(ACTOR_MIA, 0xb333, 0x5999);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_WalkTo(ACTOR_GERALD, 0x1d0, 0x100);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1e0, 248);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 192);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceDirection(ACTOR_GERALD, 0x4000, 0);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 248);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_WalkTo(ACTOR_MIA, 0x1e8, 184);
    Actor_WalkTo(ACTOR_IVAN, 0x1d8, 184);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetAnimation(ACTOR_MIA, 1);
    Actor_FaceDirection(ACTOR_MIA, 0x4000, 0);
    Event_Wait(30);
    Event_Wait(60);
    script_b = (s32)LinkedMessage_ThereYouShouldAbleGet;
    Event_SetMessage(script_b);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimation(13, 6);
    Event_Wait(120);
    Actor_StartRepeatedMotion(13, 2);
    Event_Wait(60);
    Actor_SetAnimation(13, 7);
    Event_SetMessage((script_b + 1));
    Event_ShowMessage(13, 0);
    Event_Wait(20);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Event_Wait(10);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 80);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Event_SetMessage((script_b + 2));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_ShowEmote(13, 0x102, 70);
    Actor_StartRepeatedMotion(13, 2);
    Event_Wait(60);
    Actor_SetAnimation(13, 5);
    Event_Wait(70);
    Event_SetMessage((script_b + 3));
    Event_ShowMessage(13, 0);
    Actor_SetAnimation(13, 7);
    Actor_ShowEmote(ACTOR_IVAN, 0x108, 40);
    Event_SetMessage((script_b + 4));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_SetAnimationAndWait(12, 3);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 60);
    Actor_WalkTo(ACTOR_GERALD, 0x1b8, 208);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Event_SetMessage((script_b + 5));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Actor_ShowEmote(12, 0x101, 0);
    Event_Wait(70);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 75);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Event_SetMessage((script_b + 6));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(12, 0x101, 0);
    Event_Wait(60);
    Event_SetMessage((script_b + 7));
    Event_ShowMessage(12, 0);
    Actor_WalkTo(ACTOR_GERALD, 0x1c0, 208);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_SetMessage((script_b + 8));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(12, 1);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GERALD, 12, 0);
    Event_Wait(60);
    Event_SetMessage((script_b + 9));
    Event_ShowMessage(12, 0);
    Actor_SetAnimation(ACTOR_GERALD, 4);
    Event_Wait(60);
    Event_SetMessage((script_b + 10));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(30);
    Event_SetMessage((script_b + 11));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_MIA, 0);
    Event_Wait(20);
    Event_SetMessage((script_b + 12));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Event_Wait(30);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 80);
    Actor_FaceActor(ACTOR_MIA, ACTOR_GERALD, 0);
    Event_SetMessage((script_b + 13));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Event_SetMessage((script_b + 14));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_IVAN, 13, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Event_Wait(120);
    Actor_ShowEmote(13, 0x102, 30);
    Actor_StartRepeatedMotion(13, 1);
    Event_Wait(120);
    Actor_StartRepeatedMotion(12, 1);
    Event_Wait(60);
    Event_SetMessage((script_b + 15));
    Event_ShowMessage(12, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x107, 110);
    Actor_WalkTo(ACTOR_GERALD, 0x1c8, 212);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Event_SetMessage((script_b + 16));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Event_Wait(60);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_SetMessage((script_b + 17));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Event_Wait(80);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_GERALD, 13, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Actor_FaceActor(ACTOR_MIA, 13, 0);
    Actor_FaceActor(13, 13, 0);
    Actor_FaceActor(12, 13, 0);
    Event_Wait(80);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 3);
    Event_Wait(30);
    Event_SetMessage((script_b + 18));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(13, 2);
    Event_Wait(70);
    Event_SetMessage((script_b + 19));
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 60);
    Event_SetMessage((script_b + 20));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(80);
    Event_SetMessage((script_b + 21));
    Event_OpenMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_MIA, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(14, 0x1c80000, 0x1300000);
    Actor_SetSpeed(14, 0x8000, 0x4000);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Audio_PlayCue(19);
        Event_SetMessage((script_b + 22));
        Event_ShowMessage(14, 0);
    } else {
        Audio_PlayCue(19);
        Event_SetMessage((script_b + 23));
        Event_ShowMessage(14, 0);
    }
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Actor_FaceActor(ACTOR_GERALD, 14, 0);
    Actor_FaceActor(ACTOR_MIA, 14, 0);
    Actor_FaceActor(ACTOR_IVAN, 14, 0);
    Actor_FaceActor(12, 14, 0);
    Actor_FaceActor(13, 14, 0);
    Camera_MoveTo(0x1c80000, -1, 0xf00000, 1);
    Actor_StartRepeatedMotion(13, 1);
    Event_Wait(60);
    Audio_PlayCue(8);
    script_c = (s32)LinkedMessage_Dad;
    Event_SetMessage(script_c);
    Event_ShowMessage(13, 0);
    Actor_WalkTo(14, 0x1c8, 0x118);
    Actor_WaitForMove(14);
    Actor_WalkTo(14, 0x1b8, 0x100);
    Camera_MoveTo(0x1c80000, -1, 0xe00000, 1);
    Event_SetMessage((script_c + 1));
    Event_ShowMessage(14, 0);
    Actor_WaitForMove(14);
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_ShowEmote(13, 0x102, 80);
    Actor_SetAnimation(14, 4);
    Event_Wait(89);
    Event_SetMessage((script_c + 2));
    Event_ShowMessage(14, 0);
    Actor_RunRepeatedMotion(13, 2);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((script_c + 3));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(13, 0x100, 80);
    Actor_SetAnimation(14, 4);
    Event_Wait(80);
    Actor_FaceDirection(14, 0x3000, 0);
    Event_Wait(20);
    Event_SetMessage((script_c + 4));
    Event_ShowMessage(14, 0);
    Actor_StartRepeatedMotion(13, 1);
    Event_SetMessage((script_c + 5));
    Event_ShowMessage(13, 0);
    Event_Wait(30);
    Actor_SetAnimationAndWait(14, 3);
    Actor_FaceActor(14, 13, 0);
    Event_Wait(20);
    Event_SetMessage((script_c + 6));
    Event_ShowMessage(14, 0);
    Actor_StartRepeatedMotion(13, 1);
    Actor_ShowEmote(13, 0x102, 80);
    Event_SetMessage((script_c + 7));
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(14, 0x103, 60);
    Event_SetMessage((script_c + 8));
    Event_ShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((script_c + 9));
    Event_ShowMessage(14, 0);
    Event_Wait(20);
    Actor_SetAttachedEffect(13, 0x101);
    Event_Wait(80);
    Actor_SetAttachedEffect(13, 0);
    Actor_WalkTo(14, 0x1f0, 240);
    Actor_WaitForMove(14);
    Actor_FaceActor(14, 12, 0);
    Event_Wait(20);
    Actor_FaceActor(12, 14, 0);
    Event_SetMessage((script_c + 10));
    Event_ShowMessage(14, 0);
    Actor_StartRepeatedMotion(12, 1);
    Event_SetMessage((script_c + 11));
    Event_ShowMessage(12, 0);
    Event_Wait(40);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_SetMessage((script_c + 12));
    Event_ShowMessage(14, 0);
    Actor_SetAnimation(14, 3);
    Actor_ShowEmote(12, 0x102, 60);
    Event_SetMessage((script_c + 13));
    Event_ShowMessage(12, 0);
    Actor_ShowEmote(14, 0x100, 70);
    Event_SetMessage((script_c + 14));
    Event_ShowMessage(14, 0);
    Actor_SetAnimation(12, 3);
    Event_Wait(140);
    Actor_SetAnimation(14, 3);
    Event_Wait(120);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 14, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(120);
    Actor_ShowEmote(14, 0x108, 180);
    Event_SetMessage((script_c + 15));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(12, 0x101, 80);
    Event_SetMessage((script_c + 16));
    Event_ShowMessage(12, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_GERALD, 0);
    Actor_FaceEachOther(ACTOR_MIA, ACTOR_IVAN, 0);
    Event_Wait(80);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(60);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Actor_FaceActor(ACTOR_GERALD, 14, 0);
    Actor_FaceActor(ACTOR_IVAN, 14, 0);
    Actor_FaceActor(ACTOR_MIA, 14, 0);
    Event_Wait(60);
    Event_SetMessage((script_c + 17));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Event_Wait(100);
    Event_SetMessage((script_c + 18));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(60);
    Event_SetMessage((script_c + 19));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(14, ACTOR_IVAN, 0);
    Event_Wait(20);
    Event_SetMessage((script_c + 20));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 0);
    Event_Wait(120);
    Actor_SetAnimation(14, 4);
    Event_Wait(120);
    Event_SetMessage((script_c + 21));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(14, 0x102, 90);
    Event_SetMessage((script_c + 22));
    Event_ShowMessage(14, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(12, 3);
    Event_Wait(80);
    Actor_SetAnimation(14, 4);
    Event_Wait(120);
    Event_SetMessage((script_c + 23));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(12, 0x100, 60);
    Event_SetMessage((script_c + 24));
    Event_ShowMessage(12, 0);
    Actor_FaceActor(14, 12, 0);
    Event_Wait(20);
    Actor_StartRepeatedMotion(14, 1);
    Event_Wait(50);
    Event_SetMessage((script_c + 25));
    Event_ShowMessage(14, 0);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Event_Wait(70);
    Event_SetMessage((script_c + 26));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_SetMessage((script_c + 27));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_SetMessage((script_c + 28));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 90);
    Event_SetMessage((script_c + 29));
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceDirection(14, 0x5000, 0);
    Event_Wait(20);
    Event_SetMessage((script_c + 30));
    Event_ShowMessage(14, 0);
    Actor_StartRepeatedMotion(12, 1);
    Event_Wait(60);
    Event_SetMessage((script_c + 31));
    Event_ShowMessage(12, 0);
    Actor_FaceActor(14, 12, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(20);
    Event_SetMessage((script_c + 32));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 70);
    Event_SetMessage((script_c + 33));
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Event_SetMessage((script_c + 34));
    Event_ShowMessage(14, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x100, 80);
    Event_SetMessage((script_c + 35));
    Event_ShowMessage(ACTOR_MIA, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((script_c + 36));
    Event_ShowMessage(14, 0);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_PARTY_LEADER, 0);
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Event_Wait(100);
    Event_SetMessage((script_c + 37));
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_SetMessage((script_c + 38));
        Event_ShowMessage(14, 0);
    }
    Event_Wait(30);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimation(ACTOR_MIA, 3);
    Event_Wait(100);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 12, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(12, 3);
    Event_Wait(100);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = Scene_GetRecord_2_02003054(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetAnimation(ACTOR_IVAN, 2);
    record = Scene_GetRecord_3(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetAnimation(ACTOR_MIA, 2);
    record = Scene_GetRecord_4(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_MIA, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_MIA);
    Actor_SetPosition(ACTOR_MIA, 0, 0);
    Event_Wait(30);
    Actor_SetSpeed(12, 0x6666, 0x3333);
    Actor_WalkTo(12, 0x1d8, 184);
    Actor_WaitForMove(12);
    Actor_SetAnimation(12, 1);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_SetAnimation(14, 3);
    Event_Wait(20);
    Actor_FaceActor(14, 13, 0);
    Event_Wait(20);
    Event_SetMessage((script_c + 39));
    Event_ShowMessage(14, 0);
    Actor_RunRepeatedMotion(13, 2);
    Event_SetMessage((script_c + 40));
    Event_ShowMessage(13, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_SetSpeed(12, 0x10000, 0x8000);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1e0, 248);
    Event_Wait(40);
    Actor_SetSpritePriority(12, 0);
    Actor_SetSpritePriority(ACTOR_PARTY_LEADER, 0);
    Actor_WalkTo(12, 0x1e0, 216);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    Actor_WaitForMove(12);
    Actor_FaceActor(12, 14, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Event_Wait(5);
    Actor_SetAnimation(12, 3);
    Event_Wait(100);
    Actor_SetAnimation(14, 3);
    Event_Wait(100);
    Actor_WalkTo(12, 0x1e0, 248);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 248);
    Actor_WaitForMove(12);
    Actor_WalkTo(12, 0x1c8, 248);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1c8, 0x168);
    Actor_WaitForMove(12);
    Actor_WalkTo(12, 0x1c8, 0x168);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x160, 0x168);
    Actor_WaitForMove(12);
    Actor_WalkTo(12, 0x160, 0x168);
    Event_Wait(20);
    Audio_PlayCue(17);
    SCENE_PHASE_02003054 = 0x203;
    Event_CloseScreen();
    Event_Wait(1);
    Event_Wait(210);
    Event_RequestExit(4);
    Event_End();
}

void FieldScene_SelectActorTwentyOneMessage(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    switch (Data_0200dfa4) {
    case 0:
        Event_SetMessage(0x2414);
        Event_ShowMessage(21, 0);
        break;
    case 1:
        Event_SetMessage(0x2415);
        Event_ShowMessage(21, 0);
        break;
    case 2:
        Event_SetMessage(0x2416);
        Event_ShowMessage(21, 0);
        break;
    case 3:
        Event_SetMessage(0x2417);
        Event_ShowMessage(21, 0);
        break;
    case 4:
        Event_SetMessage(0x2418);
        Event_ShowMessage(21, 0);
        break;
    case 6:
        Event_SetMessage(0x241a);
        Event_ShowMessage(21, 0);
        break;
    case 7:
        Event_SetMessage(0x241b);
        Event_ShowMessage(21, 0);
        break;
    case 5:
        Actor_FaceDirection(21, 0xd000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0xb000, 0);
        Event_Wait(50);
        Actor_FaceDirection(21, 0x5000, 0);
        Event_Wait(50);
        Event_SetMessage(0x2419);
        Event_ShowMessage(21, 0);
        break;
    }
}

void FieldScene_RunActorTwentyOneSequence(void)
{
    extern u32 Data_0200dfa4;
    extern u8 Data_03001e70[];

    s32 base5_2411;

    Actor_ShowEmote(21, 0x101, 30);
    Actor_FaceDirection(21, 0xd000, 0);
    Event_Wait(50);
    Actor_FaceDirection(21, 0xb000, 0);
    Event_Wait(50);
    Actor_FaceDirection(21, 0x5000, 0);
    Event_Wait(50);
    base5_2411 = (s32)LinkedMessage_CouldSwearJustHeardSomeone;
    Event_SetMessage(base5_2411);
    Event_ShowMessage(21, 0);
    Actor_SetAnimation(21, 4);
    Event_Wait(60);
    Actor_FaceDirection(21, 0xb000, 0);
    Event_Wait(40);
    Event_SetMessage((base5_2411 + 1));
    Event_ShowMessage(21, 0);
}

void FieldScene_RunScene3bf_02004794(void)
{
    u32 i;
    s32 record;
    s32 base5_244f;
    s32 base5_2455;

    Event_Begin();
    if (GameFlag_IsSet(0x941) != 0) {
        Event_SetMessage(0x2566);
        Event_ShowMessage(18, 0);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x313) != 0) {
            Event_SetMessage(0x2457);
            Event_OpenMessage(25, 0);
            Event_End();
        } else {
            Actor_ShowEmote(25, 0x102, 30);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            base5_244f = (s32)LinkedMessage_ShhhPleaseDontWakeDonpa;
            Event_SetMessage(base5_244f);
            Event_ShowMessage(25, 0);
            Actor_FaceActor(25, 24, 0);
            Camera_MoveToActor(24, 1);
            Camera_WaitForMove();
            Event_Wait(60);
            Camera_MoveToActor(0, 1);
            Event_Wait(20);
            Actor_ShowEmote(25, 0x105, 60);
            Event_SetMessage((base5_244f + 1));
            Event_ShowMessage(25, 0);
            Actor_ShowEmote(25, 0x107, 60);
            Event_SetMessage((base5_244f + 2));
            Event_ShowMessage(25, 0);
            Event_Wait(70);
            Actor_ShowEmote(25, 0x100, 60);
            Actor_FaceActor(25, ACTOR_PARTY_LEADER, 0);
            Event_SetMessage((base5_244f + 3));
            Event_OpenMessage(25, 0);
            if (Event_ChooseYesNo(0, 0) == 0) {
                Event_SetMessage((base5_244f + 4));
                Event_OpenMessage(25, 0);
            } else {
                Event_SetMessage((base5_244f + 5));
                Event_OpenMessage(25, 0);
            }
            Event_Wait(60);
            Actor_ShowEmote(25, 0x105, 60);
            base5_2455 = (s32)LinkedMessage_DonpaKnowsHeCoddledDodonpa;
            Event_SetMessage(base5_2455);
            Event_OpenMessage(25, 0);
            Actor_RunRepeatedMotion(25, 1);
            Event_SetMessage((base5_2455 + 1));
            Event_OpenMessage(25, 0);
            Actor_SetAnimationAndWait(25, 3);
            Event_SetMessage((base5_2455 + 2));
            Event_OpenMessage(25, 0);
            GameFlag_Set(0x313);
            Event_End();
        }
    }
}

void SelectActor25SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage(0x2568);
        Event_ShowMessage(25, 0);
    } else {
        Event_SetMessage(0x2458);
        Event_ShowMessage(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (GameFlag_IsSet(0x941)) {
        Event_SetMessage(0x2569);
        Event_ShowMessage(24, 0);
    } else {
        Event_SetMessage(0x244e);
        Event_ShowMessage(24, 0);
    }
}

/* Runs a gated sequence of parameterized calls on PRIMARY_ID (24) and, once
 * derived partway through, DERIVED_ID (25); a sequence id counting up from
 * 9569 is threaded through four of the calls one apart. Each of the two
 * outer gating checks has its own short fallback branch on PRIMARY_ID. */
void FieldScene_RunSupplementalSequenceTwo(void)
{
    s32 sequence_id;

    if (GameFlag_IsSet(2369) != 0) {
        if (GameFlag_IsSet(2382) == 0 && GameFlag_IsSet(788) == 0) {
        sequence_id = 9569;
        Event_SetMessage(sequence_id);
        Event_ShowMessage(PRIMARY_ID, 0);
        ObjectMotion_SetVariantCallbackAndInvokeObject_1_020049a0(PRIMARY_ID, 1);
        BattleRuntime_WaitIfModeZero_1_020049a0(30);
        Actor_SetSpeed(PRIMARY_ID, 6553, 3276);
        ObjectMotion_OffsetPositionAndResetMotion_1_020049a0(PRIMARY_ID, -4, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_1_020049a0(PRIMARY_ID);
        Object_SetModeById_1_020049a0(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_2_020049a0(60);
        Actor_SetSpeed(PRIMARY_ID, 13107, 6553);
        Actor_SetDestinationOffset(PRIMARY_ID, -6, 0);
        ObjectMotion_SetAngleToward_1_020049a0(PRIMARY_ID, 0, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_2_020049a0(PRIMARY_ID);
        SceneWork_SetStepValue_2_020049a0(sequence_id + 1);
        Event_ShowMessage(PRIMARY_ID, 0);
        Actor_RunRepeatedMotion(PRIMARY_ID, 1);
        ObjectMotion_SetAngleToward_2_020049a0(DERIVED_ID, PRIMARY_ID, 0);
        SceneWork_SetStepValue_3_020049a0(sequence_id + 2);
        BattleEvent_RunActionAndWait_3_020049a0(PRIMARY_ID, 0);
        BattleRuntime_WaitIfModeZero_3_020049a0(70);
        Object_SetModeById_2_020049a0(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_4_020049a0(60);
        Actor_SetSpeed(DERIVED_ID, 26214, 13107);
        ObjectMotion_ResetAndSetPositionInMode2_1_020049a0(DERIVED_ID, 880, 112);
        ObjectMotion_CommitCurrentPositionAndActivate_3_020049a0(DERIVED_ID);
        ObjectMotion_ArmCallback_1_020049a0(DERIVED_ID, 53248, 0);
        SceneWork_SetStepValue_4_020049a0(sequence_id + 3);
        Event_ShowMessage(PRIMARY_ID, 0);
        Object_SetModeById_3_020049a0(PRIMARY_ID, 3);
        BattleRuntime_WaitIfModeZero_5_020049a0(70);
        ObjectMotion_OffsetPositionAndResetMotion_3_020049a0(PRIMARY_ID, 8, 0);
        ObjectMotion_CommitCurrentPositionAndActivate_4_020049a0(PRIMARY_ID);
        Object_SetModeById_4_020049a0(PRIMARY_ID, 5);
        SceneWork_SetStepValue_5_020049a0(sequence_id + 4);
        Event_ShowMessage(PRIMARY_ID, 0);
        ObjectMotion_ResetAndSetPositionInMode2_2_020049a0(0, 896, 120);
        ObjectMotion_CommitCurrentPositionAndActivate_5_020049a0(0);
        Object_LinkPair_1_020049a0(0, DERIVED_ID, 0);
        BattleRuntime_WaitIfModeZero_6_020049a0(60);
        Actor_SetAnimation(DERIVED_ID, 3);
        BattleRuntime_WaitIfModeZero_7_020049a0(30);
        GameFlag_Set(788);
        } else {
            Event_SetMessage(9575);
            Event_ShowMessage(PRIMARY_ID, 0);
        }
    } else {
        Event_SetMessage(0x244d);
        Event_ShowMessage(PRIMARY_ID, 0);
    }
}

void ConfigureSceneActor26(void)
{
    Func_0200a30a(26, 1, 5);
    GameFlag_Set(0x94e);
}

void ConfigureSceneActor14(void)
{
    Actor_RunRepeatedMotion(14, 2);
    Event_SetMessage(0x2441);
    Event_ShowMessage(14, 0);
}

void ConfigureSceneActor13(void)
{
    Actor_RunRepeatedMotion(13, 2);
    Event_SetMessage((s32)&LinkedMessage_AllRightAllRightWe);
    Event_ShowMessage(13, 0);
}

void ConfigureSceneActor12Variant(void)
{
    Actor_RunRepeatedMotion(12, 2);
    Event_SetMessage(0x243f);
    Event_ShowMessage(12, 0);
}

void ConfigureSceneActor18(void)
{
    Event_SetMessage((s32)&LinkedMessage_DoYouKnowWhereDodonpa);
    Event_AskYesNo(18, 0);
}

void RunActor20SceneSequence(void)
{
    u32 i;
    s32 record;
    s32 base5_242e;
    s32 base5_2430;

    if (GameFlag_IsSet(0x226) != 0) {
        Event_SetMessage(0x2434);
        Event_ShowMessage(20, 0);
    } else {
        Event_Begin();
        Actor_FaceActor(20, ACTOR_PARTY_LEADER, 0);
        if (GameFlag_IsSet(0x227) == 0) {
            Actor_Jump(20, 4, 0);
            Actor_Stop(20);
            Func_0200a2ac(20);
            Event_Wait(20);
            base5_242e = (s32)LinkedMessage_WhWhWhoGoesThere;
            Event_SetMessage(base5_242e);
            Event_ShowMessage(20, 0);
            Actor_ShowEmote(20, 0x102, 30);
            Event_SetMessage((base5_242e + 1));
            Event_ShowMessage(20, 0);
            Event_Wait(30);
            Actor_SetAnimation(20, 4);
            Event_Wait(30);
        }
        base5_2430 = (s32)LinkedMessage_YouDontLookNearlyAs;
        Event_SetMessage(base5_2430);
        Event_ShowMessage(20, 0);
        Actor_ShowEmote(20, 0x101, 40);
        Event_SetMessage((base5_2430 + 1));
        Event_OpenMessage(20, 0);
        if (Event_ChooseYesNo(0, 0) == 0) {
            Event_SetMessage((base5_2430 + 2));
            Event_OpenMessage(20, 0);
            GameFlag_Set(0x226);
        } else {
            Event_SetMessage((base5_2430 + 3));
            Event_OpenMessage(20, 0);
        }
        GameFlag_Set(0x227);
        Event_End();
    }
}

void FinishActor20SceneSequence(void)
{
    extern u8 *Data_03001ebc;

    if (GameFlag_IsSet(0x226)) {
        Event_SetMessage(0x2435);
        Event_ShowMessage(20, 0);
    } else {
        s16 *q = (s16 *)(Data_03001ebc + 382);

        *q = 0;
        Psynergy_Cancel();
        RunActor20SceneSequence();
    }
}

void NoOpActorCallback(void)
{
}

void ConfigureActor13Interaction(void)
{
    u8 *interaction_resources = &Value_0000256c;

    Event_SetMessage((s32)interaction_resources);
    Event_ShowMessage(0x800d, 0);
    if (Func_0200a36e(234) != -1) {
        Message_ShowCentered((s32)(interaction_resources + 2), 1);
    }
}

void ConfigureActor13SceneResource(void)
{
    Event_SetMessage(0x256d);
    Event_ShowMessage(13, 0);
}

#include "OBJECT_RUNTIME.H"

struct DispatcherEventRuntime {
    u8 unknown_000[0x1c0];
    s32 value_1c0;
};

union DispatcherEventWork {
    struct {
        u8 unknown_000[0x1c0];
        u16 first;
        u16 second;
    } pair;
    u32 words[0x1c4 / 4];
};
extern union DispatcherEventWork Dispatch_Data_02000240;
extern u32 Dispatch_Data_0200dfa4;
extern struct DispatcherEventRuntime *Dispatch_Data_03001ebc;
extern u8 Dispatch_SceneEvent_InstallTasks[];
extern u8 Dispatch_SceneEvent_SetupActors[];
extern u8 Dispatch_SceneEvent_RestoreActors[];
extern u8 Dispatch_SceneEvent_UpdateActors[];
extern void Dispatch_Func_0200d5f8(u32);
extern void Dispatch_Func_0200cf60(void);
extern void Dispatch_Func_0200d0e4(void);
extern void Dispatch_Func_0200d324(void);
extern struct ObjectRuntime *Dispatch_Func_0200d650(s32);
extern void Dispatch_Func_0200d5e8(struct ObjectRuntime *, s32);
extern void Dispatch_Func_0200d708(s32, s32, s32);
extern void Dispatch_Func_0200d6a8(s32, s32);
extern void Dispatch_Func_0200d520(void);
extern void Dispatch_Func_0200b054(void);
extern s32 Dispatch_Func_0200d610(s32);
extern void Dispatch_Func_0200d6a0(s32, s32, s32);

static __inline__ void Scene_Call1(void (*func)(u32), u32 value)
{
    func(value);
}

static __inline__ void Scene_Call3(
    void (*func)(s32, s32, s32), s32 id, s32 x, s32 z)
{
    func(id, x, z);
}

s32 FieldScene_DispatchActorUpdate(void)
{
    struct ObjectRuntime *actor;

    Dispatch_Data_0200dfa4 = (u32)Random_Next() * 7 >> 16;
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_InstallTasks) {
        Scene_Call1(Dispatch_Func_0200d5f8, 0xe00);
        Dispatch_Func_0200cf60();
    }
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_SetupActors)
        Dispatch_Func_0200d0e4();
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_RestoreActors)
        Dispatch_Func_0200d324();
    if ((s16)Dispatch_Data_02000240.pair.first == (s32)Dispatch_SceneEvent_UpdateActors) {
        Dispatch_Data_03001ebc->value_1c0 = 0x204;
        Dispatch_Func_0200d5e8(Dispatch_Func_0200d650(12), 0);
        Dispatch_Func_0200d708(12, 0, 0);
        Dispatch_Func_0200d6a8(12, 0);
        Dispatch_Func_0200d520();
        actor = Dispatch_Func_0200d650(8);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Dispatch_Func_0200d650(9);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        actor = Dispatch_Func_0200d650(10);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        Scene_Call1(Dispatch_Func_0200d5f8, 0xe00);
        if ((s16)Dispatch_Data_02000240.pair.second == 4) {
            Scene_Call1(Dispatch_Func_0200d5f8, 0xc00);
            Dispatch_Func_0200b054();
        }
        if ((s16)Dispatch_Data_02000240.pair.second == 3) {
            Scene_Call1(Dispatch_Func_0200d5f8, 0xc00);
            if (Dispatch_Func_0200d610(0x941) != 0) {
                Dispatch_Func_0200d6a0(12, 0, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 16, 0x1b00000, 0x1580000);
                Scene_Call3(Dispatch_Func_0200d708, 16, 0x5000, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 13, 0x1c80000, 0x1200000);
                Scene_Call3(Dispatch_Func_0200d708, 13, 0x5000, 0);
                Scene_Call3(Dispatch_Func_0200d6a0, 17, 0x1c80000, 0x1400000);
                Dispatch_Func_0200d5e8(Dispatch_Func_0200d650(17), 0);
            }
        }
        actor = Dispatch_Func_0200d650(15);
        if (actor != 0)
            Dispatch_Func_0200d5e8(actor, 0);
        actor->unknown_23 = 2;
        *(s32 *)&actor->unknown_18[0] = 0xcccc;
    }
    return 0;
}

void FieldScene_InstallSceneTasks(void)
{
    FieldScene_ActivateThreeActorGroup();
    switch (gGameState.entrance) {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2_02004f60(Func_0200a572, (s32)Data_02009719, 3200);
        Value2_02004f60(Func_0200a57c, (s32)Data_0200975d, 3200);
        Value2_02004f60(Func_0200a586, (s32)Data_020097bd, 3200);
        Func_0200a650(0xe00);
        break;
    case 12:
    case 19:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Func_0200a650(0xc00);
        break;
    case 16:
    case 17:
    case 18:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2_02004f60(Func_0200a5b2, (s32)Data_02009529, 3200);
        Value2_02004f60(Func_0200a5bc, (s32)Data_020099e9, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        Map_CopyCells(101, 9, 10, 8, 110, 9);
        Func_0200a650(0xe00);
        break;
    case 13:
    case 14:
    case 15:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Value2_02004f60(Func_0200a602, (s32)Data_0200969d, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Func_0200a682(0xe00);
        break;
    }
    Actor_SetChildValue(18, 1);
    Actor_SetChildValue(17, 1);
    Actor_SetChildValue(21, 1);
    Actor_SetChildValue(12, 1);
    Actor_SetChildValue(13, 1);
    Task_Wait(1);
}

void FieldScene_SetupActorsForScene(void)
{
    struct ObjectRuntime *actor;

    FieldScene_ActivateTwoActorGroup();
    Actor_SetChildValue(9, 1);
    Actor_SetChildValue(10, 1);
    Actor_SetChildValue(17, 1);
    if (GameFlag_IsSet(0x94c)) {
        Actor_SetPosition(15, 0, 0);
    }
    if (GameFlag_IsSet(0x949)) {
        Actor_SetPosition(11, 0, 0);
    }
    if (GameFlag_IsSet(0x94b)) {
        Actor_SetPosition(16, 0, 0);
    }
    if (GameFlag_IsSet(0xf2e)) {
        Actor_SetPosition(8, 0, 0);
    }
    switch (gGameState.entrance) {
    case 1:
    case 2:
    case 3:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Func_0200a7f8(0xe00);
        Func_0200a7f6(Data_02009151, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        break;
    case 10:
    case 13:
    case 20:
    case 23:
    case 24:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        Func_0200a816(0xc00);
        Actor_SetSpriteFlags(Func_0200a874(24), 0);
        if (GameFlag_IsSet(0x314)) {
            Actor_SetPosition(25, 0x3680000, 0x780000);
        }
        break;
    case 21:
    case 22:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Func_0200a854(0xe00);
        Func_0200a7f6(Data_0200938d, 3200);
        Task_Wait(1);
        Map_Redraw();
        Task_Wait(1);
        break;
    case 11:
    case 12:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        if (GameFlag_IsSet(0x94a)) {
            FieldScene_RunSequenceTail();
        }
        break;
    case 31:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        FieldScene_RunSequenceTail();
        break;
    case 14:
    case 15:
    case 16:
        Func_0200a844(Data_02009a45, 3200);
        break;
    default:
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
        Func_0200a8c4(0xe00);
        break;
    }
    actor = Func_0200a922(8);
    Actor_SetSpriteFlags(Func_0200a92a(8), 0);
    Actor_SetSpritePriority(8, 1);
    *(s32 *)&actor->unknown_18[0] = 0xc000;
    *(s32 *)&actor->unknown_18[4] = 0xc000;
}

void FieldScene_RestoreActorsFromFlags(void)
{
    struct ObjectRuntime *actor;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    FieldScene_ActivateAlternateActorGroup();
    if (GameFlag_IsSet(0x943)) {
        PlaceActorTwelveAndFinishScene();
    }
    GameFlag_Set(0x217);
    GameFlag_Set(0x218);
    if (GameFlag_IsSet(0x944)) {
        Actor_SetPosition(8, 0, 0);
        GameFlag_Clear(0x217);
    }
    if (GameFlag_IsSet(0x945)) {
        Actor_SetPosition(9, 0, 0);
        ConfigureInteractionRegionC();
    }
    if (GameFlag_IsSet(0x946)) {
        Actor_SetPosition(10, 0, 0);
        GameFlag_Clear(0x218);
    }
    if (GameFlag_IsSet(0x947)) {
        ConfigureInteractionRegionA();
    }
    if (GameFlag_IsSet(0x948)) {
        ConfigureInteractionRegionB();
    }
    Event_Begin();
    actor = Func_0200aa16(8);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa2a(9);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Village_GetActor(10);
    if (actor != 0) {
        actor->unknown_23 = 2;
    }
    actor = Func_0200aa52(11);
    if (actor != 0) {
        Actor_SetSpriteFlags(actor, 0);
    }
    actor->unknown_23 = 2;
    actor = Func_0200aa6c(12);
    if (actor != 0) {
        actor->unknown_56[3] |= 0x10;
    }
    Actor_SetSpriteFlags(Func_0200aa84(11), 0);
    Event_End();
    Village_FinishActorRestore(0xe00);
}

void FieldScene_ActivateThreeActorGroup(void)
{
    if (GameFlag_IsSet(0x35a)) {
        PlaceSceneObjectPairFromTableC(0);
    }
    if (GameFlag_IsSet(0x35b)) {
        PlaceSceneObjectPairFromTableC(1);
    }
    if (GameFlag_IsSet(0x35c)) {
        PlaceSceneObjectPairFromTableC(2);
    }
}

void FieldScene_ActivateTwoActorGroup(void)
{
    if (GameFlag_IsSet(0x358)) {
        PlaceSceneObjectPairFromTableB(0);
    }
    if (GameFlag_IsSet(0x359)) {
        PlaceSceneObjectPairFromTableB(1);
    }
}

void FieldScene_ActivateAlternateActorGroup(void)
{
    if (GameFlag_IsSet(0x355)) {
        FieldScene_SetPositionPairs(0);
    }
    if (GameFlag_IsSet(0x356)) {
        FieldScene_SetPositionPairs(1);
    }
    if (GameFlag_IsSet(0x357)) {
        FieldScene_SetPositionPairs(2);
    }
}

void ActivateFiveActorGroupFromFlags(void)
{
    if (GameFlag_IsSet(0x350)) {
        PlaceSceneObjectPairFromTableA(0);
    }
    if (GameFlag_IsSet(0x351)) {
        PlaceSceneObjectPairFromTableA(1);
    }
    if (GameFlag_IsSet(0x352)) {
        PlaceSceneObjectPairFromTableA(2);
    }
    if (GameFlag_IsSet(0x353)) {
        PlaceSceneObjectPairFromTableA(3);
    }
    if (GameFlag_IsSet(0x354)) {
        PlaceSceneObjectPairFromTableA(4);
    }
}

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuTheseKidsNothingWorryAbout[];
/* FAKEMATCH: calls that cast Owner_RecalculateStats to another return type keep their original register order. */
void Owner_RecalculateStats();
/* FAKEMATCH: calls that cast Object_GetById to another return type keep their original register order. */
s32 Object_GetById();

enum {
    /* Message 0x182 + 189. */
    ITEM_WATER_OF_LIFE = 189,
    /* Message 0x182 + 231. */
    ITEM_BONE = 231
};


/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */

s32 PartyInventory_HasSpace(void);
void Battle_InitializeRenderObject(void);

void BattleEffect_CleanupSceneObjects(void);
s32 PartyInventory_HasSpace();
void Scene_JoinRodSearch();
void SceneActor_SetModeZeroAndValue();
void SceneEffect_ApplyThreeValuesAndFinish();
void SceneActor_SetPairZeroAndValue();
void Audio_PlayCueFromEventWork();

void Scheduler_AddOrUpdateCallback();
void Object_RefreshSelectorById();

void Object_RefreshSelectorById(s32);

s32 ArcTan2();
void FieldScene_RunSplitTripleSteps();

void SceneState_SetWord1c0To209AndRun();
void SceneEffect_ApplyPairWithValue141();
void SceneState_SetValue2ThenFinish();

void Ui_SetBank15PaletteAndClearRenderMode();
void Map_SetWorkFourValues();
u8 *Owner_GetState(s32);
void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
void Djinn_Transfer(s32, s32, s32, s32);

u8 *Object_CreateFar(s32);
u8 *Runtime_AllocateHeapBlock(s32, s32);
void Object_SetModeById(s32, s32, s32);
void ObjectMotion_WaitForAnimationChange(s32);
s32 KuupuappuHeya_IsActorNearPoint(s32, s32, s32);
s32 KuupuappuHeya_IsRecordNearPoint(s32, s32, s32);
void Object_SetMoveTarget(s32, s32, s32, s32);

/*
 * Update one actor's animation descriptor when its current state matches the
 * expected state. The helper is called by the 17-entry scene transition table
 * at 0x02002564.
 *
 * The owner starts with push {r5,r6,r7,lr} at 0x020026e4, returns through
 * pop {r5,r6,r7}/pop {r0}/bx r0 at 0x02002716-0x0200271a, and is immediately
 * followed by the callback owner at 0x0200271c. It has no trailing pool, so
 * the complete span is 56 bytes.
 */

/* Word at +456 of the shared scene work record. */

/* Field at +456 of the shared scene work record, addressed through the
 * loader-fixed pointer at 0x03001ebc. */

/* Pair of ratio-like arguments shared by three setup calls below (each
 * applied to a different index: 0, 1, 2). */

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

static __inline__ s32 Value0(s32 (*f)())
{
    return f();
}

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

static __inline__ void Call0(void (*f)())
{
    f();
}

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ s32 Value0_02001a4c(s32 (*f)())
{

    return f();
}

static __inline__ void Call1_02001a4c(void (*f)(), s32 a0)
{

    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{

    f(a0, a1, a2, a3, a4, a5);
}

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

static __inline__ void Call6_02001ba0(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

void SceneActor_FaceActors24And25TowardActorZero(void)
{
    struct FieldActor *origin = ((struct FieldActor *(*)())Object_GetById)(0);
    struct FieldActor *first = ((struct FieldActor *(*)())Object_GetById)(24);
    struct FieldActor *second = ((struct FieldActor *(*)())Object_GetById)(25);

    first->facing = ArcTan2(origin->z.fixed - first->z.fixed, origin->x.fixed - first->x.fixed);
    second->facing = ArcTan2(origin->z.fixed - second->z.fixed, origin->x.fixed - second->x.fixed);
}

void FieldScene_RunLateSequence(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Event_Begin();
    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1a00000);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(12, 0xcccc, 0x6666);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3100000, 0x1c00000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b00000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b00000);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 19);
    Actor_SetAnimation(ACTOR_GERALD, 19);
    Actor_SetAnimation(ACTOR_IVAN, 19);
    v5 = 2;
    *(u8 *)(Object_GetById(0) + 35) = v5;
    *(u8 *)(Object_GetById(1) + 35) = v5;
    *(u8 *)(Object_GetById(2) + 35) = v5;
    record = Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    record = Object_GetById(2);
    Actor_SetSpriteFlags(record, 0);
    record = Object_GetById(1);
    Actor_SetSpriteFlags(record, 0);
    Actor_FaceDirection(8, 0xb000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Camera_WaitForMove();
    Map_Redraw();
    SceneState_SetWord1c0To209AndRun();
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
    Event_SetMessage((s32)MsgKuupuappuTheseKidsNothingWorryAbout);
    SceneActor_SetModeZeroAndValue(10, 30);
    SceneActor_SetModeZeroAndValue(8, 30);
    Actor_WalkTo(11, 0x328, 0x1c8);
    Actor_WalkTo(12, 0x318, 0x1c8);
    Actor_WaitForMove(12);
    Actor_FaceDirection(12, 0, 0);
    Actor_WaitForMove(11);
    Actor_FaceDirection(11, 0, 0);
    Event_Wait(30);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 20);
    SceneActor_SetModeZeroAndValue(11, 20);
    FieldScene_RunSplitTripleSteps(12, 0, 30);
    SceneActor_SetModeZeroAndValue(12, 60);
    Event_End();
}
void SceneState_SetWord1c0To209AndRun(void);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

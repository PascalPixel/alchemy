#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuWant[];
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


#define RATIO_HI 52428
#define RATIO_LO 26214

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
void SceneActor_FaceActors24And25TowardActorZero(void);
extern u8 KuupuappuHeya_PairScriptB[];
extern u8 KuupuappuHeya_PairScriptO[];
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

/* Third scene step: sets up actors 24 and 25 (fetching each one's record),
 * runs a shared series of configuration calls touching actors 0-2, 10, 14,
 * 20, 24 and 25, then marks the two fetched records with a byte flag. */
void FieldScene_RunOpeningSequenceThird(void)
{

    void *actor24;
    void *actor25;

    actor24 = ((void *(*)())Object_GetById)(24);
    actor25 = ((void *(*)())Object_GetById)(25);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, RATIO_HI, RATIO_LO);
    Actor_SetSpeed(ACTOR_GERALD, RATIO_HI, RATIO_LO);
    Actor_SetSpeed(ACTOR_IVAN, RATIO_HI, RATIO_LO);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
    Event_Wait(10);
    Actor_ShowEmote(25, 256, 0);
    Actor_ShowEmote(24, 256, 0);
    Event_Wait(60);
    Call3(FieldScene_RunSplitTripleSteps, 25, 0, 10);
    Actor_RunRepeatedMotion(24, 2);
    Event_Wait(20);
    Value1(Engine_EventSetMessage, (s32)MsgKuupuappuWant);
    Call2(SceneActor_SetModeZeroAndValue, 24, 20);
    Actor_SetAttachedEffect(25, 258); /* main:0808a1f0 */
    Event_Wait(60);
    Call2(SceneActor_SetModeZeroAndValue, 25, 20);
    Actor_RunRepeatedMotion(24, 1);
    Call2(SceneActor_SetModeZeroAndValue, 24, 30);
    Actor_SetSpeed(24, 262144, 131072);
    Actor_SetSpeed(25, 229376, 114688);
    Value2(Engine_ActorEnableActionCallback, 25, (s32)KuupuappuHeya_PairScriptO);
    Value2(Engine_ActorEnableActionCallback, 24, (s32)KuupuappuHeya_PairScriptB);
    Call1(Object_RefreshSelectorById, 24);
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set(2130);
    GameFlag_Set(768);
    Call2(Scheduler_AddOrUpdateCallback, (s32)SceneActor_FaceActors24And25TowardActorZero, 3200);
    /* Starting movement steps for the two thieves. */
    ((struct FieldActor *)actor24)->unknown_64 = 1;
    ((struct FieldActor *)actor25)->unknown_64 = 3;
    Value0(Engine_EventEnd);
}

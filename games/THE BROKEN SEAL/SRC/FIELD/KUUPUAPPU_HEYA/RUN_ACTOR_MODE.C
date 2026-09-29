#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuCouldSomeonePleaseHelpIvan[];
extern u8 MsgKuupuappuDidJustArriveInTown[];
extern u8 MsgKuupuappuIfOnlyTheseRocksWere[];
extern u8 MsgKuupuappuIvanHasGreatPowersWouldnt[];
extern u8 MsgKuupuappuJustMeOrAmMissing[];
extern u8 MsgKuupuappuMisterFunSeeStrangeNew[];
extern u8 MsgKuupuappuOwStop[];
extern u8 MsgKuupuappuPleaseLookAfterIvan[];
extern u8 MsgKuupuappuThankGoodnessThoseThievesWere[];
extern u8 MsgKuupuappuThievesHidStolenTreasureIn[];
extern u8 MsgKuupuappuThoseThreeStrangersSureHave[];
extern u8 MsgKuupuappuWithBridgeOutWillQuite[];
extern u8 MsgKuupuappuYoureGoingHelpIvan[];
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
extern s32 KuupuappuHeya_StepActions[];
extern s32 KuupuappuHeya_IdleActions[];

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

void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
    Actor_SetAnimation(x, 1);
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_ShowMessage(x, 0);
}

void SceneState_RunGuardedActorStep(s32 x)
{
    u8 *flag = (u8 *)Object_GetById() + 91;
    s32 zero = 0;

    *flag = 1;
    Event_Begin();
    Actor_SetAnimation(x, 1);
    Event_Wait(2);
    Event_ShowMessage(x, 0);
    Event_End();
    *flag = zero;
}

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_OpenMessage(x, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        gEventWork->message += 1;
    }
    Event_ShowMessage(x, 0);
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuMisterFunSeeStrangeNew);
    Actor_SetAnimation(11, 1);
    SceneDialogue_PromptAndCountSkip(11);
    Event_End();
}

void FieldScene_RunScene383_02000428(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuIvanHasGreatPowersWouldnt);
    SceneDialogue_PromptAndCountSkip(15);
    Actor_FaceDirection(15, 0x8000, 0);
    Event_End();
}

void SceneState_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Event_Begin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Shop_Open(6, 21);
    } else if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuDidJustArriveInTown);
        SceneDialogue_PromptAndCountSkip(21);
    } else {
        Event_SetMessage((s32)MsgKuupuappuWithBridgeOutWillQuite);
        Event_ShowMessage(21, 0);
    }
    Event_End();
}

void SceneDialogue_RunActor9FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuIfOnlyTheseRocksWere);
    } else {
        Event_SetMessage((s32)MsgKuupuappuThankGoodnessThoseThievesWere);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(9);
    Event_End();
}

void SceneDialogue_RunActorTwelveFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) != 0) {
        Event_SetMessage((s32)MsgKuupuappuThievesHidStolenTreasureIn);
    } else {
        Event_SetMessage((s32)MsgKuupuappuJustMeOrAmMissing);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(12);
    Event_End();
}

void FieldScene_RunFlag856DialogueBranch(void)
{
    s32 g;
    g = 0x851;
    Event_Begin();
    if (GameFlag_IsSet(0x856) != 0) {
        if (GameFlag_IsSet(g) == 0) {
            Event_SetMessage((s32)MsgKuupuappuYoureGoingHelpIvan);
            ActorPresentation_RunActorModeOneThenZeroWithStep(16);
            Event_Wait(10);
            SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
            GameFlag_Set(g);
        } else {
            Event_SetMessage((s32)MsgKuupuappuPleaseLookAfterIvan);
        }
    } else {
        Event_SetMessage((s32)MsgKuupuappuCouldSomeonePleaseHelpIvan);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(16);
    Event_End();
}

void SceneDialogue_ShowLine128E(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgKuupuappuThoseThreeStrangersSureHave);
    ActorPresentation_RunActorModeOneThenZeroWithStep(18);
    Event_End();
}

void SceneActor_StepActor24AnimationByFacing(void)
{
    struct FieldActor *p;
    s16 *q;
    s32 v;
    s32 n;

    p = ((struct FieldActor *(*)())Object_GetById)(24);
    Event_Begin();
    Actor_RunRepeatedMotion(24, 2);
    Event_SetMessage((s32)MsgKuupuappuOwStop);
    Event_ShowMessage(24, 0);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    if ((u32)((p->facing & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Actor_EnableActionCallback(24, KuupuappuHeya_StepActions[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Actor_EnableActionCallback(24, KuupuappuHeya_StepActions[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Actor_EnableActionCallback(24, KuupuappuHeya_IdleActions[v]);
    n = *(u16 *)q - 1;
    *(u16 *)q = n;
clamp:
    if (*q > 5) {
        n = 0;
        *(u16 *)q = n;
    }
    if (*q < 0) {
        n = 5;
        *(u16 *)q = n;
    }
    Object_RefreshSelectorById(24);
    Event_End();
}
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuButChestWasEmpty[];
extern u8 MsgKuupuappuButDidntFindAnything[];
extern u8 MsgKuupuappuEveryoneThinksOurGuestsThieves[];
extern u8 MsgKuupuappuIfYoureGonnaHeadInto[];
extern u8 MsgKuupuappuRobinCheckedBarrel[];
extern u8 MsgKuupuappuRobinCheckedChest[];
extern u8 MsgKuupuappuTheyHidThoseStolenGoods[];
extern u8 MsgKuupuappuThievesDidntHitOurHouse[];
extern u8 MsgKuupuappuWeDontHaveTimeFor[];
extern u8 MsgKuupuappuWowHaveManyThingsArent[];
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

void FieldScene_RunActorTwentyAngleDialogue(void)
{
    s32 v = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Shop_Open(5, 20);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage((s32)MsgKuupuappuThievesDidntHitOurHouse);
        } else {
            Event_SetMessage((s32)MsgKuupuappuIfYoureGonnaHeadInto);
        }
        Event_ShowMessage(20, 0);
    }
    Event_End();
}

void FieldScene_RunActorTwentyThreeAngleDialogue(void)
{
    s32 v = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Event_Begin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Inn_Open(1, 23);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Event_SetMessage((s32)MsgKuupuappuEveryoneThinksOurGuestsThieves);
        } else {
            Event_SetMessage((s32)MsgKuupuappuTheyHidThoseStolenGoods);
        }
        Event_ShowMessage(23, 0);
    }
    Event_End();
}

void FieldScene_RunActorEighteenConditionalScene(void)
{
    Event_Begin();
    if (PartyInventory_HasSpace() == 0) {
        Actor_SetAnimationAndWait(18, 4);
        Event_Wait(20);
        Event_SetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
        Event_ShowMessage(18, 0);
    } else {
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
    }
    Event_End();
}

void SceneDialogue_ShowLine12BB(void)
{
    Battle_InitializeRenderObject();
    Event_SetMessage((s32)MsgKuupuappuWeDontHaveTimeFor);
    Event_ShowMessage(ACTOR_GERALD, 0);
}

void SceneDialogue_ShowEmptyBarrel(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgKuupuappuRobinCheckedBarrel, 1);
    Message_ShowCentered((s32)MsgKuupuappuButDidntFindAnything, 1);
    Event_End();
}

void SceneDialogue_ShowEmptyChest(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgKuupuappuRobinCheckedChest, 1);
    Message_ShowCentered((s32)MsgKuupuappuButChestWasEmpty, 1);
    Event_End();
}
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
extern u8 MsgKuupuappuCaveInGomaRangeDangerous[];
extern u8 MsgKuupuappuCouldTheyThievesGoodDont[];
extern u8 MsgKuupuappuDoPossessStrangePowers[];
extern u8 MsgKuupuappuFatherLooksSadWorryingLike[];
extern u8 MsgKuupuappuGeeAlwaysGetHungryWhen[];
extern u8 MsgKuupuappuGuessNothingInOurHouse[];
extern u8 MsgKuupuappuHaveLotLeftoverBonesFrom[];
extern u8 MsgKuupuappuHeReallyLikesBonesWonder[];
extern u8 MsgKuupuappuIfRockWorthlessMaybeThats[];
extern u8 MsgKuupuappuMasterHisWifeBlindedBy[];
extern u8 MsgKuupuappuOkMisterLetMeSee[];
extern u8 MsgKuupuappuTicklesBeingTickledByBoy[];
extern u8 MsgKuupuappuWantMoreBones[];
extern u8 MsgKuupuappuWonderOutsideWorldLike[];
extern u8 MsgKuupuappuWouldReallyWouldHelpMe[];
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

void SceneActor_SetModeZeroAndValue(s32 a, s32 b);

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    extern u8 Data_03001ebc[];

    u8 *work = *(u8 **)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

void SceneState_RunGuardedActorStep(s32 x);

void FieldScene_RunScene383_0200091c(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    SceneActor_SetPairZeroAndValue(18, 0, 2);
    if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage((s32)MsgKuupuappuHaveLotLeftoverBonesFrom);
        Event_OpenMessage(18, 0);
    } else {
        Event_SetMessage((s32)MsgKuupuappuWantMoreBones);
        Event_OpenMessage(18, 0);
    }
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Event_ShowMessage(18, 0);
        Event_Wait(20);
        Actor_RunRepeatedMotion(18, 2);
        Event_Wait(20);
        if (PartyInventory_HasSpace() == 0) {
            Actor_SetAnimationAndWait(18, 4);
            Event_Wait(20);
            Event_SetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
            Event_ShowMessage(18, 0);
            goto L_020009ec;
        }
        Item_ShowFound(ITEM_BONE, 3);
        Party_GiveItem(ITEM_BONE, 0);
        GameFlag_Set(0x85b);
    } else {
        bump_step(1);
        Event_Wait(20);
        Actor_SetAnimationAndWait(18, 3);
        Event_Wait(20);
        Event_ShowMessage(18, 0);
    }
    L_020009ec:;
    Actor_FaceDirection(18, 0x4000, 0);
    Event_End();
}

void SceneDialogue_RunActorNineFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuOkMisterLetMeSee);
    } else {
        Event_SetMessage((s32)MsgKuupuappuIfRockWorthlessMaybeThats);
    }
    SceneState_RunGuardedActorStep(9);
    Event_End();
}

void SceneDialogue_RunActorElevenFlaggedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuWonderOutsideWorldLike);
    } else {
        Event_SetMessage((s32)MsgKuupuappuFatherLooksSadWorryingLike);
    }
    SceneState_RunGuardedActorStep(11);
    Event_End();
}

void SceneDialogue_ShowLine124EOr135E(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuCouldTheyThievesGoodDont);
    } else {
        Event_SetMessage((s32)MsgKuupuappuGuessNothingInOurHouse);
    }
    SceneState_RunGuardedActorStep(12);
    Event_End();
}

void SceneDialogue_RunActor16FlaggedLine(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuTicklesBeingTickledByBoy);
    } else {
        Event_SetMessage((s32)MsgKuupuappuCaveInGomaRangeDangerous);
    }
    SceneState_RunGuardedActorStep(16);
    Event_End();
}

void SceneDialogue_RunActorEighteenBranchedDialogue(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x855) == 0) {
        Event_SetMessage((s32)MsgKuupuappuMasterHisWifeBlindedBy);
    } else if (GameFlag_IsSet(0x85b) == 0) {
        Event_SetMessage((s32)MsgKuupuappuGeeAlwaysGetHungryWhen);
    } else {
        Event_SetMessage((s32)MsgKuupuappuHeReallyLikesBonesWonder);
    }
    SceneState_RunGuardedActorStep(18);
    Event_End();
}

/* Configures actors 0, 1, 2 (position, movement, and animation timing), then
 * branches on whether actor 0 is already set up: one path sets up actors 0-2
 * with poses and movement, the other only advances actor 2's animation. Both
 * paths converge to check actor 0's record and, depending on that check,
 * configure either actor 2 or actor 1 from it before the scene finishes. */
void FieldScene_RunSetupSequence(void)
{

    u8 *record;

    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Audio_PlayCue(19);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x180, 0x198);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_SetPosition(ACTOR_GERALD, 0x1800000, 0x1980000);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x170, 0x198);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 20);
    if (GameFlag_IsSet(0x850) != 0) {
    } else {
        GameFlag_Set(0x850);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        Event_SetMessage((s32)MsgKuupuappuDoPossessStrangePowers);
        Audio_PlayCue(60);
        Event_Wait(30);
        SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
        SceneActor_SetModeZeroAndValue(2, 30);
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        SceneActor_SetModeZeroAndValue(2, 30);
        SceneActor_SetPairZeroAndValue(0, 1, 50);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Event_Wait(20);
        SceneEffect_ApplyPairWithValue141(2, 0);
        Event_Wait(40);
        SceneState_SetValue2ThenFinish();
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        SceneActor_SetModeZeroAndValue(2, 50);
        Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
        SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
        SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
        SceneActor_SetModeZeroAndValue(2, 40);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(30);
        Call3((void (*)())Engine_ActorFaceDirection, 2, 0xc000, 0);
        Event_Wait(30);
        Actor_WalkToAndWait(ACTOR_IVAN, 0x178, 0x178);
        Event_Wait(40);
        SceneActor_SetPairZeroAndValue(0, 1, 50);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
        Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
        Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
        Event_Wait(50);
        SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
        Actor_FaceDirection(ACTOR_IVAN, 0x4000, 0);
        Event_Wait(10);
        Event_ShowMessage(ACTOR_IVAN, 0);
        Event_OpenMessage(ACTOR_IVAN, 0);
        goto L_join_setup_paths;
    }
    Audio_PlayCue(60);
    Event_SetMessage((s32)MsgKuupuappuWouldReallyWouldHelpMe);
    Event_OpenMessage(ACTOR_IVAN, 0);
    L_join_setup_paths:;
    if (Event_ChooseYesNo(0, 0) == 0) {
        Scene_JoinRodSearch();
        GameFlag_Set(0x856);
        Actor_SetAnimation(ACTOR_IVAN, 2);
        record = ((void *(*)())Object_GetById)(0);
        if (record != 0) {
            /* Read the two s16 fields at +10 and +18 of the record. */
            Actor_SetDestination(ACTOR_IVAN, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(ACTOR_IVAN);
        Actor_SetPosition(ACTOR_IVAN, 0, 0);
    } else {
        Event_ShowMessage(ACTOR_IVAN, 0);
    }
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = ((u8 *(*)())Object_GetById)(0);
    if (record != 0) {
        /* Read the two s16 fields at +10 and +18 of the record. */
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Audio_PlayCueFromEventWork();
    Event_End();
}
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyPairWithValue141(s32 a, s32 b);
void SceneState_SetValue2ThenFinish(void);

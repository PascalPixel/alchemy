#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
extern u8 MsgKuupuappuSeeThatsHappened[];
extern u8 MsgKuupuappuTheyTheyGotUs[];
/* FAKEMATCH: calls that cast Owner_RecalculateStats to another return type keep their original register order. */
void Owner_RecalculateStats();

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
extern u8 KuupuappuHeya_ActionTable[];

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

/* Sets up actors 10, 11, 12 (position, pose, flags) and actors 0-2 and 8-9 and
 * 13-14 in several later waves, moving and animating them through a long
 * scripted sequence, branches once on an actor-12 state check, then advances
 * the shared scene phase twice before returning. */
void RunEventScript01(void)
{
    extern u8 *Data_03001ebc;

    u32 i;
    s32 record;
    u8 *work;
    s32 base5_200d17c;
    u8 *actor12_record;

    record = (s32)Object_GetById(12);
    actor12_record = *(u8 **)(record + 80);
    Event_Begin();
    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1880000);
    record = (s32)Object_GetById(10);
    Actor_SetSpriteFlags(record, 0);
    record = (s32)Object_GetById(11);
    Actor_SetSpriteFlags(record, 0);
    record = (s32)Object_GetById(12);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetAnimation(10, 9);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)((s32)Object_GetById(12) + 35) &= 254;
    /* Set flag bits 0x0c of the byte at +9. */
    actor12_record[9] |= 12;
    base5_200d17c = (s32)KuupuappuHeya_ActionTable;
    Actor_EnableActionCallback(10, base5_200d17c);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b00000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0xb000, 0);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_FaceDirection(8, 0xb000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Camera_WaitForMove();
    Map_Redraw();
    SceneState_SetWord1c0To209AndRun();
    Actor_EnableActionCallback(11, base5_200d17c);
    Event_Wait(30);
    Actor_EnableActionCallback(12, base5_200d17c);
    Event_Wait(30);
    Event_SetMessage((s32)MsgKuupuappuTheyTheyGotUs);
    SceneActor_SetModeZeroAndValue(10, 20);
    Actor_ShowEmote(8, 0x102, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_WalkToAndWait(8, 0x328, 0x1c8);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 0x1b0);
    Actor_FaceDirection(ACTOR_GERALD, 0, 0);
    Actor_WalkToAndWait(8, 0x328, 0x198);
    Actor_FaceDirection(8, 0x8000, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1b0);
    Actor_FaceDirection(ACTOR_GERALD, 0xc000, 0);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(8, 3, 20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_WalkToAndWait(8, 0x300, 0x198);
    Event_Wait(20);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    FieldScene_RunSplitTripleSteps(2, 8, 40);
    SceneActor_SetModeZeroAndValue(8, 30);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Actor_WalkToAndWait(8, 0x2e8, 0x198);
    Event_Wait(50);
    Actor_RunRepeatedMotion(11, 2);
    SceneActor_SetModeZeroAndValue(11, 20);
    Actor_EnableActionCallback(11, base5_200d17c);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
    Actor_FaceActor(ACTOR_GERALD, 11, 0);
    FieldScene_RunSplitTripleSteps(2, 11, 20);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_RunRepeatedMotion(12, 2);
    SceneActor_SetModeZeroAndValue(12, 30);
    Actor_EnableActionCallback(12, base5_200d17c);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(1, 30);
    FieldScene_RunSplitTripleSteps(2, 0, 30);
    FieldScene_RunSplitTripleSteps(0, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 30);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(1, 30);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneEffect_ApplyPairWithValue141(2, 0);
    Ui_SetBank15PaletteAndClearRenderMode();
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 10);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Event_Wait(60);
    FieldScene_RunSplitTripleSteps(0, 1, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        FieldScene_RunSplitTripleSteps(1, 2, 20);
        SceneState_SetValue2ThenFinish();
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        ((void (*)())Engine_EventWait)(20);
        SceneActor_SetModeZeroAndValue(1, 20);
    } else {
        bump_step(1);
        Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
        Event_Wait(60);
        SceneActor_SetModeZeroAndValue(1, 20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
        FieldScene_RunSplitTripleSteps(1, 2, 20);
        SceneState_SetValue2ThenFinish();
        Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
    }
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    Event_SetMessage((s32)MsgKuupuappuSeeThatsHappened);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 40);
    Actor_WalkToAndWait(8, 0x328, 0x198);
    Actor_FaceDirection(8, 0x8000, 0);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Actor_FaceActor(ACTOR_GERALD, 8, 0);
    FieldScene_RunSplitTripleSteps(2, 8, 20);
    Actor_RunRepeatedMotion(8, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_SetPosition(9, 0x2e80000, 0x1980000);
    Actor_WalkToAndWait(9, 0x300, 0x198);
    FieldScene_RunSplitTripleSteps(9, 10, 30);
    Actor_SetPosition(13, 0x2e80000, 0x1980000);
    Actor_WalkToAndWait(13, 0x300, 0x198);
    Actor_SetPosition(14, 0x2e80000, 0x1980000);
    Actor_WalkTo(14, 0x310, 0x190);
    Actor_WalkToAndWait(13, 0x308, 0x1a8);
    Actor_WaitForMove(14);
    Actor_FaceActor(13, 10, 0);
    FieldScene_RunSplitTripleSteps(14, 10, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneActor_SetModeZeroAndValue(11, 20);
    SceneActor_SetModeZeroAndValue(12, 30);
    SceneActor_SetPairZeroAndValue(9, 13, 20);
    Actor_RunRepeatedMotion(13, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(13, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 30);
    SceneActor_SetPairZeroAndValue(9, 14, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 30);
    FieldScene_RunSplitTripleSteps(9, 10, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_SetAnimation(13, 3);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
    SceneActor_SetPairZeroAndValue(13, 14, 20);
    Actor_SetAnimation(13, 3);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
    Actor_WalkTo(14, 0x318, 0x188);
    Actor_WalkToAndWait(13, 0x310, 0x190);
    Actor_FaceActor(13, 12, 0);
    Actor_WaitForMove(14);
    FieldScene_RunSplitTripleSteps(14, 11, 20);
    Actor_RunRepeatedMotion(13, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(13, 20);
    SceneEffect_ApplyThreeValuesAndFinish(14, 4, 20);
    SceneActor_SetModeZeroAndValue(14, 30);
    SceneActor_SetPairZeroAndValue(13, 0, 20);
    SceneActor_SetModeZeroAndValue(13, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
    /* Write the field at +0x1c8, then the phase/status word at +0x1c0, of
     * the shared scene work record. */
    work = Data_03001ebc;
    *(s32 *)(work + 0x1c8) = 30;
    *(s32 *)(work + 0x1c0) = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_Wait(60);
    Event_End();
}
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

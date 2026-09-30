#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "CALL.H"
#include "EVENT_RUNTIME.H"

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

enum {
    /* Message 0x182 + 189. */
    ITEM_WATER_OF_LIFE = 189,
    /* Message 0x182 + 231. */
    ITEM_BONE = 231
};

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
void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
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

void SceneState_RunGuardedActorStep(s32 x);
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyPairWithValue141(s32 a, s32 b);
void SceneState_SetValue2ThenFinish(void);

extern u8 MsgKuupuappuThankHelpBelieve[];
void Engine_MessageShowCentered();
void Engine_EventWait();
void Event_PrepareObjectAndApplyValue();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorWaitForMove();
void Engine_ActorSetAnimation();
void Engine_ActorJump();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorFaceActor();
void Engine_ActorFaceEachOther();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
void Engine_EventShowMessage();
void Engine_ActorShowEmote();
void Engine_ActorSetAttachedEffect();

extern u8 MsgKuupuappuVaultCutFree[];
extern u8 MsgKuupuappuVaultThievesCaught[];
void Party_SetFields1ceAnd1d0();
void Event_SetPair1d4();
void BattleFx_SetWeightedResult();

enum PromptMessage {
    MSG_ROBIN_CHECKED_CHEST = 0x929,
    MSG_ROBIN_CHECKED_BARREL = 0x92b,
    MSG_BUT_CHEST_WAS_EMPTY = 0x949,
    MSG_BUT_DIDNT_FIND_ANYTHING = 0x94b,
    MSG_IF_ONLY_THESE_ROCKS_WERE = 0x1243,
    MSG_OK_MISTER_LET_ME_SEE = 0x1245,
    MSG_MISTER_FUN_SEE_STRANGE_NEW = 0x1247,
    MSG_WONDER_OUTSIDE_WORLD_LIKE = 0x124b,
    MSG_JUST_ME_OR_AM_MISSING = 0x124c,
    MSG_COULD_THEY_THIEVES_GOOD_DONT = 0x124e,
    MSG_COULD_SOMEONE_PLEASE_HELP_IVAN = 0x1250,
    MSG_IVAN_HAS_GREAT_POWERS_WOULDNT = 0x1253,
    MSG_DO_POSSESS_STRANGE_POWERS = 0x1256,
    MSG_WOULD_REALLY_WOULD_HELP_ME = 0x125d,
    MSG_YOURE_GOING_HELP_IVAN = 0x1276,
    MSG_PLEASE_LOOK_AFTER_IVAN = 0x1278,
    MSG_TICKLES_BEING_TICKLED_BY_BOY = 0x127c,
    MSG_THIEVES_DIDNT_HIT_OUR_HOUSE = 0x1282,
    MSG_DID_JUST_ARRIVE_IN_TOWN = 0x1284,
    MSG_EVERYONE_THINKS_OUR_GUESTS_THIEVES = 0x128d,
    MSG_THOSE_THREE_STRANGERS_SURE_HAVE = 0x128e,
    MSG_MASTER_HIS_WIFE_BLINDED_BY = 0x1294,
    MSG_ROBIN_TAKE_LEAD = 0x129f,
    MSG_OW_STOP = 0x12ac,
    MSG_WE_DONT_HAVE_TIME_FOR = 0x12bb,
    MSG_THESE_KIDS_NOTHING_WORRY_ABOUT = 0x12dd,
    MSG_THEY_THEY_GOT_US = 0x12e4,
    MSG_SEE_THATS_HAPPENED = 0x12f2,
    MSG_WAIT_DONT_WANT_TAKE_YOUR = 0x132a,
    MSG_THANK_GOODNESS_THOSE_THIEVES_WERE = 0x1353,
    MSG_IF_ROCK_WORTHLESS_MAYBE_THATS = 0x1355,
    MSG_DID_THOSE_THIEVES_COME_FROM = 0x1356,
    MSG_MY_FATHER_WORRIED_ABOUT_THOSE = 0x1359,
    MSG_FATHER_LOOKS_SAD_WORRYING_LIKE = 0x135b,
    MSG_THIEVES_HID_STOLEN_TREASURE_IN = 0x135c,
    MSG_GUESS_NOTHING_IN_OUR_HOUSE = 0x135e,
    MSG_HEADING_OUT_BEYOND_GOMA_RANGE = 0x1364,
    MSG_HEARD_DEFEATED_THOSE_THIEVES = 0x1368,
    MSG_CAVE_IN_GOMA_RANGE_DANGEROUS = 0x136c,
    MSG_WE_FOUND_OUR_STOLEN_WEAPONS = 0x1370,
    MSG_IF_YOURE_GONNA_HEAD_INTO = 0x1372,
    MSG_WITH_BRIDGE_OUT_WILL_QUITE = 0x1374,
    MSG_THEY_HID_THOSE_STOLEN_GOODS = 0x137b,
    MSG_HAVE_LOT_LEFTOVER_BONES_FROM = 0x137c,
    MSG_GEE_ALWAYS_GET_HUNGRY_WHEN = 0x1382,
    MSG_WOW_HAVE_MANY_THINGS_ARENT = 0x1384,
    MSG_WANT_MORE_BONES = 0x1385,
    MSG_HE_REALLY_LIKES_BONES_WONDER = 0x1cf4
};

#define SCENE_WORD_1C8 (*(u32 *)(*(u8 **)&gEventWork + 456))

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern u8 LinkedMessage_MasterHammetIsntOnlyOne;
extern u8 LinkedMessage_YouWereSuchGreatHelp[];
extern u8 LinkedMessage_TheyreActingSuspiciousSomethingsNot[];
extern u8 LinkedMessage_TheyreBack[];
extern u8 LinkedMessage_YouRobinRightWontForget[];
extern u8 LinkedMessage_IvanGotShamansRod[];

extern u8 MsgKuupuappuUhnnUhnn[];

extern u8 MsgKuupuappuWant[];
#define RATIO_HI 52428
#define RATIO_LO 26214
void SceneActor_FaceActors24And25TowardActorZero(void);
extern u8 KuupuappuHeya_PairScriptB[];
extern u8 KuupuappuHeya_PairScriptO[];

extern u8 MsgKuupuappuRobinTakeLead[];
extern u8 MsgKuupuappuTheyreActingSuspiciousSomethingsNot[];
extern u8 KuupuappuHeya_PairScriptF[];
extern u8 KuupuappuHeya_PairScriptG[];
extern u8 KuupuappuHeya_PairScriptK[];
void Scheduler_RemoveCallback();

void KuupuappuHeya_StartActorStops(void);

extern u8 MsgKuupuappuTheyreBack[];

/* The "shown" halfword of an actor record. */
#define ACTOR_SHOWN_OFFSET 100

extern u8 MsgKuupuappuImSurrounded[];
extern u8 MsgKuupuappuNowIvan[];
extern u8 MsgKuupuappuTheresNowhereRun[];
extern struct EventRuntime *Data_03001ebc;
void KuupuappuHeya_RunScene021C8();
void KuupuappuHeya_UpdateActorStops(void);

struct SceneObjectFlags {
    u8 unknown_000[9];
    unsigned char unk_low : 2;
    unsigned char mode : 2;
    unsigned char unk_high : 4;
};

extern u8 MsgKuupuappuLearn[];
extern u8 MsgKuupuappuLeave[];
extern struct EventWork *gEventWork;
void Engine_AudioPlaySceneCue();
void Engine_ActorSetSpritePriority();
void Engine_ActorSetSpeed();
void Engine_ActorSetPosition();
void Engine_CameraFollowActor();
void Engine_MapRedraw();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
s32 Engine_UiWorkWaitThenFinalizeCapacity();
void Engine_ActorStartRepeatedMotion();

struct Flags9 {
    u8 pad[9];
    u8 low : 2;
    u8 mode : 2;
};

extern u8 KuupuappuHeya_PairScriptA[];
extern u8 KuupuappuHeya_PairScriptC[];
extern u8 KuupuappuHeya_PairScriptD[];
extern u8 KuupuappuHeya_PairScriptE[];
extern u8 KuupuappuHeya_PairScriptH[];
extern u8 KuupuappuHeya_PairScriptI[];
extern u8 KuupuappuHeya_PairScriptJ[];
extern u8 KuupuappuHeya_PairScriptL[];
extern u8 KuupuappuHeya_PairScriptM[];
extern u8 KuupuappuHeya_PairScriptN[];
extern u8 KuupuappuHeya_PairScriptP[];
extern u8 KuupuappuHeya_PairScriptQ[];
extern u8 KuupuappuHeya_PairScriptR[];

void SceneState_SetFlagByActorPosition(void);
s32 Engine_GameFlagIsSet();
void Engine_ActorFaceDirection();
void Engine_ActorEnableActionCallback();
void OverlayObject_ConfigureObject22WithResource17();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsTo();
void Engine_ActorSetSpriteFlags();
void Engine_EventBegin();
void KuupuappuHeya_RunVaultEvent();
void Engine_EventEnd();
void Engine_AudioPlayCue();
void Map_CopyMetatileCellsRect();
void FieldScene_RunLateSequence();
void Engine_EventRequestExit();
void RunEventScript01();
void RunDialoguePromptScene();

struct ActorMode {
    u8 pad[100];
    u16 mode;
};

struct ActorFlags {
    u8 pad[89];
    u8 flags;
};

extern u8 MsgKuupuappuTheseKidsNothingWorryAbout[];
void SceneState_SetWord1c0To209AndRun(void);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

extern u8 MsgKuupuappuWontGoAway[];
void SceneActor_SetModeZeroAndValue(s32 actor, s32 frames);
void SceneActor_SetPairZeroAndValue(s32 actor, s32 other, s32 frames);
void SceneEffect_ApplyThreeValuesAndFinish(s32 actor, s32 animation, s32 frames);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);
void BattleFx_SetWeightedResult(s32 first, s32 second);

extern u8 KuupuappuHeya_ActionTable[];

u8 *Owner_GetState(s32 owner);
void Djinn_Transfer(s32 owner, s32 djinn, s32 element, s32 flags);
void Owner_RecalculateStats(s32 owner);

void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc);

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
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

void Scene_JoinRodSearch(void)
{
    u32 i;
    u8 *record;
    s32 v5;
    s32 v6;
    s32 mes;

    Engine_EventWait(20);
    *(u8 *)((u8 *)Object_GetById(2) + 91) = 0;
    Engine_ActorJump(2, 4, 0);
    Engine_EventWait(40);
    mes = (s32)MsgKuupuappuThankHelpBelieve;
    Engine_EventSetMessage((s32)MsgKuupuappuThankHelpBelieve);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(50);
    SceneActor_SetModeZeroAndValue(2, 30);
    Call3(Engine_ActorWalkToAndWait, 2, 0x178, 0x188);
    SceneEffect_ApplyPairWithValue141(2, 0);
    Engine_EventWait(40);
    SceneState_SetValue2ThenFinish();
    v5 = 254;
    Engine_ActorRunRepeatedMotion(0, 1);
    *(u8 *)((u8 *)Object_GetById(0) + 90) &= v5;
    Call3(Engine_ActorWalkToAndWait, 0, 0x180, 0x1a8);
    v6 = 1;
    Engine_EventWait(1);
    *(u8 *)((u8 *)Object_GetById(0) + 90) |= v6;
    Engine_EventWait(30);
    Engine_EventShowMessage(2, 0);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Call2(Engine_ActorSetAttachedEffect, 1, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(2, 1);
    Engine_EventWait(10);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_EventWait(10);
    FieldScene_RunSplitTripleSteps(1, 0, 30);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(10);
    FieldScene_RunSplitTripleSteps(0, 1, 40);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Call3(Engine_ActorShowEmote, 2, 0x102, 0);
    Engine_EventWait(60);
    Engine_EventOpenMessage(2, 0);
    Engine_ActorFaceActor(0, 2, 0);
    Engine_ActorFaceActor(1, 2, 0);
    Engine_EventChooseYesNo(0, 0);
    Engine_EventWait(30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Call3(Engine_ActorWalkToAndWait, 2, 0x180, 0x198);
    Engine_EventWait(10);
    Engine_MessageShowCentered((mes + 5), 1);
    Engine_EventSetMessage((mes + 6));
    Engine_ActorFaceEachOther(2, 1, 0);
    FieldScene_RunSplitTripleSteps(0, 1, 20);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(1, 1);
    *(u8 *)((u8 *)Object_GetById(1) + 90) &= v5;
    Call3(Engine_ActorWalkToAndWait, 1, 0x160, 0x198);
    Engine_EventWait(1);
    {
        u8 *record = (u8 *)Object_GetById(1);
        /* FAKEMATCH: retain this byte read before the flag merge. */
        u8 value = *(volatile u8 *)&record[90];

        record[90] = (u8)(value | v6);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorWalkTo, 2, 0x170, 0x198);
    Engine_EventWait(20);
    Call3(Engine_ActorWalkToAndWait, 0, 0x170, 0x1a8);
    Engine_ActorFaceActor(0, 1, 0);
    Engine_ActorWaitForMove(2);
    SceneEffect_ApplyPairWithValue141(2, 1);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
    Engine_EventShowMessage(1, 0);
    SceneState_SetValue2ThenFinish();
    SceneActor_SetPairZeroAndValue(2, 0, 30);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(0, 1);
    Engine_EventWait(30);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Engine_EventShowMessage(2, 0);
    Engine_ActorRunRepeatedMotion(1, 2);
    Engine_ActorFaceActor(1, 0, 0);
    Engine_EventShowMessage(1, 0);
    Engine_ActorFaceActor(0, 1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 40);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Call3(Engine_ActorShowEmote, 1, 0x101, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Engine_EventShowMessage(2, 0);
    Engine_ActorShowEmote(1, 0x103, 0);
    Engine_EventWait(60);
    FieldScene_RunSplitTripleSteps(1, 0, 10);
    Engine_EventShowMessage(1, 0);
    Engine_ActorShowEmote(2, 0x100, 0);
    Engine_EventWait(60);
    Engine_EventShowMessage(2, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    Engine_MessageShowCentered((mes + 13), 1);
    Engine_EventSetMessage((mes + 14));
    Engine_ActorFaceEachOther(2, 0, 0);
    Engine_ActorFaceActor(1, 0, 0);
    Engine_ActorShowEmote(0, 0x102, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyPairWithValue141(2, 0);
    Ui_SetBank15PaletteAndClearRenderMode();
    SceneActor_SetModeZeroAndValue(1, 30);
    SceneState_SetValue2ThenFinish();
    Engine_ActorFaceEachOther(2, 1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 20);
    Engine_ActorFaceEachOther(2, 0, 0);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(20);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
    SceneActor_SetModeZeroAndValue(1, 30);
    Engine_ActorFaceActor(2, 1, 0);
    Call3(Engine_ActorShowEmote, 2, 0x101, 0);
    Engine_EventWait(30);
    SceneActor_SetPairZeroAndValue(0, 1, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Engine_ActorFaceActor(0, 2, 0);
    Engine_ActorFaceActor(1, 2, 0);
    Engine_ActorFaceActor(2, 0, 0);
    Engine_ActorShowEmote(2, 0x106, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(2, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Call3(Engine_ActorShowEmote, 0, 0x101, 0);
    Engine_ActorShowEmote(1, 0x101, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 30);
    Engine_ActorShowEmote(0, 0x105, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 10);
    SceneActor_SetModeZeroAndValue(2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Engine_ActorSetAnimation(0, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    Event_PrepareObjectAndApplyValue(2, 1);
    Engine_EventSetMessage(mes + 22);
    Engine_ActorRunRepeatedMotion(2, 1);
    Engine_EventShowMessage(2, 0);
}

/* The Vault's closing scene after the thieves are caught: the party is cut
 * free, the mayor and the villagers talk it over, one question decides who
 * speaks next, and the scene closes back into the house at entrance 17,
 * with entrance 16 kept as the one to return to. The two runs of lines
 * start at MsgKuupuappuVaultCutFree and MsgKuupuappuVaultThievesCaught. */
void FieldScene_RunVaultClosingSequence(void)
{
    s32 base;
    u8 *state;

    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 52428, 26214);
    Actor_SetSpeed(ACTOR_GERALD, 52428, 26214);
    Actor_SetSpeed(ACTOR_IVAN, 52428, 26214);
    Engine_ActorRunRepeatedMotion(0, 3);
    Engine_EventWait(20);
    base = (s32)MsgKuupuappuVaultCutFree;
    Engine_MessageShowCentered(base, 1);
    base = base + 1;
    Engine_EventSetMessage(base);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 1);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(8, 3, 40);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
    FieldScene_RunSplitTripleSteps(0, 8, 20);
    Actor_SetPosition(ACTOR_GERALD, 51904512, 28835840);
    Actor_SetPosition(ACTOR_IVAN, 51904512, 28835840);
    Actor_WalkTo(ACTOR_GERALD, 808, 432);
    Actor_WalkTo(ACTOR_IVAN, 792, 456);
    Engine_ActorWaitForMove(1);
    Engine_ActorFaceActor(1, 8, 0);
    Engine_ActorWaitForMove(2);
    FieldScene_RunSplitTripleSteps(2, 8, 60);
    Actor_ShowEmote(8, 258, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorSetAnimation(2, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_ShowEmote(ACTOR_IVAN, 258, 0);
    Actor_ShowEmote(ACTOR_GERALD, 258, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 12288, 0);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(8, 30);
    Actor_FaceDirection(8, 53248, 0);
    Engine_EventWait(30);
    Actor_ShowEmote(8, 256, 0);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 45056, 0);
    Engine_EventWait(40);
    Actor_FaceDirection(8, 53248, 0);
    Engine_EventWait(40);
    FieldScene_RunSplitTripleSteps(8, 0, 20);
    SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(2, 1);
    SceneActor_SetModeZeroAndValue(2, 40);
    Actor_SetPosition(10, 48758784, 26738688);
    Engine_AudioPlayCue(61);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorStartRepeatedMotion(0, 1);
    Engine_ActorStartRepeatedMotion(1, 1);
    Engine_ActorStartRepeatedMotion(2, 1);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(30);
    Engine_ActorFaceActor(0, 10, 0);
    Engine_ActorFaceActor(1, 10, 0);
    Engine_ActorFaceActor(2, 10, 0);
    FieldScene_RunSplitTripleSteps(8, 10, 40);
    Actor_SetSpeed(10, 52428, 26214);
    Actor_SetSpeed(11, 98304, 49152);
    Actor_SetSpeed(12, 98304, 49152);
    Actor_SetPosition(11, 48758784, 26738688);
    Actor_SetPosition(12, 48758784, 26738688);
    Actor_WalkToAndWait(10, 792, 416);
    Actor_FaceDirection(10, 12288, 0);
    Engine_EventWait(70);
    Engine_ActorFaceActor(0, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Engine_ActorFaceActor(2, 10, 0);
    Engine_ActorFaceActor(8, 10, 0);
    Actor_EnableActionCallback(11, 33608264);
    Engine_EventWait(40);
    Actor_EnableActionCallback(12, 33608364);
    Object_RefreshSelectorById(12);
    Actor_FaceDirection(11, 8192, 0);
    Actor_FaceDirection(12, 8192, 0);
    Event_Wait(40);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(11, 20);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(12, 40);
    Actor_ShowEmote(ACTOR_IVAN, 257, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    SceneActor_SetModeZeroAndValue(10, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
    Actor_ShowEmote(ACTOR_GERALD, 257, 0);
    Event_Wait(60);
    Actor_ShowEmote(8, 256, 0);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    SceneActor_SetModeZeroAndValue(8, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Engine_ActorFaceActor(1, 8, 0);
    FieldScene_RunSplitTripleSteps(2, 8, 20);
    SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(2, 2);
    Engine_EventWait(20);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_ShowEmote(8, 258, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(8, 20);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(10);
    SceneActor_SetModeZeroAndValue(10, 20);
    Engine_ActorFaceActor(0, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    FieldScene_RunSplitTripleSteps(2, 10, 20);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 20);
    SceneActor_SetModeZeroAndValue(11, 20);
    SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
    SceneActor_SetModeZeroAndValue(12, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
    Actor_ShowEmote(ACTOR_GERALD, 258, 0);
    Actor_ShowEmote(ACTOR_IVAN, 258, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
    Engine_EventOpenMessage(10, 0);
    Engine_EventWait(50);
    Engine_ActorFaceActor(2, 0, 0);
    FieldScene_RunSplitTripleSteps(1, 0, 30);
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
        Engine_EventWait(40);
        Engine_ActorFaceActor(1, 10, 0);
        Actor_FaceActor(ACTOR_IVAN, 10, 0);
        Actor_RunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(10, 0);
    } else {
        Event_Wait(40);
        Engine_ActorFaceActor(1, 10, 0);
        Engine_ActorFaceActor(2, 10, 0);
        base = (s32)MsgKuupuappuVaultThievesCaught;
        Engine_EventSetMessage(base);
        Engine_ActorRunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Engine_EventShowMessage(10, 0);
        base = base - 3;
        Engine_EventSetMessage(base);
    }
    Actor_ShowEmote(11, 259, 0);
    Engine_EventWait(60);
    SceneActor_SetModeZeroAndValue(11, 20);
    SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
    SceneActor_SetModeZeroAndValue(12, 30);
    SceneActor_SetPairZeroAndValue(10, 11, 30);
    Engine_ActorSetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
    SceneActor_SetPairZeroAndValue(10, 12, 30);
    Engine_ActorSetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
    Actor_FaceDirection(10, 12288, 0);
    Actor_FaceDirection(11, 12288, 0);
    Actor_FaceDirection(12, 12288, 0);
    Engine_EventWait(20);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    Engine_EventShowMessage(10, 0);
    GameFlag_Set(2132);
    gEventWork->start_transition = 0x200;
    base = (s32)&SceneId_KuupuappuHeya;
    Party_SetFields1ceAnd1d0(base, 17);
    Event_SetPair1d4(base, 16);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while puts the state's address in a block of its
       own, so it is loaded ahead of the byte's offset. */
    do {
        state[0x22b] = 3;
    } while (0);
    BattleFx_SetWeightedResult(12, 5);
    Engine_EventEnd();
}

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
void SceneState_SetValue123Mode11(void)
{
    Audio_PlayCue(123);
    Event_RequestExit(11);
}

/* Sets up the opening sequence: two calls with fixed argument pairs, a
 * write to the scene work record, and two more calls with fixed args. */
void FieldScene_RunOpeningSequenceHead(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384); /* object_id 0, speed_limit 32768, acceleration 16384 */
    Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408); /* object_id 0, x 728, z 408 */
    SCENE_WORD_1C8 = 16;
    Audio_PlayCue(123);
    Event_RequestExit(15); /* main:0808a248 */
}

/* The leader walks to the door; the first time, actor 8 groans before the
 * room is left through exit 14. */
void FieldScene_RunOpeningSequenceSecond(void)
{
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
    if (GameFlag_IsSet(0x854) == 0) {
        Event_Begin();
        ((s32 (*)())Engine_EventSetMessage)((s32)MsgKuupuappuUhnnUhnn);
        Event_ShowMessage(8, 0);
        Event_End();
    }
    *(u32 *)((u8 *)gEventWork + 456) = 16;
    ((s32 (*)())Engine_AudioPlayCue)(123);
    Event_RequestExit(14);
}

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */

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
    FieldScene_RunSplitTripleSteps(25, 0, 10);
    Actor_RunRepeatedMotion(24, 2);
    Event_Wait(20);
    Engine_EventSetMessage((s32)MsgKuupuappuWant);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_SetAttachedEffect(25, 258); /* main:0808a1f0 */
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(25, 20);
    Actor_RunRepeatedMotion(24, 1);
    SceneActor_SetModeZeroAndValue(24, 30);
    Actor_SetSpeed(24, 262144, 131072);
    Actor_SetSpeed(25, 229376, 114688);
    Engine_ActorEnableActionCallback(25, (s32)KuupuappuHeya_PairScriptO);
    Engine_ActorEnableActionCallback(24, (s32)KuupuappuHeya_PairScriptB);
    Object_RefreshSelectorById(24);
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44); /* main:080091c0 */
    GameFlag_Set(2130);
    GameFlag_Set(768);
    Scheduler_AddOrUpdateCallback((s32)SceneActor_FaceActors24And25TowardActorZero, 3200);
    /* Starting movement steps for the two thieves. */
    ((struct FieldActor *)actor24)->unknown_64 = 1;
    ((struct FieldActor *)actor25)->unknown_64 = 3;
    Value0(Engine_EventEnd);
}

/* Ivan finds actors 24 and 25 suspicious and the party agrees to follow
 * them; the pair then take their places by the far wall. */
void FieldScene_RunScene383SequenceB(void)
{
    s32 actor24;
    s32 actor25;
    s32 base;

    actor24 = ((s32 (*)())Object_GetById)(24);
    actor25 = ((s32 (*)())Object_GetById)(25);
    Event_Begin();
    Scheduler_RemoveCallback((s32)SceneActor_FaceActors24And25TowardActorZero);
    GameFlag_Clear(0x300);
    if (*(s16 *)(actor24 + 100) <= 3) {
        Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptG);
    } else {
        Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptF);
    }
    if (*(s16 *)(actor25 + 100) <= 2) {
        Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptK);
    } else {
        Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptF);
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 0x2d8);
    Actor_SetPosition(ACTOR_IVAN, 0xf80000, 0x2d80000);
    Actor_SetPosition(ACTOR_GERALD, 0xf80000, 0x2d80000);
    Actor_WalkTo(ACTOR_IVAN, 0x108, 0x2e8);
    Actor_WalkToAndWait(ACTOR_GERALD, 232, 0x2e8);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_PARTY_LEADER, 0);
    FieldScene_RunSplitTripleSteps(2, 0, 30);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    base = (s32)MsgKuupuappuTheyreActingSuspiciousSomethingsNot;
    Event_SetMessage(base);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 10);
    Event_OpenMessage(ACTOR_GERALD, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    SceneActor_SetModeZeroAndValue(1, 30);
    Event_SetMessage((base + 4));
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 50);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 30);
    Event_SetMessage((s32)MsgKuupuappuRobinTakeLead);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Event_Wait(40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
    Actor_WalkTo(ACTOR_IVAN, 248, 0x2d8);
    Actor_WalkToAndWait(ACTOR_GERALD, 248, 0x2d8);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    Actor_SetPosition(24, 0x680000, 0x2b80000);
    Actor_SetPosition(25, 0x780000, 0x2b80000);
    Actor_FaceDirection(24, 0, 0);
    Actor_FaceDirection(25, 0x8000, 0);
    Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
    Event_End();
}

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
void FieldScene_RunSteps107And250(void)
{
    GameFlag_Set(0x107);
    GameFlag_Set(0x250);
    KuupuappuHeya_StartActorStops();
}

/* Actors 24 and 25 come back: one line, then each takes up its script and
 * its shown state. */
void FieldScene_ConfigurePairedActors(void)
{
    Event_Wait(30);
    Actor_RunRepeatedMotion(24, 1);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKuupuappuTheyreBack);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_FaceDirection(25, 0, 20);
    Actor_SetAttachedEffect(25, 0x102);
    Actor_RunRepeatedMotion(25, 2);
    SceneActor_SetModeZeroAndValue(25, 20);
    Actor_SetAnimationAndWait(24, 4);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    Actor_SetSpeed(25, 0x38000, 0x1c000);
    ((s32 (*)())Engine_ActorEnableActionCallback)(25, (s32)KuupuappuHeya_PairScriptO);
    Actor_EnableActionCallback(24, KuupuappuHeya_PairScriptB);
    Object_RefreshSelectorById(24);
    /* FAKEMATCH: each shown state is parked in a word-sized local before
     * its halfword store, which keeps the constants' loads where the
     * reference has them. */
    {
        u8 *record = (u8 *)Object_GetById(24);
        s32 shown = 1;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    {
        u8 *record = (u8 *)((s32 (*)())Object_GetById)(25);
        s32 shown = 3;

        *(u16 *)(record + ACTOR_SHOWN_OFFSET) = shown;
    }
    Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

void FieldScene_SelectActorPair(void)
{
    u8 *record;
    s32 actor;
    u8 *work;

    work = (u8 *)Data_03001ebc;
    Event_Begin();
    Scheduler_RemoveCallback((s32)KuupuappuHeya_UpdateActorStops);
    GameFlag_Clear(0x107);
    GameFlag_Clear(0x250);
    Actor_SetAnimation(24, 1);
    Actor_SetAnimation(25, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(24, ACTOR_IVAN, 0);
    Actor_FaceActor(25, ACTOR_IVAN, 0);
    Event_Wait(10);
    actor = 24;
    switch (*(s16 *)(((s32)work + 0x182))) {
    case 202:
    case 203:
        Event_SetMessage((s32)MsgKuupuappuImSurrounded);
        Actor_SetAttachedEffect(25, 0x102);
        Actor_RunRepeatedMotion(25, 2);
        SceneActor_SetModeZeroAndValue(25, 20);
        if (*(s16 *)(((s32)work + 0x182)) == 202) {
            actor = 25;
            break;
        }
        /* fall through */
    case 201:
        Event_SetMessage((s32)MsgKuupuappuTheresNowhereRun);
        Actor_SetAttachedEffect(24, 0x102);
        Actor_RunRepeatedMotion(24, 2);
        actor = 24;
        SceneActor_SetModeZeroAndValue(24, 20);
        break;
    }
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Actor_FaceActor(ACTOR_IVAN, actor, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    Event_Wait(20);
    Event_SetMessage((s32)MsgKuupuappuNowIvan);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneEffect_ApplyPairWithValue141(2, actor);
    Ui_SetBank15PaletteAndClearRenderMode();
    Event_Wait(60);
    Actor_RunRepeatedMotion(24, 2);
    SceneActor_SetModeZeroAndValue(24, 20);
    Actor_RunRepeatedMotion(25, 2);
    SceneActor_SetModeZeroAndValue(25, 20);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 60);
    SceneState_SetValue2ThenFinish();
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    Actor_FaceActor(ACTOR_IVAN, ACTOR_GERALD, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(ACTOR_IVAN, 3);
    Event_Wait(40);
    record = (u8 *)Object_GetById(0);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = (u8 *)Object_GetById(1);
    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    record = (u8 *)Object_GetById(2);

    (*(struct SceneObjectFlags **)(record + 80))->mode = 1;
    Data_03001ebc->value_1c8 = 24;
    Data_03001ebc->value_1c0 = 0x201;
    Event_CloseScreen();
    Event_WaitForScreen();
    KuupuappuHeya_RunScene021C8();
    Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
    GameFlag_Set(0x853);
    {
        u8 *record = (u8 *)Object_GetById(24);
        s32 shown = 5;

        *(u16 *)((s32)record + 100) = shown;
    }
    {
        u8 *record = (u8 *)Object_GetById(25);
        s32 shown = 4;

        *(u16 *)((s32)record + 100) = shown;
    }
    Scheduler_AddOrUpdateCallback((s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
    Data_03001ebc->value_1c0 = 0x209;
    Event_End();
}

void KuupuappuHeya_RunScene021C8(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    Engine_AudioPlaySceneCue();
    Engine_ActorSetSpritePriority(0, 1);
    {
        u8 *record = (u8 *)Object_GetById(0);
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | 1);
    }
    Map_SetWorkFourValues(0x200000, 0x2400000, 0x1900000, 0x3a80000);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 1, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 2, 0xcccc, 0x6666);
    Call3(Engine_ActorSetPosition, 0, 0xf80000, 0x2d80000);
    Call3(Engine_ActorSetPosition, 2, 0x1080000, 0x2e80000);
    Engine_ActorSetPosition(1, 0xe80000, 0x2e80000);
    record = (u8 *)Object_GetById(0);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    record = (u8 *)Object_GetById(1);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    record = (u8 *)Object_GetById(2);
    ((struct Flags9 *)(*(u8 **)(record + 80)))->mode = 1;
    Engine_ActorFaceEachOther(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    Call3(Engine_ActorSetPosition, 24, 0x680000, 0x2b80000);
    Call3(Engine_ActorSetPosition, 25, 0x780000, 0x2b80000);
    Engine_ActorFaceEachOther(24, 25, 0);
    Engine_CameraFollowActor(0, 0);
    ((void (*)())Engine_CameraWaitForMove)();
    Engine_MapRedraw();
    Engine_EventWait(30);
    {
        u8 *work = *(u8 **)&gEventWork;

        *(s32 *)(work + 0x1c8) = 24;
        *(s32 *)(work + 0x1c0) = 0x201;
    }
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(10);
    Engine_EventSetMessage((s32)MsgKuupuappuLearn);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Engine_ActorFaceActor(0, 2, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    Engine_EventShowMessage(2, 0);
    Call3(Engine_ActorShowEmote, 0, 0x102, 0);
    Call3(Engine_ActorShowEmote, 1, 0x102, 0);
    Engine_EventWait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 30);
    Engine_ActorRunRepeatedMotion(1, 1);
    Engine_EventWait(20);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Engine_EventOpenMessage(1, 0);
    v5 = 0;
    if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
        Engine_EventWait(20);
        Engine_ActorStartRepeatedMotion(2, 2);
        SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
        Engine_EventOpenMessage(1, 0);
        v5 = 0;
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) != 0) {
            Engine_EventWait(20);
            Engine_ActorShowEmote(2, 0x102, 0);
            Engine_EventWait(60);
            Engine_ActorRunRepeatedMotion(2, 2);
            SceneActor_SetPairZeroAndValue(0, 2, 20);
            Engine_EventOpenMessage(2, 0);
            v5 = 0;
            if (Engine_UiWorkWaitThenFinalizeCapacity(2, 0) != 0) {
                Engine_EventWait(20);
                Engine_ActorShowEmote(2, 0x105, 0);
                Engine_EventWait(60);
                SceneActor_SetModeZeroAndValue(2, 20);
                FieldScene_RunSplitTripleSteps(1, 2, 10);
                Engine_ActorRunRepeatedMotion(1, 1);
                Engine_EventWait(10);
                SceneActor_SetModeZeroAndValue(1, 10);
                FieldScene_RunSplitTripleSteps(2, 1, 20);
                Engine_ActorShowEmote(2, 0x101, 0);
                Engine_EventWait(60);
                SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
                SceneActor_SetModeZeroAndValue(1, 10);
                SceneActor_SetPairZeroAndValue(1, 0, 20);
                SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
                v5 = 1;
                SceneActor_SetModeZeroAndValue(1, 20);
            }
        }
    }
    if (v5 == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuLeave);
        SceneActor_SetModeZeroAndValue(1, 20);
        SceneActor_SetPairZeroAndValue(1, 0, 20);
        SceneActor_SetModeZeroAndValue(1, 20);
    }
    Engine_ActorShowEmote(0, 0x105, 0);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(1, 1);
    SceneActor_SetModeZeroAndValue(1, 10);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 10);
    Engine_ActorFaceEachOther(1, 2, 0);
    SceneActor_SetPairZeroAndValue(0, 2, 10);
    Engine_EventShowMessage(1, 0);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
    Call3(Engine_ActorWalkTo, 2, 248, 0x2d8);
    Call3(Engine_ActorWalkToAndWait, 1, 248, 0x2d8);
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
}

/* Pose actors 24 and 25 for dialogue steps 11 to 18. */
void KuupuappuHeya_PoseDialogueActors(void)
{
    switch (*(s16 *)(*(u8 **)&gEventWork + 0x16c)) {
    case 11:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 2, (s32)KuupuappuHeya_PairScriptC);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 4, (s32)KuupuappuHeya_PairScriptR);
        break;
    case 12:
        SceneActor_UpdateAnimationOnStateMatch(24, 1, 4, (s32)KuupuappuHeya_PairScriptG);
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 3, (s32)KuupuappuHeya_PairScriptD);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 3, (s32)KuupuappuHeya_PairScriptO);
        break;
    case 13:
        SceneActor_UpdateAnimationOnStateMatch(24, 2, 1, (s32)KuupuappuHeya_PairScriptA);
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 6, (s32)KuupuappuHeya_PairScriptJ);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 4, (s32)KuupuappuHeya_PairScriptQ);
        break;
    case 14:
        SceneActor_UpdateAnimationOnStateMatch(24, 3, 2, (s32)KuupuappuHeya_PairScriptC);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 3, (s32)KuupuappuHeya_PairScriptP);
        break;
    case 15:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 5, (s32)KuupuappuHeya_PairScriptH);
        SceneActor_UpdateAnimationOnStateMatch(25, 1, 2, (s32)KuupuappuHeya_PairScriptM);
        break;
    case 16:
        SceneActor_UpdateAnimationOnStateMatch(24, 4, 1, (s32)KuupuappuHeya_PairScriptB);
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 6, (s32)KuupuappuHeya_PairScriptI);
        SceneActor_UpdateAnimationOnStateMatch(25, 3, 1, (s32)KuupuappuHeya_PairScriptL);
        break;
    case 17:
        SceneActor_UpdateAnimationOnStateMatch(24, 5, 4, (s32)KuupuappuHeya_PairScriptF);
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 3, (s32)KuupuappuHeya_PairScriptE);
        SceneActor_UpdateAnimationOnStateMatch(25, 4, 2, (s32)KuupuappuHeya_PairScriptN);
        break;
    case 18:
        SceneActor_UpdateAnimationOnStateMatch(24, 6, 5, (s32)KuupuappuHeya_PairScriptH);
        SceneActor_UpdateAnimationOnStateMatch(25, 2, 1, (s32)KuupuappuHeya_PairScriptK);
        break;
    }
}

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
    u8 *rec = ((u8 *(*)())Object_GetById)(actor);

    if (*(s16 *)(rec + 100) == expected) {
        Actor_EnableActionCallback(actor, desc);
        *(u16 *)(rec + 100) = (u16)next;
    }
}

void SceneState_SetFlagByActorPosition(void)
{
    u8 *p0 = ((u8 *(*)())Object_GetById)(0);
    s32 rx = *(s32 *)(p0 + 8);
    s32 x;
    s32 z;

    u8 *p1 = ((u8 *(*)())Object_GetById)(0);
    x = rx >> 20;
    z = *(s32 *)(p1 + 16);
    x = x - 34;
    z = z >> 20;

    if ((u32)x <= 1 && z > 40 && z <= 42) {
        GameFlag_Set(148 << 2);
    } else {
        GameFlag_Clear(148 << 2);
    }
}

/* Place the room's actors and map cells for the current story state. */
s32 KuupuappuHeya_ApplyEntryState(void)
{
    *(s32 *)((*(s32 *)&gEventWork + 0x1c0)) = 0x209;
    switch (gGameState.entrance) {
    case 5:
        {
            s32 zero = 0;

            ((u8 *)Object_GetById(8))[85] = zero;
            *(s32 *)(((u8 *)Object_GetById(8)) + 12) = zero;
            *(s32 *)(((u8 *)Object_GetById(8)) + 20) = zero;
        }
        break;
    case 10:
        if (Engine_GameFlagIsSet(0x850) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 2, 0x1780000, 0x1780000);
            Call3((void (*)())Engine_ActorFaceDirection, 2, 0x4000, 0);
        }
        if (Engine_GameFlagIsSet(0x856) != 0) {
            ((void (*)())Engine_ActorSetPosition)(2, 0, 0);
        }
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 16, 0x2180000, 0x1d00000);
            ((void (*)())Engine_ActorEnableActionCallback)(16, 1);
            Call3((void (*)())Engine_ActorFaceDirection, 16, 0x5000, 0);
        }
        break;
    case 7:
    case 11:
        if (Engine_GameFlagIsSet(0x855) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 18, 0x2380000, 0x2880000);
            ((void (*)())Engine_ActorEnableActionCallback)(18, 1);
            Call3((void (*)())Engine_ActorFaceDirection, 18, 0x4000, 0);
            OverlayObject_ConfigureObject22WithResource17(231, 0x2380000, 0x100000, 0x2a00000);
            ((void (*)())Engine_TaskAddCallback)((s32)SceneState_SetFlagByActorPosition, 0xc80);
        } else if (Engine_GameFlagIsSet(0x853) != 0) {
            ((void (*)())Engine_ActorSetPosition)(18, 0, 0);
        }
        break;
    case 12:
        if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x852) != 0
            && Engine_GameFlagIsSet(0x853) == 0 && Engine_GameFlagIsSet(0x300) != 0) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 14, 45, 3, 1, 14, 44);
            ((void (*)())Engine_TaskAddCallback)((s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
            break;
        }
        if (Engine_GameFlagIsSet(0x856) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 25, 0x780000, 0x2b80000);
            Call3((void (*)())Engine_ActorFaceDirection, 25, 0x8000, 0);
        }
        if (Engine_GameFlagIsSet(0x852) != 0) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 14, 45, 3, 1, 14, 44);
            if (Engine_GameFlagIsSet(0x853) == 0) {
                ((void (*)())Engine_MapCopyCellAttributes)(14, 50, 3, 1, 14, 44);
                break;
            }
            ((struct ActorMode *)((u8 *)Object_GetById(24)))->mode = 5;
            ((struct ActorMode *)((u8 *)Object_GetById(25)))->mode = 4;
            ((void (*)())Engine_TaskAddCallback)((s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
        }
        break;
    case 13:
    case 14:
        Engine_EventWait(2);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 35, 20, 2, 10);
        ((void (*)())Engine_MapCopyCellsTo)(54, 2, 95, 20, 2, 10);
        ((void (*)())Engine_MapCopyCellsTo)(54, 2, 35, 80, 2, 10);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 46, 21, 4, 8);
        ((void (*)())Engine_MapCopyCellsTo)(54, 2, 46, 81, 4, 8);
        ((struct ActorFlags *)((u8 *)Object_GetById(26)))->flags |= 4;
        Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
        if (Engine_GameFlagIsSet(0x859) != 0) {
            Call3((void (*)())Engine_ActorSetPosition, 26, 0x2a40000, 0x19b0000);
            Call6((void (*)())Engine_MapCopyCellAttributes, 101, 24, 3, 4, 41, 24);
        }
        break;
    case 15:
        Engine_EventWait(2);
        Call6((void (*)())Engine_MapCopyCellsTo, 54, 2, 44, 21, 2, 8);
        ((void (*)())Engine_MapCopyCellsTo)(54, 2, 44, 81, 2, 8);
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x855) != 0) {
            ((void (*)())Engine_ActorSetAnimation)(15, 2);
            ((void (*)())Engine_ActorSetAnimation)(16, 2);
            Engine_EventWait(1);
            ((void (*)())Engine_ActorSetAnimation)(17, 2);
            break;
        }
        ((void (*)())Engine_ActorSetPosition)(8, 0x3380000, 0x1c80000);
        if (Engine_GameFlagIsSet(0x854) != 0) {
            KuupuappuHeya_RunVaultEvent();
            Engine_EventEnd();
            break;
        }
        ((void (*)())Engine_ActorSetAnimation)(8, 7);
        SceneState_SetWord1c0To209AndRun();
        Engine_AudioPlayCue(17);
        Engine_EventSetMessage((s32)MsgKuupuappuUhnnUhnn);
        ((void (*)())SceneActor_SetModeZeroAndValue)(8, 10);
        Engine_EventEnd();
        break;
    case 16:
        ((void (*)())Map_CopyMetatileCellsRect)(54, 2, 2, 8, 44, 21);
        ((void (*)())Map_CopyMetatileCellsRect)(54, 2, 2, 8, 44, 81);
        ((void (*)())Engine_ActorSetPosition)(8, 0x3380000, 0x1c80000);
        FieldScene_RunLateSequence();
        Engine_EventRequestExit(16);
        break;
    case 17:
        ((void (*)())Map_CopyMetatileCellsRect)(54, 2, 2, 8, 44, 21);
        ((void (*)())Map_CopyMetatileCellsRect)(54, 2, 2, 8, 44, 81);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Call3((void (*)())Engine_ActorSetPosition, 8, 0x3380000, 0x1c80000);
            RunEventScript01();
            RunDialoguePromptScene();
            break;
        }
        ((void (*)())Engine_ActorSetAnimation)(15, 2);
        ((void (*)())Engine_ActorSetAnimation)(16, 2);
        Engine_EventWait(1);
        ((void (*)())Engine_ActorSetAnimation)(17, 2);
        break;
    }
    return 0;
}

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
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
    *(u8 *)((s32)Object_GetById(0) + 35) = v5;
    *(u8 *)((s32)Object_GetById(1) + 35) = v5;
    *(u8 *)((s32)Object_GetById(2) + 35) = v5;
    record = (s32)Object_GetById(0);
    Actor_SetSpriteFlags(record, 0);
    record = (s32)Object_GetById(2);
    Actor_SetSpriteFlags(record, 0);
    record = (s32)Object_GetById(1);
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

/* The Vault house scene in which the three villagers will not go away:
   they and the party are placed and turned, the screen opens, the three
   argue in turn, and the scene closes back into the same house, entrance
   17, with entrance 16 kept as the one to return to. */
void KuupuappuHeya_RunVaultEvent(void)
{
    u8 *state;

    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1a00000);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(1, 0x3280000, 0x1b00000);
    Actor_SetPosition(2, 0x3180000, 0x1c80000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(1, 0xb000, 0);
    Actor_FaceDirection(2, 0xb000, 0);
    Actor_FaceActor(8, 10, 0);
    gEventWork->start_transition = 0x209;
    Camera_FollowActor(0, 0);
    Camera_WaitForMove();
    Map_Redraw();
    Task_Wait(1);
    gEventWork->transition_frames = 32;
    SceneState_SetWord1c0To209AndRun();
    Event_Wait(60);
    Event_SetMessage((s32)MsgKuupuappuWontGoAway);
    Actor_RunRepeatedMotion(11, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(11, 30);
    Actor_RunRepeatedMotion(12, 1);
    Event_Wait(20);
    Event_ShowMessage(12, 0);
    SceneActor_SetPairZeroAndValue(10, 11, 30);
    Actor_SetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
    SceneActor_SetPairZeroAndValue(10, 12, 30);
    Actor_SetAnimation(10, 3);
    SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
    Actor_FaceDirection(10, 0x3000, 0);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x3000, 0);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
    Event_ShowMessage(10, 0);
    gEventWork->start_transition = 0x200;
    Party_SetFields1ceAnd1d0((s32)&SceneId_KuupuappuHeya, 17);
    Event_SetPair1d4((s32)&SceneId_KuupuappuHeya, 16);
    state = (u8 *)&gGameState;
    /* FAKEMATCH: the do-while puts the state's address in a block of its
       own, so it is loaded ahead of the byte's offset. */
    do {
        state[0x22b] = 3;
        BattleFx_SetWeightedResult(12, 5);
    } while (0);
}

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

/* The "shown" half word at +100 of an actor record. */

/* Phase/status word at 0x1c0 of the shared scene work record. */
s32 OverlayObject_GetObjectTwoByte118(void)
{
    return Owner_GetState(2)[0x118];
}

/* After twenty placement updates, if bit 0 of party member 2's word at
   +0xf8 is set, his djinni are transferred, cue 126 plays and members 0
   and 2 have their stats recalculated. It is declared to return a value,
   and returns none. */
s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
    u8 *state;

    BattlePlacement_UpdateTimedEntriesTwentyTimes();
    state = Owner_GetState(2);
    if (*(s32 *)(state + 0xf8) & 1) {
        Djinn_Transfer(2, 0, 0, 0);
        Audio_PlayCue(126);
        Owner_RecalculateStats(0);
        Owner_RecalculateStats(2);
    }
}

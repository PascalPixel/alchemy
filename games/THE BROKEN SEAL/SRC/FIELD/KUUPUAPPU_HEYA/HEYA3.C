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

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

s32 BattleFx_PlayCueAndStartEmitterOnTarget();
void Party_RemoveOwnerRestored();
extern u8 MsgKuupuappuIvanGotShamansRod[];
extern u8 MsgKuupuappuWaitDontWantTakeYour[];
extern u8 MsgKuupuappuYouRobinRightWontForget[];
extern u8 KuupuappuHeya_VaultScriptA[];
extern u8 KuupuappuHeya_VaultScriptB[];
extern u8 KuupuappuHeya_VaultScriptC[];
extern u8 KuupuappuHeya_VaultScriptD[];
extern u8 KuupuappuHeya_VaultScriptE[];

static __inline__ void bump_halfword(s32 off, s32 amount)
{
    u8 *work = (u8 *)gEventWork;
    u16 *slot = (u16 *)(work + off);
    /* FAKEMATCH: the sum goes through a word-sized temporary before the
     * halfword store, as the reference computes it. */
    s32 next = *slot + amount;

    *slot = next;
}

s32 OverlayObject_GetObjectTwoByte118(void);
s32 OverlayObject_RunObjectTwoWhenFlagged(void);

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

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern u8 LinkedMessage_MasterHammetIsntOnlyOne;
extern u8 KuupuappuHeya_Stops[];
extern u8 LinkedMessage_YouWereSuchGreatHelp[];
extern u8 LinkedMessage_TheyreActingSuspiciousSomethingsNot[];
extern u8 LinkedMessage_TheyreBack[];
extern u8 LinkedMessage_YouRobinRightWontForget[];
extern u8 LinkedMessage_IvanGotShamansRod[];

struct Stop {
    u8 id;
    u8 pos;
    u8 unknown_02[2];
};

struct StopSet {
    u8 unknown_00[4];
    struct Stop stops[3];
};

struct StopRecord {
    u8 unknown_00[16];
};

/*
 * resource_383 owner at 0x020047bc, 64 bytes.
 * Tests whether a record is within eight units of the given point. Each axis
 * falls back to the record's own position when its target field is the
 * unset marker 0x80000000; the comparison is on the squared distance.
 */
struct Rec_383b {
    u8 pad00[8];
    s32 f8;                     /* +8  */
    u8 pad0c[4];
    s32 f16;                    /* +16 */
    u8 pad14[36];
    s32 f56;                    /* +56 */
    u8 pad3c[4];
    s32 f64;                    /* +64 */
};

struct Rec_383b *ObjectTable_Get();

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

/* The room's closing scene: Ivan is given the Shaman's Rod and the party
 * takes its leave. */
void RunDialoguePromptScene(void)
{
    s32 off1d8;
    u8 *rec8;
    u8 *record;
    u8 *work;
    s32 aftermath;
    s32 rod;
    u8 *p7;

    p7 = (u8 *)gEventWork;
    GameFlag_Set(0x855);
    Event_Begin();
    {
        u8 *record = (u8 *)Object_GetById(12);
        /* FAKEMATCH: a result temporary, not a compound or-assign: the
         * reference merges the byte into the mask's register, which the
         * two-address ORR does only when the result is its own object. */
        u8 merged = (u8)(record[35] | 1);

        record[35] = merged;
    }
    Actor_MoveToAndWait(15, 0x368, 0x1a9);
    Actor_MoveToAndWait(16, 0x368, 0x199);
    Actor_MoveToAndWait(17, 0x368, 0x179);
    Actor_SetPosition(11, 0x3080000, 0x1880000);
    Actor_SetPosition(10, 0x3180000, 0x1880000);
    Actor_SetPosition(12, 0x3280000, 0x1880000);
    Actor_SetAnimation(10, 5);
    Actor_SetAnimation(11, 5);
    Actor_SetAnimation(12, 5);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    record = (u8 *)Object_GetById(10);
    Actor_SetSpriteFlags((struct FieldActor *)record, 1);
    record = (u8 *)Object_GetById(11);
    Actor_SetSpriteFlags((struct FieldActor *)record, 1);
    record = (u8 *)Object_GetById(12);
    Actor_SetSpriteFlags((struct FieldActor *)record, 1);
    Actor_SetPosition(13, 0x3000000, 0x1980000);
    Actor_SetPosition(14, 0x3000000, 0x1a80000);
    Actor_MoveToAndWait(9, 0x310, 0x1a8);
    Actor_SetPosition(8, 0x3280000, 0x1980000);
    Actor_FaceActor(13, 9, 0);
    Actor_FaceActor(8, 9, 0);
    Actor_FaceActor(14, 10, 0);
    Actor_FaceActor(9, 10, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
    Actor_SetPosition(ACTOR_GERALD, 0x3280000, 0x1b80000);
    Actor_SetPosition(ACTOR_IVAN, 0x3080000, 0x1b80000);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
    Actor_FaceActor(ACTOR_GERALD, 10, 0);
    Actor_FaceActor(ACTOR_IVAN, 10, 0);
    work = (u8 *)gEventWork;
    *(s32 *)(work + 0x1c8) = 30;
    *(s32 *)(work + 0x1c0) = 0x201;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(40);
    Actor_RunRepeatedMotion(10, 2);
    aftermath = (s32)MsgKuupuappuYouRobinRightWontForget;
    Event_SetMessage(aftermath);
    SceneActor_SetModeZeroAndValue(10, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_SetAnimation(13, 3);
    SceneEffect_ApplyThreeValuesAndFinish(8, 3, 20);
    Actor_StartRepeatedMotion(11, 2);
    Actor_StartRepeatedMotion(12, 2);
    Event_Wait(60);
    Actor_SetSpeed(13, 0xcccc, 0x6666);
    Actor_WalkTo(13, 0x2ea, 0x198);
    Actor_FaceDirection(9, 0xb000, 0);
    Actor_FaceDirection(14, 0xb000, 0);
    Actor_EnableActionCallback(11, (s32)KuupuappuHeya_VaultScriptB);
    Event_Wait(20);
    Actor_EnableActionCallback(10, (s32)KuupuappuHeya_VaultScriptB);
    Event_Wait(15);
    Actor_EnableActionCallback(12, (s32)KuupuappuHeya_VaultScriptB);
    Event_Wait(35);
    Actor_EnableActionCallback(8, KuupuappuHeya_VaultScriptA);
    Event_Wait(20);
    Actor_WaitForMove(13);
    Actor_SetPosition(13, 0, 0);
    Event_Wait(40);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    Actor_SetSpeed(14, 0xcccc, 0x6666);
    Actor_WalkToAndWait(9, 0x310, 0x198);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(14, 0x300, 0x198);
    FieldScene_RunSplitTripleSteps(14, 0, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
    Actor_FaceDirection(14, 0x2000, 10);
    SceneActor_SetModeZeroAndValue(14, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    SceneActor_SetPairZeroAndValue(0, 2, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
    Actor_FaceEachOther(9, 14, 0);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_EnableActionCallback(14, KuupuappuHeya_VaultScriptC);
    Event_Wait(50);
    ((s32 (*)())Engine_ActorEnableActionCallback)(9, (s32)KuupuappuHeya_VaultScriptD);
    Actor_SetSpeed(ACTOR_GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x318, 0x1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x318, 0x198);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1c8);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Event_Wait(100);
    FieldScene_RunSplitTripleSteps(14, 9, 60);
    FieldScene_RunSplitTripleSteps(9, 14, 40);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 40);
    Actor_FaceDirection(9, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(9, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(15, 4);
    Actor_SetPosition(18, 0x3680000, 0x1a80000);
    Actor_SetSpritePriority(18, 1);
    Actor_SetSpeed(18, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(18, 0, -8);
    Actor_WaitForMove(18);
    Actor_RunRepeatedMotion(18, 2);
    Event_Wait(60);
    Message_ShowCentered((aftermath + 5), 1);
    Actor_SetAnimation(15, 2);
    Actor_SetPosition(18, 0, 0);
    {
        u8 *work0 = (u8 *)gEventWork;
        u16 *slot0;
        s32 next0;

        /* FAKEMATCH: the step offset is parked in off1d8 for the later
         * bump and the sum goes through a word-sized temporary; both keep
         * the reference's registers. */
        off1d8 = 0x1d8;
        slot0 = (u16 *)(work0 + off1d8);
        next0 = *slot0 + 1;
        *slot0 = next0;
    }
    Actor_RunRepeatedMotion(14, 1);
    SceneActor_SetModeZeroAndValue(14, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 40);
    FieldScene_RunSplitTripleSteps(9, 14, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
    FieldScene_RunSplitTripleSteps(1, 14, 40);
    Actor_FaceDirection(14, 0, 0);
    Event_Wait(40);
    Actor_RunRepeatedMotion(14, 2);
    Audio_PlayCue(124);
    Actor_SetAnimation(16, 4);
    {
        u8 *rec;
        /* FAKEMATCH: the zero is spelled as a sum of a zero local with
         * itself, which keeps the reference's register for it. */
        s32 t = 0;
        u16 zero_sym = (u16)(t + t);

        rec = (u8 *)Object_GetById(19);
        rec[85] = zero_sym;
    }
    Actor_SetSpritePriority(19, 1);
    Actor_SetPosition(19, 0x3680000, 0x1980000);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_SetDestinationOffset(19, 0, -8);
    Actor_WaitForMove(19);
    Actor_RunRepeatedMotion(19, 2);
    Event_Wait(60);
    Message_ShowCentered((aftermath + 8), 1);
    Actor_SetAnimation(16, 2);
    Actor_SetPosition(19, 0, 0);
    bump_halfword(off1d8, 1);
    Actor_ShowEmote(9, 0x102, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(14, 3, 50);
    FieldScene_RunSplitTripleSteps(9, 0, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_Wait(30);
    Actor_ShowEmote(14, 0x100, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(14, 0x358, 0x178);
    Event_Wait(20);
    FieldScene_RunSplitTripleSteps(14, 9, 20);
    SceneActor_SetModeZeroAndValue(14, 20);
    Actor_FaceActor(9, 14, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x100, 0);
    Event_Wait(60);
    FieldScene_RunSplitTripleSteps(2, 14, 30);
    FieldScene_RunSplitTripleSteps(9, 2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(14, 0x5000, 0);
    Event_Wait(30);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 40);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    Actor_FaceActor(ACTOR_GERALD, 9, 0);
    FieldScene_RunSplitTripleSteps(2, 9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_ShowEmote(ACTOR_IVAN, 0x102, 0);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(2, 40);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 3);
    Event_Wait(30);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 30);
    Actor_SetSpeed(ACTOR_IVAN, 0x18000, 0xc000);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x198);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_ShowMessage(ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    FieldScene_RunSplitTripleSteps(1, 9, 30);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(ACTOR_GERALD, 0x102);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_Wait(60);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_RunRepeatedMotion(9, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Actor_ShowEmote(ACTOR_IVAN, 0x101, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 1);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 40);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(60);
    Actor_WalkToAndWait(9, 0x348, 0x1a8);
    SceneActor_SetPairZeroAndValue(9, 0, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Actor_FaceDirection(9, 0x5000, 0);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    SceneActor_SetModeZeroAndValue(9, 30);
    SceneActor_SetPairZeroAndValue(9, 14, 20);
    SceneActor_SetModeZeroAndValue(9, 20);
    Actor_WalkToAndWait(14, 0x358, 0x198);
    Actor_EnableActionCallback(9, (s32)KuupuappuHeya_VaultScriptE);
    Actor_EnableActionCallback(14, (s32)KuupuappuHeya_VaultScriptE);
    Actor_WalkTo(ACTOR_GERALD, 0x318, 0x1c8);
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    Actor_WalkTo(ACTOR_IVAN, 0x308, 0x1b0);
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_FaceDirection(ACTOR_GERALD, 0xd000, 0);
    Actor_WaitForMove(ACTOR_IVAN);
    Actor_FaceDirection(ACTOR_IVAN, 0xd000, 0);
    Object_RefreshSelectorById(9);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    rec8 = (((s32 (*)())Object_GetById))(14);
    {
        u8 *target = rec8 + 91;
        /* FAKEMATCH: the flag goes through a word-sized local and a
         * pointer temporary, which keeps the reference's registers. */
        s32 shown = 1;

        *target = shown;
    }
    *(s32 *)(rec8 + 56) = ACTOR_NO_TARGET;
    *(s32 *)(rec8 + 60) = ACTOR_NO_TARGET;
    *(s32 *)(rec8 + 64) = ACTOR_NO_TARGET;
    Actor_ShowEmote(9, 0x100, 0);
    Actor_SetAnimation(14, 1);
    Event_Wait(50);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_WalkToAndWait(ACTOR_GERALD, 0x328, 0x1b8);
    Actor_FaceDirection(ACTOR_GERALD, 0xb000, 0);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(9, 20);
    SceneActor_SetPairZeroAndValue(0, 1, 50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
    FieldScene_RunSplitTripleSteps(1, 9, 20);
    SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
    Actor_WalkTo(9, 0x2e8, 0x198);
    Actor_WalkTo(14, 0x2e8, 0x198);
    Actor_WaitForMove(9);
    Actor_WaitForMove(14);
    Event_Wait(30);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x318, 0x198);
    SceneActor_SetPairZeroAndValue(0, 1, 30);
    Actor_ShowEmote(ACTOR_IVAN, 0x105, 0);
    Event_Wait(50);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 40);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    SceneActor_SetPairZeroAndValue(0, 1, 40);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 4);
    SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 40);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x101, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x100, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x102, 0);
    Event_Wait(60);
    SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
    SceneActor_SetModeZeroAndValue(2, 20);
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
    SceneActor_SetModeZeroAndValue(1, 30);
    Event_OpenMessage(ACTOR_IVAN, 0);
    if (Event_ChooseYesNo(0, 0) != 0) {
        Event_Wait(20);
        Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
        Event_Wait(20);
        SceneActor_SetModeZeroAndValue(1, 20);
        SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
        SceneActor_SetModeZeroAndValue(2, 20);
    }
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x178);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_FaceDirection(ACTOR_IVAN, 0, 0);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
    rec8 = BattleFx_PlayCueAndStartEmitterOnTarget(2, 17, 65);
    Event_Wait(60);
    rod = (s32)MsgKuupuappuIvanGotShamansRod;
    Message_ShowCentered(rod, 1);
    Engine_ObjectDispatchRelease((struct FieldActor *)rec8);
    Actor_SetAnimation(17, 2);
    Event_Wait(20);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x358, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x320, 0x1c8);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x1a8);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    FieldScene_RunSplitTripleSteps(1, 2, 30);
    SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
    Event_SetMessage((rod + 1));
    SceneActor_SetModeZeroAndValue(2, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    {
        u16 *slot = (u16 *)(p7 + 0x1d8);
        s32 saved = *(s16 *)slot;

        if (OverlayObject_GetObjectTwoByte118()!= 0) {
            Event_SetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
            Event_ShowMessage(ACTOR_IVAN, 0);
            OverlayObject_RunObjectTwoWhenFlagged();
        }
        Party_RemoveOwnerRestored(2);
        *slot = saved;
    }
    SceneEffect_ApplyThreeValuesAndFinish(1, 3, 50);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x308, 0x198);
    Actor_FaceActor(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Actor_FaceActor(ACTOR_GERALD, ACTOR_IVAN, 0);
    Actor_WalkToAndWait(ACTOR_IVAN, 0x2e8, 0x198);
    Event_Wait(40);
    SceneActor_SetPairZeroAndValue(0, 1, 20);
    Actor_RunRepeatedMotion(ACTOR_GERALD, 1);
    Event_Wait(20);
    SceneActor_SetModeZeroAndValue(1, 20);
    SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
    Actor_SetAnimation(ACTOR_GERALD, 2);
    record = (((s32 (*)())Object_GetById))(0);
    if (record != 0) {
        Actor_SetDestination(ACTOR_GERALD, *(s16 *)(record + 10), *(s16 *)(record + 18));
    }
    Actor_WaitForMove(ACTOR_GERALD);
    Actor_SetPosition(ACTOR_GERALD, 0, 0);
    Event_Wait(30);
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(ACTOR_IVAN, 0, 0);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x209;
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
void SceneState_SetWord1c0To209AndRun(void)
{
    u8 *state;

    state = *(u8 **)&gEventWork;
    *(s32 *)(state + 0x1c0) = 0x209;
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(1);
}

void SceneActor_SetModeZeroAndValue(s32 a, s32 b)
{
    Event_ShowMessage(a, 0);
    Event_Wait(b);
}

void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
    Actor_FaceActor(a, b, 0);
    Event_Wait(c);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
    Actor_FaceEachOther(a, b, 0);
    Event_Wait(c);
}

void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
    Object_SetModeById(a, b, c);
    ObjectMotion_WaitForAnimationChange(a);
    Event_Wait(c);
}

void SceneEffect_ApplyPairWithValue141(s32 a, s32 b)
{
    Psynergy_Begin(141, 1);
    Psynergy_SetTarget(a, b);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Task_Wait(1);
}

void SceneState_SetValue2ThenFinish(void)
{
    Psynergy_PlayEffect(2);
    Psynergy_LowerHands();
    BattleEffect_CleanupSceneObjects();
}

void OverlayObject_ConfigureObject22WithResource17(s32 a)
{
    u8 *o;
    u8 *q;
    u8 *p;
    u8 *v;
    s32 z;
    s32 m;

    z = 0;
    o = Object_CreateFar(22);
    if (o != 0) {
        q = *(u8 **)(o + 0x50);
        p = q + 38;
        *p = z;
        p += 1;
        *p = z;
        m = 33;
        m = -m;
        q[5] &= m;
        q[9] &= 15;
        o[0x55] = z;
        o[0x5c] = 1;
        v = Runtime_AllocateHeapBlock(17, 0x608);
        Item_LoadIcon(a);
        v += 0x400;
        Vram_Load(q[28], 0x80, v);
        Heap_Release(17);
    }
}

u8 *SceneData_FindEntryAtPosition(s32 *o)
{
    s32 x = (o[0] + (s32)0xFFC00000) >> 19;
    s32 y = (o[2] + (s32)0xFD900000) >> 19;
    u8 *e = KuupuappuHeya_Stops;
    u8 *ret = 0;
    u32 i;

    for (i = 0; i <= 36; i++, e += 16) {
        s32 a = e[0];

        if (a == x || a + 1 == x) {
            s32 b = e[1];

            if (b == y || b + 1 == y) {
                ret = e;
                break;
            }
        }
    }
    return ret;
}

/* Snap *pos to the nearest used stop of the set and return that stop's record,
 * or return null when every stop is unused. */
struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopSet *set, s16 *pos)
{
    struct Stop *stop;
    s32 id;
    s32 best;
    s32 at;
    s32 d;
    u32 i;
    u16 p;

    id = -1;
    at = *pos;
    stop = set->stops;
    best = 0x8000;
    for (i = 0; i <= 2; i++, stop++) {
        p = stop->pos << 8;
        d = (s16)(p - *pos);
        if (d < 0)
            d = -d;
        /* FAKEMATCH: the used-stop test reads the id through a volatile
         * lvalue, so the id is loaded again for the result. */
        if (*(volatile u8 *)&stop->id != 0xff && d < best) {
            best = d;
            id = stop->id;
            at = (s16)p;
        }
    }
    if (id == -1)
        return 0;
    *pos = at;
    return &((struct StopRecord *)KuupuappuHeya_Stops)[id];
}

s32 KuupuappuHeya_IsRecordNearPoint(s32 x, s32 y, s32 id)
{
    struct Rec_383b *rec = ObjectTable_Get(id);
    s32 target_x = rec->f56;
    s32 target_y;
    s32 dx;
    s32 dy;

    if (target_x == (s32)0x80000000) {
        target_x = rec->f8;
    }
    target_y = rec->f64;
    if (target_y == (s32)0x80000000) {
        target_y = rec->f16;
    }

    target_x = (target_x - x) >> 16;
    target_y = (target_y - y) >> 16;
    if (target_x * target_x + target_y * target_y <= 64)
        return 1;
    return 0;
}

/* Whether the actor's destination, or its position when it has none, lies
 * within 16 pixels of the point (x, z). */
s32 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id)
{
    struct FieldActor *actor = Engine_ActorLookup(id);
    s32 ax;
    s32 az;

    ax = actor->target_x;
    if (ax == ACTOR_NO_TARGET)
        ax = actor->x.fixed;
    az = actor->target_z;
    if (az == ACTOR_NO_TARGET)
        az = actor->z.fixed;
    ax = (ax - x) >> 16;
    az = (az - z) >> 16;
    if (ax * ax + az * az <= 0x100)
        return 1;
    return 0;
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
s32 SceneActor_CheckTileFreeOfKinds(u8 *p)
{
    s32 x;
    s32 y;

    if (p == 0) {
        return 1;
    }
    x = (p[0] << 19) + 0x480000;
    y = (p[1] << 19) + 0x2780000;
    if (KuupuappuHeya_IsActorNearPoint(x, y, 0) != 0 || KuupuappuHeya_IsRecordNearPoint(x, y, 2) != 0
        || KuupuappuHeya_IsRecordNearPoint(x, y, 24) != 0 || KuupuappuHeya_IsRecordNearPoint(x, y, 25) != 0) {
        return -1;
    }
    return 0;
}

void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
    Object_SetMoveTarget(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}

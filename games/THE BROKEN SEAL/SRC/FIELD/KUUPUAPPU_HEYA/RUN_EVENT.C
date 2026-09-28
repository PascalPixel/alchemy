#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
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
extern u8 LinkedMessage_YouWereSuchGreatHelp[];
extern u8 LinkedMessage_TheyreActingSuspiciousSomethingsNot[];
extern u8 LinkedMessage_TheyreBack[];
extern u8 KuupuappuHeya_ActionTable[];
extern u8 LinkedMessage_YouRobinRightWontForget[];
extern u8 LinkedMessage_IvanGotShamansRod[];

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

    record = Value1(Object_GetById, 12);
    actor12_record = *(u8 **)(record + 80);
    Event_Begin();
    Actor_SetPosition(10, 0x3180000, 0x1a00000);
    Actor_SetPosition(11, 0x3200000, 0x1900000);
    Actor_SetPosition(12, 0x3080000, 0x1880000);
    record = Object_GetById(10);
    Actor_SetSpriteFlags(record, 0);
    record = Object_GetById(11);
    Actor_SetSpriteFlags(record, 0);
    record = Object_GetById(12);
    Actor_SetSpriteFlags(record, 0);
    Actor_SetAnimation(10, 9);
    Actor_SetAnimation(11, 9);
    Actor_SetAnimation(12, 9);
    /* Clear the low bit of the flag byte at +35. */
    *(u8 *)(Object_GetById(12) + 35) &= 254;
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
    Event_SetMessage(MSG_THEY_THEY_GOT_US);
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
    Event_SetMessage(MSG_SEE_THATS_HAPPENED);
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

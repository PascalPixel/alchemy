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

void FieldScene_RunSteps107And250(void)
{
    GameFlag_Set(0x107);
    GameFlag_Set(0x250);
    KuupuappuHeya_StartActorStops();
}
void KuupuappuHeya_StartActorStops(void);

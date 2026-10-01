#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "ITEM_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

/* FAKEMATCH: calls through a cast of Object_GetById keep the unprototyped call
 * this file's code made before it shared the header's declaration. */

void Owner_RecalculateStats();


/* The scene's tables, in the overlay's read-only data. */
extern u8 KuupuappuHeya_Scripts[];
extern u8 KuupuappuHeya_Messages[];
extern u8 KuupuappuHeya_Regions[];

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern u8 KuupuappuHeya_SceneTableB[];
extern u8 KuupuappuHeya_SceneTableA[];
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

extern struct EventRuntime *Data_03001ebc;

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step_020036f8(s32 off, s32 amount)
{
    u8 *work = (u8 *)Data_03001ebc;
    u16 *slot = (u16 *)((s32)work + off);
    s32 next = *slot + amount;

    *slot = next;
}

void FieldScene_PrepareActors(const struct ScenePlacement *placements);

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

extern u8 MsgKuupuappuMasterHammetIsntOnlyOne[];
extern u8 MsgKuupuappuWeFoundOurStolenWeapons[];

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

/*
 * resource_383 owner at 0x02002ba0, 80 bytes.
 * Points two records at a third: each gets the angle from its own offset to the
 * reference record, stored as a halfword at +6.
 */
extern s32 KuupuappuHeya_StepActions[];
extern s32 KuupuappuHeya_IdleActions[];
void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);

extern const struct SceneEvent gKuupuappuHeyaEventsEntrances15To17[];
extern const struct SceneEvent gKuupuappuHeyaEventsFlag855[];
extern const struct SceneEvent gKuupuappuHeyaEvents[];

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
u8 *SceneData_GetScriptTable(void)
{
    return KuupuappuHeya_Scripts;
}

u8 *SceneData_GetRegionTable(void)
{
    return KuupuappuHeya_Regions;
}

u8 *SceneData_GetMessageTable(void)
{
    return KuupuappuHeya_Messages;
}

/* The actors placed in the Vault houses: entrances 15 to 17 have their own
   table. The chosen table is prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    const struct ScenePlacement *table;
    /* FAKEMATCH: a temporary holding the first entrance keeps the compiler
       from rewriting the test as greater than 14. */
    s32 first = 15;

    if (gGameState.entrance <= 17) {
        if (gGameState.entrance >= first) {
            table = ((const struct ScenePlacement *)KuupuappuHeya_SceneTableB);
            goto prepare;
        }
    }
    table = ((const struct ScenePlacement *)KuupuappuHeya_SceneTableA);
prepare:
    FieldScene_PrepareActors(table);
    return table;
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
void ActorPresentation_SetSceneCellByAngle(void)
{
    s32 x;
    s32 z;

    if (*(u16 *)(((u8 *(*)())Object_GetById)(0) + 6) >= 0xa000
        && *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6) <= 0xe000) {
        Engine_LeaderCheckAhead();
        x = 42;
        z = 85;
        Map_CopyCellAttributes(41, 85, 1, 1, x, z);
    } else if (*(u16 *)(((u8 *(*)())Object_GetById)(0) + 6) >= 0x2000
               && *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6) <= 0x6000) {
        Engine_LeaderCheckAhead();
        x = 42;
        z = 85;
        Map_CopyCellAttributes(43, 85, 1, 1, x, z);
    }
}

void FieldScene_RunObjectTwentySixPositionCheck(void)
{
    struct FieldActor *obj;
    s32 x;
    s32 z;

    Engine_EventBegin();
    obj = Object_GetById(26);
    if ((obj->x.fixed >> 20) == 42) {
        x = 41;
        z = 24;
        Map_CopyCellAttributes(101, 24, 3, 4, x, z);
        GameFlag_Set(0x859);
    }
    Engine_EventEnd();
}

/* Actor 19 serves the shop when the leader faces it across the counter and
 * talks otherwise. */
void FieldScene_RunActorNineteenAngleDialogue(void)
{
    s32 v = *(u16 *)((u8 *)Object_GetById(0) + 6);

    Engine_EventBegin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Engine_ShopOpen(4, 19);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Engine_EventSetMessage((s32)MsgKuupuappuMasterHammetIsntOnlyOne);
        } else {
            Engine_EventSetMessage((s32)MsgKuupuappuWeFoundOurStolenWeapons);
        }
        Event_ShowMessage(19, 0);
    }
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
void FieldScene_RunActorTwentyAngleDialogue(void)
{
    s32 v = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Engine_EventBegin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Engine_ShopOpen(5, 20);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Engine_EventSetMessage((s32)MsgKuupuappuThievesDidntHitOurHouse);
        } else {
            Engine_EventSetMessage((s32)MsgKuupuappuIfYoureGonnaHeadInto);
        }
        Event_ShowMessage(20, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunActorTwentyThreeAngleDialogue(void)
{
    s32 v = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Engine_EventBegin();
    if (v >= 0xa001 && v <= 0xdfff) {
        Engine_InnOpen(1, 23);
    } else {
        if (GameFlag_IsSet(0x855) == 0) {
            Engine_EventSetMessage((s32)MsgKuupuappuEveryoneThinksOurGuestsThieves);
        } else {
            Engine_EventSetMessage((s32)MsgKuupuappuTheyHidThoseStolenGoods);
        }
        Event_ShowMessage(23, 0);
    }
    Engine_EventEnd();
}

void FieldScene_RunActorEighteenConditionalScene(void)
{
    Engine_EventBegin();
    if (PartyInventory_HasSpace() == 0) {
        Engine_ActorSetAnimationAndWait(18, 4);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
        Event_ShowMessage(18, 0);
    } else {
        Engine_ItemShowFound(ITEM_BONE, 3);
        Engine_PartyGiveItem(ITEM_BONE, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_ShowLine12BB(void)
{
    Battle_InitializeRenderObject();
    Engine_EventSetMessage((s32)MsgKuupuappuWeDontHaveTimeFor);
    Event_ShowMessage(ACTOR_GERALD, 0);
}

void SceneDialogue_ShowEmptyBarrel(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgKuupuappuRobinCheckedBarrel, 1);
    Engine_MessageShowCentered((s32)MsgKuupuappuButDidntFindAnything, 1);
    Engine_EventEnd();
}

void SceneDialogue_ShowEmptyChest(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgKuupuappuRobinCheckedChest, 1);
    Engine_MessageShowCentered((s32)MsgKuupuappuButChestWasEmpty, 1);
    Engine_EventEnd();
}

/* What the Vault houses answer: entrances 15 to 17 have their own events,
   and the others change once flag 0x855 is set. */
const struct SceneEvent *Scene_GetEvents(void)
{
    /* FAKEMATCH: a temporary holding the first entrance keeps the compiler
       from rewriting the test as greater than 14. */
    s32 first = 15;

    if (gGameState.entrance <= 17) {
        if (gGameState.entrance >= first) {
            return gKuupuappuHeyaEventsEntrances15To17;
        }
    }
    if (GameFlag_IsSet(0x855) != 0) {
        return gKuupuappuHeyaEventsFlag855;
    }
    return gKuupuappuHeyaEvents;
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
void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
    Engine_ActorSetAnimation(x, 1);
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_ShowMessage(x, 0);
}

void SceneState_RunGuardedActorStep(s32 x)
{
    u8 *flag = (u8 *)((s32 (*)())Object_GetById)() + 91;
    s32 zero = 0;

    *flag = 1;
    Engine_EventBegin();
    Engine_ActorSetAnimation(x, 1);
    Engine_EventWait(2);
    Event_ShowMessage(x, 0);
    Engine_EventEnd();
    *flag = zero;
}

void SceneDialogue_PromptAndCountSkip(s32 x)
{
    SceneActor_SetPairZeroAndValue(x, 0, 2);
    Event_OpenMessage(x, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        gEventWork->message += 1;
    }
    Event_ShowMessage(x, 0);
}

void SceneDialogue_RunActorElevenDialogue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuMisterFunSeeStrangeNew);
    Engine_ActorSetAnimation(11, 1);
    SceneDialogue_PromptAndCountSkip(11);
    Engine_EventEnd();
}

void FieldScene_RunScene383_02000428(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuIvanHasGreatPowersWouldnt);
    SceneDialogue_PromptAndCountSkip(15);
    Actor_FaceDirection(15, 0x8000, 0);
    Engine_EventEnd();
}

void SceneState_BranchOnSlotZeroFacingAndFlag855(void)
{
    s32 value = *(u16 *)(((u8 *(*)())Object_GetById)(0) + 6);

    Engine_EventBegin();
    if (value >= 0xa001 && value <= 0xdfff) {
        Engine_ShopOpen(6, 21);
    } else if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuDidJustArriveInTown);
        SceneDialogue_PromptAndCountSkip(21);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuWithBridgeOutWillQuite);
        Event_ShowMessage(21, 0);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor9FlaggedLine(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuIfOnlyTheseRocksWere);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuThankGoodnessThoseThievesWere);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(9);
    Engine_EventEnd();
}

void SceneDialogue_RunActorTwelveFlaggedDialogue(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) != 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuThievesHidStolenTreasureIn);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuJustMeOrAmMissing);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(12);
    Engine_EventEnd();
}

void FieldScene_RunFlag856DialogueBranch(void)
{
    s32 g;
    g = 0x851;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x856) != 0) {
        if (GameFlag_IsSet(g) == 0) {
            Engine_EventSetMessage((s32)MsgKuupuappuYoureGoingHelpIvan);
            ActorPresentation_RunActorModeOneThenZeroWithStep(16);
            Engine_EventWait(10);
            SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
            GameFlag_Set(g);
        } else {
            Engine_EventSetMessage((s32)MsgKuupuappuPleaseLookAfterIvan);
        }
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuCouldSomeonePleaseHelpIvan);
    }
    ActorPresentation_RunActorModeOneThenZeroWithStep(16);
    Engine_EventEnd();
}

void SceneDialogue_ShowLine128E(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuThoseThreeStrangersSureHave);
    ActorPresentation_RunActorModeOneThenZeroWithStep(18);
    Engine_EventEnd();
}

void SceneActor_StepActor24AnimationByFacing(void)
{
    struct FieldActor *p;
    s16 *q;
    s32 v;
    s32 n;

    p = Object_GetById(24);
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(24, 2);
    Engine_EventSetMessage((s32)MsgKuupuappuOwStop);
    Event_ShowMessage(24, 0);
    Actor_SetSpeed(24, 0x40000, 0x20000);
    if ((u32)((p->facing & 0xf000) - 0x5000) <= 0x6000) {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v <= 2) {
            Engine_ActorEnableActionCallback(24, KuupuappuHeya_StepActions[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    } else {
        q = (s16 *)((u8 *)p + 100);
        v = *q;
        if (v > 2) {
            Engine_ActorEnableActionCallback(24, KuupuappuHeya_StepActions[v]);
            *(u16 *)q = *(u16 *)q + 1;
            goto clamp;
        }
    }
    Engine_ActorEnableActionCallback(24, KuupuappuHeya_IdleActions[v]);
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
    Engine_EventEnd();
}

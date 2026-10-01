#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

struct EffectWork {
    u8 pad[80];
    struct EffectRecord *record;
};

struct SceneActor {
    u8 reserved_00[6];
    s16 temporary_state;
    u8 reserved_08[92];
    u16 presentation_flags;
};

struct SceneActor_02000cfc {
    u8 reserved_00[100];
    u16 presentation_flags;
};

struct SceneActor_02000d78 { u8 reserved_00[100]; u16 presentation_flags; };

struct Presentation {
    u8 reserved_00[9];
    u8 flags;
};

struct SceneActor_02000fb4 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_02001010 {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct SceneActor_0200113c {
    u8 reserved_00[35];
    u8 state_23;
    u8 reserved_24[44];
    struct Presentation *presentation;
};

struct OverlayEffectMotion {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[28];
    s32 horizontal_rate;
    s32 vertical_rate;
    s32 shadow_x;
    s32 shadow_y;
    s32 shadow_z;
    u8 pad44[32];
    s16 mode;
};

extern u8 *Data_03001e8c[];
extern u8 *gWork;

/* The scene's tables, laid out after the code. */
extern u8 KuupuappuMura_Scripts[];
extern u8 KuupuappuMura_Messages[];
extern u8 KuupuappuMura_Actors[];
extern u8 KuupuappuMura_ActorsFlag855[];
extern u8 KuupuappuMura_Extras[];
extern u8 KuupuappuMura_ExtrasFlag855[];

/* Map cell steps played as the leader arrives in each scene. */
extern const u16 KuupuappuMura_Scene5Cells[];
extern const u16 KuupuappuMura_Scene6Cells[];
extern const u16 KuupuappuMura_Scene7Cells[];
extern const u16 KuupuappuMura_Scene8Cells[];
extern const u16 KuupuappuMura_Scene9Cells[];
extern const u16 KuupuappuMura_Scene10Cells[];
u8 *Owner_GetState();
void BattlePlacement_UpdateTimedEntriesTwentyTimes(void);
void Djinn_Transfer(int, int, int, int);
s32 Owner_RecalculateStats(int);
void Party_RemoveOwnerRestored();
s32 PartyInventory_FindOwner(s32);
s32 SceneActor_CheckFacingAndRange();
void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);
void SceneActor_RunActorCommandWithFlag91(s32 x);

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

extern u8 MsgFieldPeeredWell[];
extern u8 MsgKuupuappuAccusingUsStealingHammetsTreasured[];
extern u8 MsgKuupuappuCanHearWaterRumblingDown[];
extern u8 MsgKuupuappuGuessFolksGot[];
extern u8 MsgKuupuappuLeavingImStillWorriedAbout[];
extern u8 MsgKuupuappuMasterHammetsCaravan[];
extern u8 MsgKuupuappuNobodysStealingAnything[];
extern u8 MsgKuupuappuOffOnAdventure[];
extern u8 MsgKuupuappuPoorGuyLeft[];
extern u8 MsgKuupuappuRuffRrruff[];
extern u8 MsgKuupuappuThoseTravelersLeftInBig[];
extern u8 MsgKuupuappuWaitDontWantTakeYour[];
extern u8 MsgKuupuappuWasntEruptionMtAlephIncredible[];

extern u8 MsgKuupuappuFixingRoofCant[];

extern u8 MsgKuupuappuBetWasThoseThreeCreeps[];
extern u8 MsgKuupuappuCarefulSearchWillRevealPassage[];
extern u8 MsgKuupuappuDidntGoLunpa[];
extern u8 MsgKuupuappuEruptionDevastatedRoads[];
extern u8 MsgKuupuappuEveryoneGratefulCaptured[];
extern u8 MsgKuupuappuEveryoneKnowsThoseThreeAt[];
extern u8 MsgKuupuappuFeelMuchBetter[];
extern u8 MsgKuupuappuGreatCaughtThieves[];
extern u8 MsgKuupuappuGroupTravelersWasStrangeBunch[];
extern u8 MsgKuupuappuHowPunishPrisoners[];
extern u8 MsgKuupuappuIfBringMeBoneIll[];
extern u8 MsgKuupuappuManShouldStealFromAnother[];
extern u8 MsgKuupuappuMustStrongerThanLookHave[];
extern u8 MsgKuupuappuOnesWhoCapturedThieves[];
extern u8 MsgKuupuappuRuffRrruff2[];
extern u8 MsgKuupuappuSneakWastingTime[];
extern u8 MsgKuupuappuStealingInMidstVolcanicEruption[];
extern u8 MsgKuupuappuSupposeDoesntMatterHowRich[];
extern u8 MsgKuupuappuTalkingAboutHammetsServantIvan[];
extern u8 MsgKuupuappuThankForOtherDayLeaving[];
extern u8 MsgKuupuappuThoseMenCapturedTheyreIn[];
extern u8 MsgKuupuappuWhereDidIvanGoBy[];
extern u8 MsgKuupuappuWithRoadOutOnlyWay[];
extern u8 MsgKuupuappuYoureHittingRoadAgain[];

/*
 * Per-frame callback for one actor record, installed into field +0x6c of
 * actors 14 and 15 by the overlay initialiser. Always returns 0. The work
 * pointer table holds the scene record at entry 0 and the scene work at
 * entry 12, the same pointer the rest of this overlay reads as gWork.
 */
s32 SceneActor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = Data_03001e8c;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];        /* the scene work, gWork */
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    u8 *partner;
    u8 *player;

    /*
     * Bit 0 of the actor's own flag halfword selects which partner to test.
     * This must stay two calls rather than one call on a conditional
     * expression: the conditional form folds to arithmetic on the flag.
     */
    if ((*flags & 1) != 0) {
        partner = Actor_Get(15);
    } else {
        partner = Actor_Get(14);
    }
    if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Actor_Get(ACTOR_PARTY_LEADER);

    /*
     * Widen the range when the scene counter at workspace + 376 is already
     * running, or when the scene byte at scene + 0x0ea4 is set.
     */
    if (*(s16 *)(workspace + 376) != 0 || scene[0x0ea4] != 0) {
        range = 26;
        if ((*flags & 2) != 0) {
            force = 1;
        }
    }

    SceneActor_CheckFacingAndRange(self, player, range, force);
    return 0;
}

s32 ActorPresentation_UpdateEntityFromLeader(u8 *entity)
{
    u8 *base = Data_03001e8c[0];
    u8 *workspace = Data_03001e8c[12];
    s32 flag = 0;
    s32 selector = 18;
    u8 *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (*(s16 *)(workspace + 376) != 0 || base[0x0ea4] != 0) {
        selector = 26;
        flag = 1;
    }
    SceneActor_CheckFacingAndRange(entity, leader, selector, flag);
    return 0;
}

void *SceneData_GetScriptTable(void)
{
    return KuupuappuMura_Scripts;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *SceneData_GetMessageTable(void)
{
    return KuupuappuMura_Messages;
}

void *SceneData_SelectActorTableByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return KuupuappuMura_ActorsFlag855;
    return KuupuappuMura_Actors;
}

int OverlayObject_GetObject2Byte280(void)
{
    return Owner_GetState(2)[280];
}

/*
 * The flagged path passes the result of its final call back to the caller, so
 * this is spelled as a tail call and the return type is s32. The
 * fall-through path returns nothing.
 */
s32 OverlayObject_RunObject2WhenFlagged(void)
{
    BattlePlacement_UpdateTimedEntriesTwentyTimes();
    if ((*(u32 *)(Owner_GetState(2) + 248) & 1) != 0) {
        Djinn_Transfer(2, 0, 0, 0);
        Audio_PlayCue(126);
        Owner_RecalculateStats(0);
        return Owner_RecalculateStats(2);
    }
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
void FieldScene_RunScene382_020004a0(void)
{
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0) {
        Engine_EventRequestExit(p5->touched_trigger - 19);
        return;
    }
    Engine_EventBegin();
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)(record + 8), *(s32 *)(record + 16));
    }
    Actor_SetSpeed(ACTOR_IVAN, 0xcccc, 0x6666);
    if (p5->touched_trigger == 20) {
        Actor_WalkToAndWait(ACTOR_IVAN, 0x190, 0x1c0);
    } else {
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(PIXELS(0xE0), -1, PIXELS(0xA2), 1);
        Actor_WalkToAndWait(ACTOR_IVAN, 224, 162);
        Engine_CameraWaitForMove();
    }
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_IVAN, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKuupuappuLeavingImStillWorriedAbout);
    Event_ShowMessageAndWait(0x9002, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (OverlayObject_GetObject2Byte280()!= 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
        Event_ShowMessage(ACTOR_IVAN, 0);
        OverlayObject_RunObject2WhenFlagged();
        Engine_TaskWait(20);
    }
    Party_RemoveOwnerRestored(2);
    Engine_EventRequestExit(p5->touched_trigger - 19);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void SceneState_SetFlags947And29dc(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgKuupuappuCanHearWaterRumblingDown, 1);
    Engine_EventEnd();
}

void *SceneData_SelectTableA414ByFlag855(void)
{
    if (GameFlag_IsSet(0x855) != 0)
        return KuupuappuMura_ExtrasFlag855;
    return KuupuappuMura_Extras;
}

void SceneDialogue_RunActor9LineAndAdvance(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuWasntEruptionMtAlephIncredible);
    SceneActor_ApplyActorCueThenWait(9, 0, 2);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(9, 0);
    Actor_FaceDirection(9, 0x5000, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorThirteenSceneSetup(void)
{
    u8 *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuThoseTravelersLeftInBig);
    Engine_ActorSetAnimation(13, 1);
    SceneActor_ApplyActorCueThenWait(13, 0, 2);
    Event_OpenMessage(13, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(13, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    int Engine_EventChooseYesNo(int, int);

    u8 *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuAccusingUsStealingHammetsTreasured);
    SceneActor_ApplyActorCueThenWait(17, 0, 2);
    Event_OpenMessage(17, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(17, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuOffOnAdventure);
    SceneActor_ApplyActorCueThenWait(18, 0, 2);
    Event_OpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gWork;
        ++*(u16 *)(workspace + 472);
    }
    Event_ShowMessage(18, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor11Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGuessFolksGot); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor16Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuNobodysStealingAnything); SceneActor_RunActorStep(16); Engine_EventEnd(); }

void SceneDialogue_RunActor19Line(void)
{
    void Event_ShowMessage(int, int);

    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff); Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2); Event_ShowMessage(19, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    void Engine_ActorSetAnimation(s32, s32);

    struct SceneActor *actor = Actor_Get(14);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuPoorGuyLeft);
    Engine_ActorSetAnimation(14, 0);
    SceneActor_ApplyActorCueThenWait(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->temporary_state = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct SceneActor *actor = Actor_Get(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuMasterHammetsCaravan);
    Engine_ActorSetAnimation(15, 0);
    SceneActor_ApplyActorCueThenWait(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->temporary_state = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->presentation_flags &= 1;
}

/* The villager who cannot find the man meant to be fixing the roof. */
void Villager_LookForRoofer(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuFixingRoofCant);
    SceneActor_ApplyActorCueThenWait(21, 0, 2);
    Actor_ShowEmote(21, 0x103, 0);
    Engine_EventWait(30);
    Event_OpenMessage(21, 0);
    Engine_EventEnd();
}

/*
 * Kuupuappu scene steps after the roof dialogue: actor lines and the
 * leader's arrival in scenes five to ten.
 */

/* The scene step counter at 0x1d8 of the shared scene work record. */
void SceneActor_RunActorStep(int actor)
{
    void Event_ShowMessage(int, int);

    Engine_EventBegin(); Engine_ActorSetAnimation(actor, 1); SceneActor_ApplyActorCueThenWait(actor, 0, 2);
    Event_ShowMessage(actor, 0); Engine_EventEnd();
}

void SceneActor_RunActorCommandWithFlag91(s32 x)
{
    void Event_ShowMessage(s32, s32);

    u8 *flag = (u8 *)Actor_Get(x) + 91;
    s32 zero = 0;

    *flag = 1;
    Engine_EventBegin();
    Engine_ActorSetAnimation(x, 1);
    Engine_EventWait(2);
    Event_ShowMessage(x, 0);
    Engine_EventEnd();
    *flag = zero;
}

void ActorPresentation_RunActorEightSceneSetup(void)
{
    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuYoureHittingRoadAgain); SceneActor_ApplyActorCueThenWait(8, 0, 2); Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gWork; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(8, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor11SecondLine(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGreatCaughtThieves); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuTalkingAboutHammetsServantIvan);
    if (GameFlag_IsSet(2) != 0) { workspace = gWork; ++*(u16 *)(workspace + 472); }
    SceneActor_RunActorStep(12); Engine_EventEnd();
}

void SceneDialogue_RunActor13Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuEruptionDevastatedRoads); SceneActor_RunActorStep(13); Engine_EventEnd(); }

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    void Engine_TaskWait(s32);

    struct SceneActor *actor = Actor_Get(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;
    /* tmp keeps the flag result live in a register; do not fold it away. */
    s32 tmp;

    *flags = (tmp = *flags | 2);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuDidntGoLunpa);
    if (GameFlag_IsSet(2) != 0)
        ++gEventWork->message;
    Engine_ActorSetAnimation(14, 0);
    SceneActor_ApplyActorCueThenWait(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->temporary_state = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    *flags &= 1;
}

void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
    struct SceneActor *actor = Actor_Get(15);
    s16 saved = actor->temporary_state;

    actor->presentation_flags |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuEveryoneGratefulCaptured);
    Engine_ActorSetAnimation(15, 0);
    SceneActor_ApplyActorCueThenWait(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->temporary_state = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenSceneSetup(void)
{
    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuThoseMenCapturedTheyreIn); Engine_ActorSetAnimation(16, 1); SceneActor_ApplyActorCueThenWait(16, 0, 2); Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gWork; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(16, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
    void Event_ShowMessage(int, int);

    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuOnesWhoCapturedThieves); SceneActor_ApplyActorCueThenWait(18, 0, 2); Event_OpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gWork; ++*(u16 *)(workspace + 472); }
    Event_ShowMessage(18, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    void Engine_ActorSetAnimation(s32, s32);
    void Engine_ActorSetAnimation(s32, s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
    Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2);
    Event_ShowMessage(19, 0);
    Engine_ActorSetAnimation(19, 1);
    if (PartyInventory_FindOwner(231) != -1 && GameFlag_IsSet(0x858) == 0) {
        u16 *p = (u16 *)(gWork + 370);
        u16 value = 1;

        *p = value;
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor20Line(void)
{
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuThankForOtherDayLeaving); SceneActor_ApplyActorCueThenWait(20, 0, 2); Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20); Event_ShowMessage(20, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor11FlaggedLine(void)
{
    int GameFlag_IsSet(int);

    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) Engine_EventSetMessage((s32)MsgKuupuappuManShouldStealFromAnother); else Engine_EventSetMessage((s32)MsgKuupuappuHowPunishPrisoners);
    SceneActor_RunActorCommandWithFlag91(11); Engine_EventEnd();
}

void SceneDialogue_RunActor13FlaggedLine(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuGroupTravelersWasStrangeBunch);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuWithRoadOutOnlyWay);
    }
    SceneActor_RunActorCommandWithFlag91(13);
    Engine_EventEnd();
}

void ActorPresentation_RunActorFourteenFlaggedDialogue(void)
{
    ((struct SceneActor *)Actor_Get(14))->presentation_flags |= 2;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuBetWasThoseThreeCreeps);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuWhereDidIvanGoBy);
        if (GameFlag_IsSet(2) != 0)
            ++gEventWork->message;
    }
    SceneActor_RunActorCommandWithFlag91(14);
    Engine_EventEnd();
    ((struct SceneActor *)Actor_Get(14))->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    void Engine_EventSetMessage(s32);

    ((struct SceneActor *)Actor_Get(15))->presentation_flags |= 2;
    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0)
        Engine_EventSetMessage((s32)MsgKuupuappuSupposeDoesntMatterHowRich);
    else
        Engine_EventSetMessage((s32)MsgKuupuappuMustStrongerThanLookHave);
    SceneActor_RunActorCommandWithFlag91(15);
    Engine_EventEnd();
    ((struct SceneActor *)Actor_Get(15))->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenScriptBranch(void)
{
    void Engine_EventSetMessage(int);

    Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) Engine_EventSetMessage((s32)MsgKuupuappuEveryoneKnowsThoseThreeAt); else Engine_EventSetMessage((s32)MsgKuupuappuFeelMuchBetter);
    SceneActor_RunActorCommandWithFlag91(16); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenScriptBranch(void)
{
    void Engine_EventWait(int);

    u8 *actor = Actor_Get(19); actor[91] = 1; Engine_EventBegin();
    if (GameFlag_IsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuStealingInMidstVolcanicEruption); Engine_ActorSetAnimation(19, 0); Engine_EventWait(2);
    } else if (GameFlag_IsSet(0x858) != 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuCarefulSearchWillRevealPassage);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuIfBringMeBoneIll);
    }
    Event_ShowMessage(19, 0); Engine_EventEnd(); actor[91] = 0;
}

void SceneDialogue_RunActor21Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuSneakWastingTime); SceneActor_RunActorCommandWithFlag91(21); Engine_EventEnd(); }

void SceneState_Apply200ThenPlace55_26(void)
{
    GameFlag_Set(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Map_CopyCellAttributes(55, 26, 4, 2, v1, v2);
    }
}

void SceneState_Apply200ThenPlace23_23(void)
{
    GameFlag_Clear(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Map_CopyCellAttributes(23, 23, 4, 2, v1, v2);
    }
}

void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
    Actor_SetSpeed(0, 0x8000, 0x4000);
    Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
    gEventWork->transition_frames = 16;
    Engine_EventRequestExit(continuation);
}

void FieldScene_SetupScene5At408_320(void)
{
    Audio_PlayCue(158); Map_AnimateCells(KuupuappuMura_Scene5Cells, 56, 19); SceneActor_PlaceAndSetSceneDelay(408, 320, 5);
}

void FieldScene_SetupScene6At312_304(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene6Cells, 50, 18); SceneActor_PlaceAndSetSceneDelay(312, 304, 6); }

void FieldScene_SetupScene7At216_288(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene7Cells, 44, 17); SceneActor_PlaceAndSetSceneDelay(216, 288, 7); }

void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor_02000fb4 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMura_Scene8Cells, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{

    struct SceneActor_02001010 *actor = Actor_Get(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Audio_PlayCue(158);
    Map_AnimateCells(KuupuappuMura_Scene9Cells, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Map_CopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10At120_144(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene10Cells, 38, 6); SceneActor_PlaceAndSetSceneDelay(120, 144, 10); }

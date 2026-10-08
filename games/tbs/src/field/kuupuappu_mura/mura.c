#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "tbs_edition.h"
#include "btlpty.h"
#include "text/msg_ids.h"

#define UnitCalc ((s32 (*)(int))UnitCalc)

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
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


u8 *UnitGet();
void Djinn_Transfer(int, int, int, int);
void Party_RemoveOwnerRestored();
s32 PartyInventory_FindOwner(s32);
s32 SceneActor_CheckFacingAndRange();

s32 SceneActor_UpdateProximityToLeader(u8 *self)
{
    u8 **globals = gWindowWork;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];
    u16 *flags = (u16 *)(self + 100);
    s32 force = 0;
    s32 range = 18;
    struct FieldActor *partner;
    struct FieldActor *player;

    if ((*flags & 1) != 0) {
        partner = Object_GetById(15);
    } else {
        partner = Object_GetById(14);
    }
    if (SceneActor_CheckFacingAndRange(self, partner, 32, 0) != 0) {
        return 0;
    }

    player = Object_GetById(ACTOR_PARTY_LEADER);

    if (*(s16 *)(workspace + 376) != 0 || scene[RENDER_MODE_OFS] != 0) {
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
    u8 *base = gWindowWork[0];
    u8 *workspace = gWindowWork[12];
    s32 flag = 0;
    s32 selector = 18;
    struct FieldActor *leader;

    if (*(s32 *)(entity + 56) == (s32)0x80000000)
        return 0;

    leader = Object_GetById(ACTOR_PARTY_LEADER);
    if (*(s16 *)(workspace + 376) != 0 || base[RENDER_MODE_OFS] != 0) {
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
    if (Engine_GameFlagIsSet(0x855) != 0)
        return KuupuappuMura_ActorsFlag855;
    return KuupuappuMura_Actors;
}

int KuupuappuMura_GetIwanVenusDjinnCount(void)
{
    return ((struct BtlUnit *)UnitGet(2))->djinn_owned_counts[0];
}

s32 OverlayObject_RunObject2WhenFlagged(void)
{
    BattlePlacement_UpdateTimedEntriesTwentyTimes();
    if ((*(u32 *)(UnitGet(2) + 248) & 1) != 0) {
        Djinn_Transfer(2, 0, 0, 0);
        Engine_AudioPlayCue(126);
        UnitCalc(0);
        return UnitCalc(2);
    }
}

void KuupuappuMura_SayFarewellToIvan(void)
{
    struct FieldActor *record;
    struct EventWork *p5;

    p5 = gEventWork;
    if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0) {
        Engine_EventRequestExit(p5->touched_trigger - 19);
        return;
    }
    Engine_EventBegin();
    record = Actor_Get(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    if (p5->touched_trigger == 20) {
        Actor_WalkToAndWait(IWAN, 0x190, 0x1c0);
    } else {
        Camera_SetSpeed(0xcccc, 0x1999);
        Camera_MoveTo(PIXELS(0xE0), -1, PIXELS(0xA2), 1);
        Actor_WalkToAndWait(IWAN, 224, 162);
        Engine_CameraWaitForMove();
    }
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgKuupuappuLeavingImStillWorriedAbout);
    Event_ShowMessageAndWait(0x9002, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    if (KuupuappuMura_GetIwanVenusDjinnCount()!= 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
        Event_ShowMessage(IWAN, 0);
        OverlayObject_RunObject2WhenFlagged();
        Engine_TaskWait(20);
    }
    Party_RemoveOwnerRestored(2);
    Engine_EventRequestExit(p5->touched_trigger - 19);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void KuupuappuMura_InspectWell(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgKuupuappuCanHearWaterRumblingDown, 1);
    Engine_EventEnd();
}

void *KuupuappuMura_SelectEvents(void)
{
    if (Engine_GameFlagIsSet(0x855) != 0)
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
        gEventWork->message++;
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
    Engine_EventOpenMessage(13, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = (u8 *)gEventWork;
        ++*(u16 *)(workspace + 472);
    }
    Engine_EventShowMessage(13, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    int Engine_EventChooseYesNo(int, int);

    u8 *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuAccusingUsStealingHammetsTreasured);
    SceneActor_ApplyActorCueThenWait(17, 0, 2);
    Engine_EventOpenMessage(17, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = (u8 *)gEventWork;
        ++*(u16 *)(workspace + 472);
    }
    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    u8 *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuOffOnAdventure);
    SceneActor_ApplyActorCueThenWait(18, 0, 2);
    Engine_EventOpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = (u8 *)gEventWork;
        ++*(u16 *)(workspace + 472);
    }
    Engine_EventShowMessage(18, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor11Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGuessFolksGot); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor16Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuNobodysStealingAnything); SceneActor_RunActorStep(16); Engine_EventEnd(); }

void SceneDialogue_RunActor19Line(void)
{
    void Engine_EventShowMessage(int, int);

    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff); Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2); Engine_EventShowMessage(19, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    void Engine_ActorSetAnimation(s32, s32);

    struct SceneActor *actor = Object_GetById(14);
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
    struct SceneActor *actor = Object_GetById(15);
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

void Villager_LookForRoofer(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuFixingRoofCant);
    SceneActor_ApplyActorCueThenWait(21, 0, 2);
    Engine_ActorShowEmote(21, 0x103, 0);
    Engine_EventWait(30);
    Engine_EventOpenMessage(21, 0);
    Engine_EventEnd();
}

void SceneActor_RunActorStep(int actor)
{
    void Engine_EventShowMessage(int, int);

    Engine_EventBegin(); Engine_ActorSetAnimation(actor, 1); SceneActor_ApplyActorCueThenWait(actor, 0, 2);
    Engine_EventShowMessage(actor, 0); Engine_EventEnd();
}

void SceneActor_RunActorCommandWithFlag91(s32 x)
{
    void Engine_EventShowMessage(s32, s32);

    u8 *flag = (u8 *)Object_GetById(x) + 91;
    s32 zero = 0;

    *flag = 1;
    Engine_EventBegin();
    Engine_ActorSetAnimation(x, 1);
    Engine_EventWait(2);
    Engine_EventShowMessage(x, 0);
    Engine_EventEnd();
    *flag = zero;
}

void ActorPresentation_RunActorEightSceneSetup(void)
{
    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuYoureHittingRoadAgain); SceneActor_ApplyActorCueThenWait(8, 0, 2); Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = (u8 *)gEventWork; ++*(u16 *)(workspace + 472); }
    Engine_EventShowMessage(8, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor11SecondLine(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGreatCaughtThieves); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor12LineAndAdvance(void)
{
    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuTalkingAboutHammetsServantIvan);
    if (Engine_GameFlagIsSet(2) != 0) { workspace = (u8 *)gEventWork; ++*(u16 *)(workspace + 472); }
    SceneActor_RunActorStep(12); Engine_EventEnd();
}

void SceneDialogue_RunActor13Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuEruptionDevastatedRoads); SceneActor_RunActorStep(13); Engine_EventEnd(); }

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    void Engine_TaskWait(s32);

    struct SceneActor *actor = Object_GetById(14);
    u16 *flags = &actor->presentation_flags;
    s16 saved = actor->temporary_state;

    s32 tmp;

    *flags = (tmp = *flags | 2);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuDidntGoLunpa);
    if (Engine_GameFlagIsSet(2) != 0)
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
    struct SceneActor *actor = Object_GetById(15);
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
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuThoseMenCapturedTheyreIn); Engine_ActorSetAnimation(16, 1); SceneActor_ApplyActorCueThenWait(16, 0, 2); Engine_EventOpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = (u8 *)gEventWork; ++*(u16 *)(workspace + 472); }
    Engine_EventShowMessage(16, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
    void Engine_EventShowMessage(int, int);

    u8 *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuOnesWhoCapturedThieves); SceneActor_ApplyActorCueThenWait(18, 0, 2); Engine_EventOpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = (u8 *)gEventWork; ++*(u16 *)(workspace + 472); }
    Engine_EventShowMessage(18, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    void Engine_ActorSetAnimation(s32, s32);
    void Engine_ActorSetAnimation(s32, s32);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
    Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimation(19, 1);
    if (PartyInventory_FindOwner(231) != -1 && Engine_GameFlagIsSet(0x858) == 0) {
        u16 *p = (u16 *)((u8 *)gEventWork + 370);
        u16 value = 1;

        *p = value;
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor20Line(void)
{
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuThankForOtherDayLeaving); SceneActor_ApplyActorCueThenWait(20, 0, 2); Engine_ActorSetAnimationAndWait(20, 3);
    Engine_EventWait(20); Engine_EventShowMessage(20, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor11FlaggedLine(void)
{
    int Engine_GameFlagIsSet(int);

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) Engine_EventSetMessage((s32)MsgKuupuappuManShouldStealFromAnother); else Engine_EventSetMessage((s32)MsgKuupuappuHowPunishPrisoners);
    SceneActor_RunActorCommandWithFlag91(11); Engine_EventEnd();
}

void SceneDialogue_RunActor13FlaggedLine(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuGroupTravelersWasStrangeBunch);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuWithRoadOutOnlyWay);
    }
    SceneActor_RunActorCommandWithFlag91(13);
    Engine_EventEnd();
}

void ActorPresentation_RunActorFourteenFlaggedDialogue(void)
{
    ((struct SceneActor *)Object_GetById(14))->presentation_flags |= 2;
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuBetWasThoseThreeCreeps);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuWhereDidIvanGoBy);
        if (Engine_GameFlagIsSet(2) != 0)
            ++gEventWork->message;
    }
    SceneActor_RunActorCommandWithFlag91(14);
    Engine_EventEnd();
    ((struct SceneActor *)Object_GetById(14))->presentation_flags &= 1;
}

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    void Engine_EventSetMessage(s32);

    ((struct SceneActor *)Object_GetById(15))->presentation_flags |= 2;
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0)
        Engine_EventSetMessage((s32)MsgKuupuappuSupposeDoesntMatterHowRich);
    else
        Engine_EventSetMessage((s32)MsgKuupuappuMustStrongerThanLookHave);
    SceneActor_RunActorCommandWithFlag91(15);
    Engine_EventEnd();
    ((struct SceneActor *)Object_GetById(15))->presentation_flags &= 1;
}

void ActorPresentation_RunActorSixteenScriptBranch(void)
{
    void Engine_EventSetMessage(int);

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) Engine_EventSetMessage((s32)MsgKuupuappuEveryoneKnowsThoseThreeAt); else Engine_EventSetMessage((s32)MsgKuupuappuFeelMuchBetter);
    SceneActor_RunActorCommandWithFlag91(16); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenScriptBranch(void)
{
    void Engine_EventWait(int);

    struct FieldActor *actor = Object_GetById(19); actor->paused = 1; Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuStealingInMidstVolcanicEruption); Engine_ActorSetAnimation(19, 0); Engine_EventWait(2);
    } else if (Engine_GameFlagIsSet(0x858) != 0) {
        Engine_EventSetMessage((s32)MsgKuupuappuCarefulSearchWillRevealPassage);
    } else {
        Engine_EventSetMessage((s32)MsgKuupuappuIfBringMeBoneIll);
    }
    Engine_EventShowMessage(19, 0); Engine_EventEnd(); actor->paused = 0;
}

void SceneDialogue_RunActor21Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuSneakWastingTime); SceneActor_RunActorCommandWithFlag91(21); Engine_EventEnd(); }

void SceneState_Apply200ThenPlace55_26(void)
{
    Engine_GameFlagSet(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Engine_MapCopyCellAttributes(55, 26, 4, 2, v1, v2);
    }
}

void SceneState_Apply200ThenPlace23_23(void)
{
    Engine_GameFlagClear(0x200);
    {
        int v1 = 23;
        int v2 = 26;
        Engine_MapCopyCellAttributes(23, 23, 4, 2, v1, v2);
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
    Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene5Cells, 56, 19); SceneActor_PlaceAndSetSceneDelay(408, 320, 5);
}

void FieldScene_SetupScene6At312_304(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene6Cells, 50, 18); SceneActor_PlaceAndSetSceneDelay(312, 304, 6); }

void FieldScene_SetupScene7At216_288(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene7Cells, 44, 17); SceneActor_PlaceAndSetSceneDelay(216, 288, 7); }

void ActorPresentation_SetupActorZeroForSceneEightAt376_224(void)
{
    struct SceneActor_02000fb4 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene8Cells, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{

    struct SceneActor_02001010 *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct Presentation *presentation = actor->presentation;
    u8 flags;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene9Cells, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->state_23 &= ~1;
    flags = presentation->flags;
    flags |= 12;
    presentation->flags = flags;
    SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10At120_144(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene10Cells, 38, 6); SceneActor_PlaceAndSetSceneDelay(120, 144, 10); }

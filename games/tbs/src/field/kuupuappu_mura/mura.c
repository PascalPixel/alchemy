#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "tbs_edition.h"
#include "btlpty.h"
#include "text/msg_ids.h"
#include "call.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "inventory.h"
#include "b5_context.h"

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void SceneEffect_SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareSceneObject(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Engine_ObjectCreate(fourth, first, second, third);
    if (obj != NULL) {
        rec = obj->sprite;
        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    struct FieldActor *result = Engine_ObjectCreate(arg3, arg0, arg1, arg2);

    if (result != NULL) {
        struct FieldSprite *object = result->sprite;

        object->priority = 1;
        result->motion_flags = 0;
        result->collision_flags = 8;
        Engine_ActorSetSpriteFlags(result, 0);
        ObjectGroup_SetChildValue(result, 15);
        result->priority_flags = (result->priority_flags & 0xfe) | 2;
        return result;
    }
    return NULL;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawnov.c"

union DriftingObject {
    struct FieldActor actor;
    struct OverlayEffectMotion motion;
};

void Djinn_Transfer(s32 owner, s32 element, s32 index, s32 target);
void Party_RemoveOwnerRestored(s32 owner);
void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value);
s32 ArcTan2(s32 x, s32 y);

s32 SceneActor_CheckFacingAndRange(struct FieldActor *actor, struct FieldActor *target, s32 range, s32 force);
void SceneActor_RunActorStep(s32 actor);
void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect);

s32 SceneActor_UpdateProximityToLeader(struct FieldActor *self)
{
    u8 **globals = gWindowWork;
    u8 *scene = globals[0];
    u8 *workspace = globals[12];
    s16 *flags = &self->timer;
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

s32 ActorPresentation_UpdateEntityFromLeader(struct FieldActor *entity)
{
    u8 *base = gWindowWork[0];
    u8 *workspace = gWindowWork[12];
    s32 flag = 0;
    s32 selector = 18;
    struct FieldActor *leader;

    if (entity->target_x == (s32)0x80000000)
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
        /* FAKEMATCH: the native returns through r1, so this function returns
           the last call's value; character.h declares UnitCalc void, and a
           void tail returns through r0 (differ 4). */
        return ((s32 (*)(s32))UnitCalc)(2);
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
    struct EventWork *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuThoseTravelersLeftInBig);
    Engine_ActorSetAnimation(13, 1);
    SceneActor_ApplyActorCueThenWait(13, 0, 2);
    Engine_EventOpenMessage(13, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gEventWork;
        ++workspace->message;
    }
    Engine_EventShowMessage(13, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorSeventeenSceneSetup(void)
{
    struct EventWork *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuAccusingUsStealingHammetsTreasured);
    SceneActor_ApplyActorCueThenWait(17, 0, 2);
    Engine_EventOpenMessage(17, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gEventWork;
        ++workspace->message;
    }
    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenSceneSetup(void)
{
    struct EventWork *workspace;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuOffOnAdventure);
    SceneActor_ApplyActorCueThenWait(18, 0, 2);
    Engine_EventOpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        workspace = gEventWork;
        ++workspace->message;
    }
    Engine_EventShowMessage(18, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor11Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGuessFolksGot); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor16Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuNobodysStealingAnything); SceneActor_RunActorStep(16); Engine_EventEnd(); }

void SceneDialogue_RunActor19Line(void)
{
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff); Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2); Engine_EventShowMessage(19, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorFourteenDialogue(void)
{
    struct FieldActor *actor = Object_GetById(14);
    s16 saved = actor->facing;

    actor->timer |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuPoorGuyLeft);
    Engine_ActorSetAnimation(14, 0);
    SceneActor_ApplyActorCueThenWait(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->facing = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->timer &= 1;
}

void ActorPresentation_RunActorFifteenDialogue(void)
{
    struct FieldActor *actor = Object_GetById(15);
    s16 saved = actor->facing;

    actor->timer |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuMasterHammetsCaravan);
    Engine_ActorSetAnimation(15, 0);
    SceneActor_ApplyActorCueThenWait(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->facing = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->timer &= 1;
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

void SceneActor_RunActorStep(s32 actor)
{
    Engine_EventBegin(); Engine_ActorSetAnimation(actor, 1); SceneActor_ApplyActorCueThenWait(actor, 0, 2);
    Engine_EventShowMessage(actor, 0); Engine_EventEnd();
}

void SceneActor_RunActorCommandWithFlag91(s32 x)
{
    u8 *flag = &Object_GetById(x)->paused;
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
    struct EventWork *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuYoureHittingRoadAgain); SceneActor_ApplyActorCueThenWait(8, 0, 2); Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gEventWork; ++workspace->message; }
    Engine_EventShowMessage(8, 0); Engine_EventEnd();
}

void SceneDialogue_RunActor11SecondLine(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuGreatCaughtThieves); SceneActor_RunActorStep(11); Engine_EventEnd(); }

void SceneDialogue_RunActor12LineAndAdvance(void)
{
    struct EventWork *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuTalkingAboutHammetsServantIvan);
    if (Engine_GameFlagIsSet(2) != 0) { workspace = gEventWork; ++workspace->message; }
    SceneActor_RunActorStep(12); Engine_EventEnd();
}

void SceneDialogue_RunActor13Line(void) { Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuEruptionDevastatedRoads); SceneActor_RunActorStep(13); Engine_EventEnd(); }

void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
    struct FieldActor *actor = Object_GetById(14);
    u16 *flags = (u16 *)&actor->timer;
    s16 saved = actor->facing;

    s32 tmp;

    *flags = (tmp = *flags | 2);
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuDidntGoLunpa);
    if (Engine_GameFlagIsSet(2) != 0)
        ++gEventWork->message;
    Engine_ActorSetAnimation(14, 0);
    SceneActor_ApplyActorCueThenWait(14, 0, 2);
    SceneActor_ApplyActorZeroThenWait(14, 10);
    actor->facing = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    *flags &= 1;
}

void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
    struct FieldActor *actor = Object_GetById(15);
    s16 saved = actor->facing;

    actor->timer |= 2;
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuEveryoneGratefulCaptured);
    Engine_ActorSetAnimation(15, 0);
    SceneActor_ApplyActorCueThenWait(15, 0, 2);
    SceneActor_ApplyActorZeroThenWait(15, 10);
    actor->facing = saved;
    Engine_TaskWait(1);
    Engine_EventEnd();
    actor->timer &= 1;
}

void ActorPresentation_RunActorSixteenSceneSetup(void)
{
    struct EventWork *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuThoseMenCapturedTheyreIn); Engine_ActorSetAnimation(16, 1); SceneActor_ApplyActorCueThenWait(16, 0, 2); Engine_EventOpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gEventWork; ++workspace->message; }
    Engine_EventShowMessage(16, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
    struct EventWork *workspace;
    Engine_EventBegin(); Engine_EventSetMessage((s32)MsgKuupuappuOnesWhoCapturedThieves); SceneActor_ApplyActorCueThenWait(18, 0, 2); Engine_EventOpenMessage(18, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) { workspace = gEventWork; ++workspace->message; }
    Engine_EventShowMessage(18, 0); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
    Engine_ActorSetAnimation(19, 0);
    SceneActor_ApplyActorCueThenWait(19, 0, 2);
    Engine_EventShowMessage(19, 0);
    Engine_ActorSetAnimation(19, 1);
    if (PartyInventory_FindOwner(231) != -1 && Engine_GameFlagIsSet(0x858) == 0) {
        u16 *p = &gEventWork->unknown_172;
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
    Object_GetById(14)->timer |= 2;
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
    Object_GetById(14)->timer &= 1;
}

void ActorPresentation_RunActorFifteenScriptBranch(void)
{
    Object_GetById(15)->timer |= 2;
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0)
        Engine_EventSetMessage((s32)MsgKuupuappuSupposeDoesntMatterHowRich);
    else
        Engine_EventSetMessage((s32)MsgKuupuappuMustStrongerThanLookHave);
    SceneActor_RunActorCommandWithFlag91(15);
    Engine_EventEnd();
    Object_GetById(15)->timer &= 1;
}

void ActorPresentation_RunActorSixteenScriptBranch(void)
{
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x855) == 0) Engine_EventSetMessage((s32)MsgKuupuappuEveryoneKnowsThoseThreeAt); else Engine_EventSetMessage((s32)MsgKuupuappuFeelMuchBetter);
    SceneActor_RunActorCommandWithFlag91(16); Engine_EventEnd();
}

void ActorPresentation_RunActorNineteenScriptBranch(void)
{
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
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene8Cells, 54, 13);
    {
        s32 cell = 23;
        s32 row = 12;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->priority_flags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

void ActorPresentation_SetupActorZeroForSceneNineAt296_176(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene9Cells, 49, 10);
    {
        s32 cell = 18;
        s32 row = 10;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->priority_flags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

void FieldScene_SetupScene10At120_144(void)
{ Engine_AudioPlayCue(158); Engine_MapAnimateCells(KuupuappuMura_Scene10Cells, 38, 6); SceneActor_PlaceAndSetSceneDelay(120, 144, 10); }

// ここに共通ソースが入る（ROM の並びのとおり）
#include "scene_cells.c"

void ActorPresentation_SetupActorZeroForSceneTwelveAt72_160(void)
{
    struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

    Engine_AudioPlayCue(158);
    Engine_MapAnimateCells(KuupuappuMura_Scene12Cells, 35, 9);
    {
        s32 cell = 4;
        s32 row = 10;

        Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
    }
    actor->priority_flags &= ~1;
    sprite->priority = 3;
    SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

void FieldScene_SetupScene13At152_264(void) { Engine_AudioPlayCue(123); SceneActor_PlaceAndSetSceneDelay(152, 264, 13); }

void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
    struct FieldActor *record = Object_GetById(actor); int frames;
    Engine_ActorSetSpeed(actor, 0x30000, 0x18000); record->gravity = 0x8000;
    record->bounce = 0; record->velocity_y = field40; Engine_ActorSetSpriteFlags(record, 0);
    Engine_ActorMoveToAndWait(actor, x, z); Engine_ActorSetPosition(actor, x << 16, z << 16);
    for (frames = 60; frames != 0; --frames) { Engine_TaskWait(1); if (*(s16 *)((u8 *)record + 42) == 0) break; }
    Engine_ActorSetSpriteFlags(record, 1); record->gravity = 0x10000;
}

void FieldScene_RunActor23SequenceOnceByFlag867(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Event_PlayCue(100);
    Engine_EventWait(40);
    if (GameFlag_IsSet(0x867) == 0) {
        Actor_SetAttachedEffect(23, 0x102);
        Engine_ActorJump(23, 4, 0);
        Engine_EventWait(12);
        Engine_ActorJump(23, 4, 0);
        Engine_EventWait(20);
        ActorPresentation_MoveActorToPositionAndWait(23, 0x188, 104, 0x70000);
        Engine_EventWait(20);
        Actor_WalkToAndWait(23, 0x198, 104);
        Actor_WalkToAndWait(23, 0x198, 120);
        GameFlag_Set(0x867);
    }
    Engine_EventEnd();
}

void FieldScene_RunActor19MotionSequence(void)
{
    u32 i;
    s32 record;

    PartyInventory_Discard(231);
    Engine_EventBegin();
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(19, 2);
    Actor_SetSpeed(19, 0xcccc, 0x6666);
    Actor_WalkToAndWait(19, 216, 0x198);
    Engine_EventWait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Engine_ActorJump(19, 6, 0);
    Engine_EventWait(30);
    Actor_WalkToAndWait(19, 216, 0x188);
    Engine_EventWait(10);
    Actor_FaceDirection(19, 0x4000, 20);
    GameFlag_Set(0x858);
    Engine_EventEnd();
}

s32 KuupuappuMura_RestoreEntryState(void)
{
    u32 i;
    s32 rec7;
    struct FieldActor *record;
    s32 v7;
    s32 zero;

    if (Engine_GameFlagIsSet(0x87a) != 0) {
        Engine_EventRequestExit(14);
    }
    if (Engine_GameFlagIsSet(0x200) != 0) {
        Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
    }
    OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
    v7 = 1;
    zero = 0;
    record = Object_GetById(14);
    record->timer = v7;
    record->update = (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
    record = Object_GetById(15);
    record->timer = zero;
    record->update = (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
    if (Engine_GameFlagIsSet(0x858) != 0) {
        Actor_SetPosition(19, 0xd80000, 0x1880000);
    }
    rec7 = Engine_GameFlagIsSet(0x853);
    if (Engine_GameFlagIsSet(0x855) == 0) {
        if ((v7 & rec7) != 0) {
            record = Object_GetById(21);
            record->update = (void (*)(union FieldObject *))ActorPresentation_UpdateEntityFromLeader;
        }
    }
    if (gGameState.entrance <= 2) {
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Engine_GameFlagClear(0x867);
            if (Engine_GameFlagIsSet(0x855) == 0) {
                if (Engine_GameFlagIsSet(0x856) != 0) {
                    Engine_EventBegin();
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetPosition(2, record->x.fixed, record->z.fixed);
                    }
                    if (gGameState.entrance == 1) {
                        Actor_SetPosition(2, 0x1900000, 0x1c00000);
                    } else {
                        Actor_SetPosition(2, 0xe00000, 0xa20000);
                    }
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Actor_SetSpeed(2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Event_PrepareObjectAndApplyValue(2, 0);
                    Engine_EventEnd();
                }
            }
        }
    }
    if (Engine_GameFlagIsSet(0x867) != 0) {
        Actor_SetPosition(23, 0x1980000, 0x780000);
    }
    if (gGameState.entrance == 11) {
        if (Engine_GameFlagIsSet(0x855) == 0) {
            if (Engine_GameFlagIsSet(0x856) != 0) {
                if (Engine_GameFlagIsSet(2) == 0) {
                    Engine_EventBegin();
                    Actor_SetPosition(2, 0x280000, 0x1360000);
                    Engine_ActorFaceEachOther(2, 0, 0);
                    Engine_EventOpenScreen();
                    Engine_EventWaitForScreen();
                    Engine_EventWait(30);
                    Engine_ActorRunRepeatedMotion(2, 2);
                    Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
                    Engine_EventShowMessageAndWait(2, 0, 20);
                    Engine_ActorSetAnimationAndWait(0, 3);
                    Actor_SetSpeed(2, 0xcccc, 0x6666);
                    Engine_ActorSetAnimation(2, 2);
                    record = Object_GetById(0);
                    if (record != 0) {
                        Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
                    }
                    Engine_ActorWaitForMove(2);
                    Engine_ActorSetPosition(2, 0, 0);
                    Event_PrepareObjectAndApplyValue(2, 0);
                    Engine_EventEnd();
                }
            }
        }
        Engine_GameFlagClear(0x12f);
    } else {
        if (gGameState.entrance == 13) {
            if (Engine_GameFlagIsSet(0x855) != 0) {
                Engine_ActorSetPosition(20, 0, 0);
            }
        }
    }
    return 0;
}

s32 SceneActor_GetPositionDistance(s32 *a, s32 *b)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 dxsq = dx *dx;
    s32 dysq = dy *dy;
    s32 dzsq = dz *dz;

    return Iwram_Sqrt(dxsq + dysq + dzsq);
}

s32 SceneActor_CheckFacingAndRange(struct FieldActor *actor, struct FieldActor *target, s32 range, s32 force)
{
    s32 result = 0;
    s32 *target_pos = &target->x.fixed;
    s32 *actor_pos = &actor->x.fixed;
    if (SceneActor_GetPositionDistance(target_pos, actor_pos) < range || force != 0) {
        u32 angle = (u16)ArcTan2(target->z.fixed - actor->z.fixed,
                                       *target_pos - *actor_pos);
        u32 left = (angle - 0x1000) & 0xf000;
        u32 right = (angle + 0x1000) & 0xf000;
        u32 forward = angle & 0xf000;
        u32 facing = actor->facing & 0xf000;
        if (forward == facing || right == facing || left == facing || force != 0) {
            actor->paused = 1;
            Object_SetMode(actor, 1);
            result = 1;
        }
    } else {
        actor->paused = 0;
        Object_SetMode(actor, 2);
    }
    return result;
}

void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
    Engine_EventShowMessage(actor, 0);
    Engine_EventWait(delay);
}

void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
    Engine_ActorFaceEachOther(actor, cue, 0);
    Engine_EventWait(delay);
}

void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
    s32 horizontal_rate;
    s32 vertical_rate;

    effect->x += effect->horizontal_rate;
    effect->shadow_x = effect->x;

    if (effect->mode != 0) {
        effect->y += effect->vertical_rate;
        effect->shadow_y = effect->y;
    } else {
        effect->z += effect->vertical_rate;
        effect->shadow_z = effect->z;
        effect->y += 1024;
        effect->shadow_y = effect->y;
    }

    horizontal_rate = effect->horizontal_rate;
    effect->horizontal_rate = horizontal_rate - Math_Divide(horizontal_rate, 28);
    vertical_rate = effect->vertical_rate;
    effect->vertical_rate = vertical_rate - Math_Divide(vertical_rate, 28);
}

void KuupuappuMura_SpawnDriftingEffect(s32 flags)
{
    union DriftingObject *leader;
    union DriftingObject *leaf;
    struct FieldSprite *sprite;
    s32 x;
    s32 z;
    u8 zero;

    leader = (union DriftingObject *)Object_GetById(19);
    if (leader == NULL)
        return;
    x = Engine_RandomNext();
    z = Engine_RandomNext();
    x <<= 3;
    x = (u32)x >> 16;
    z <<= 3;
    x -= 4;
    z = (u32)z >> 16;
    z -= 4;
    x <<= 16;
    z <<= 16;
    {
        s32 base_x = leader->motion.x;
        s32 base_z = leader->motion.z;
        s32 base_y;

        x += base_x;
        base_y = leader->motion.y;
        z += base_z;
        leaf = (union DriftingObject *)Engine_ObjectCreate(0xac, x, base_y, z);
    }
    if (leaf == NULL)
        return;
    sprite = leaf->actor.sprite;
    switch (Engine_RandomNext() & 1) {
    case 1:
        Object_SetMode(&leaf->actor, 3);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptA);
        break;
    default:
        Object_SetMode(&leaf->actor, 2);
        Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptB);
        break;
    }
    {
        u8 *motion_flags = &leaf->actor.motion_flags;

        zero = 0;
        *motion_flags = zero;
    }
    if (flags & 2) {
        s32 cnt;
        s32 bias;

        cnt = (u32)Engine_RandomNext() % 10 + 5;
        /* FAKEMATCH: reuse the coordinate local for the direction mask. */
        x = 1;
        flags &= x;
        bias = flags ^ x;
        bias <<= 2;
        cnt += bias;
        leaf->motion.vertical_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = (u32)Engine_RandomNext() % 15 - 7;
        leaf->motion.horizontal_rate = 0x1999 * cnt;
        leaf->motion.mode = zero;
    } else {
        s32 cnt;

        cnt = (u32)Engine_RandomNext() % 10 + 8;
        leaf->motion.horizontal_rate = (0x3332 * flags - 0x1999) * cnt;
        cnt = (u32)Engine_RandomNext() % 14 + 1;
        leaf->motion.vertical_rate = 0x1999 * cnt;
        leaf->motion.mode = 1;
    }
    {
        void (*update)(union FieldObject *) =
            (void (*)(union FieldObject *))SceneEffect_UpdateMotionWithDamping;
        u8 *sprite_flags;

        /* FAKEMATCH: preserve the sprite-flags address scheduling boundary. */
        do {
            sprite_flags = &sprite->flags;
        } while (0);
        leaf->actor.update = update;
        *sprite_flags = 0;
    }
    sprite->priority = leader->actor.sprite->priority;
}

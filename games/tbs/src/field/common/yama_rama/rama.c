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
#include "scene_ids.h"
#include "call.h"
#include "text/msg_ids.h"

s32 ArcTan2(s32, s32);





void Object_RefreshSelectorById();
void BattleFx_PlayQueuedSound(void);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    void Engine_ActorSetPosition(s32, s32, s32);

    Engine_ActorSetPosition(actor, x, y);
}

static __inline__ void Scene_AdvanceStep(s32 amount)
{
    gEventWork->message += amount;
}


void BattleFx_SetQueuedSoundAndPlay(s32 value);

s32 EventScript_PrepareActorRenderFlags(struct FieldActor *actor)
{
    actor->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    /* FAKEMATCH: needed to match: packed byte accesses avoid caching the sprite, preserving the 34-byte callback. */
    ((u8 *)actor->sprite)[9] |= 0xc;
    ((u8 *)actor->sprite)[21] |= 0xc;
    return 0;
}

s32 OverlayObject_SetFacingTowardObject10(void *self)
{
    struct FieldActor *actor = self;
    struct FieldActor *obj;

    obj = Object_GetById(0xA);
    actor->facing = ArcTan2(obj->z.fixed - actor->z.fixed,
        obj->x.fixed - actor->x.fixed);
    return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEntrances;
    }
    return YamaRama_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return YamaRama_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TemplePlacements;
    }
    return YamaRama_Placements;
}

void SceneDialogue_RunMessage1958Step(void)
{

    struct EventWork *work;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaYoungWarriorsDoComeFrom);
    Engine_EventOpenMessage(10, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_EventWait(20);
        Engine_EventShowMessage(10, 0);
    } else {
        work = gEventWork;
        work->message += 1;
        Engine_EventAskYesNo(10, 0);
    }

    Engine_EventEnd();
}

void SceneDialogue_RunActor11Message195d(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDoKnowMeditation);
    Engine_EventAskYesNo(11, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor13Message1961(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDidKnowMasterHamaGreatest);
    Engine_EventAskYesNo(13, 0);
    Engine_EventEnd();
}

void FieldScene_RunPrimaryScript(void)
{
    Event_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    Object_GetById(0)->motion_flags = 0;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(2);
}

void FieldScene_RunScene3a2SequenceA(void)
{
    void Event_ShowMessageAndWait();

    Engine_EventBegin();
    Actor_SetPosition(8, 0x880000, 0xa80000);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x900000, 0xc80000);
    Actor_SetPosition(GERALD, 0xa00000, 0xc00000);
    Actor_SetPosition(IWAN, 0x800000, 0xc80000);
    Actor_SetPosition(MEARI, 0x700000, 0xc00000);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xe000, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgYamaAdeptsLetMeThankAgain);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(IWAN, 0x102, 60);
    Engine_EventWait(120);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(GERALD, 0x6000, 20);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Actor_FaceDirection(MEARI, 0, 20);
        Actor_ShowEmote(MEARI, 0x101, 60);
        Event_ShowMessageAndWait(MEARI, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(IWAN, 0xe000, 0);
        Engine_ActorStartRepeatedMotion(GERALD, 2);
        Actor_SetAttachedEffect(GERALD, 0x102);
        Engine_EventWait(60);
        Actor_FaceDirection(GERALD, 0x8000, 20);
        Event_ShowMessageAndWait(GERALD, 0, 20);
        gEventWork->message += 2;
    } else {
        gEventWork->message += 2;
        Engine_EventWait(20);
        Actor_FaceDirection(MEARI, 0, 20);
        Engine_ActorSetAnimationAndWait(MEARI, 3);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(MEARI, 0, 20);
        Actor_ShowEmote(GERALD, 0x102, 60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
        Actor_FaceDirection(IWAN, 0xe000, 0);
        Event_ShowMessageAndWait(GERALD, 0, 20);
    }
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 0);
    Actor_FaceDirection(IWAN, 0xc000, 0);
    Actor_FaceDirection(MEARI, 0xe000, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventWait(30);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0xc000, 30);
    Event_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCells, 67, 6);
    Actor_SetSpeed(8, 0xcccc, 0x6666);
    Actor_WalkToAndWait(8, 136, 136);
    Actor_SetPosition(8, 0, 0);
    Event_PlayCue(188);
    Map_AnimateCells(YamaRama_BoulderCellsBack, 67, 6);
    Engine_EventWait(60);
    BattleFx_PlayQueuedSound();
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(GERALD, 0x6000, 20);
    Event_ShowMessageAndWait(GERALD, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(MEARI, 0x2000, 20);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    Event_AskYesNo(GERALD, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_FaceDirection(IWAN, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Actor_ShowEmote(GERALD, 0x102, 60);
    Event_ShowMessageAndWait(GERALD, 0, 20);
    Actor_FaceDirection(IWAN, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
    Actor_ShowEmote(MEARI, 0x102, 60);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_FaceDirection(IWAN, 0xa000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(IWAN, 0x8000, 0x4000);
    Actor_WalkToAndWait(IWAN, 128, 184);
    Actor_FaceDirection(IWAN, 0x4000, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
    Actor_FaceDirection(GERALD, 0xa000, 0);
    Actor_FaceDirection(MEARI, 0xe000, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(GERALD, 0x8000, 0x4000);
    Actor_SetSpeed(MEARI, 0x8000, 0x4000);
    Actor_WalkTo(GERALD, 144, 200);
    Actor_WalkTo(IWAN, 144, 200);
    Actor_WalkTo(MEARI, 144, 200);
    Engine_ActorWaitForMove(GERALD);
    Actor_SetPosition(GERALD, 0, 0);
    Engine_ActorWaitForMove(IWAN);
    Actor_SetPosition(IWAN, 0, 0);
    Engine_ActorWaitForMove(MEARI);
    Actor_SetPosition(MEARI, 0, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunLine1956(void)
{
    void Engine_EventBegin(void);

    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgYamaHeWhoHasPowerSee, 1);
    Engine_EventEnd();
}

void ConfigureAndPlaceActorFourteen(void)
{
    void Engine_ActorSetPosition(s32, s32, s32);

    s32 a = 21, b = 9;
    Engine_MapCopyCellAttributes(85, 9, 1, 1, a, b);
    Engine_MapObjectSetPosition(100, 0, 0);
    PlaceActor(14, 0x01580000, 0x00980000);
}

void YamaRama_ClearActorPlacement(void)
{
    u32 i;
    s32 record;

    Map_CopyCellAttributes(21, 73, 1, 1, 21, 9);
    MapObject_SetPosition(100, -1, -1);
    Actor_SetPosition(14, 0, 0);
}

void SceneDialogue_RunActorFifteenByLeaderHeading(void)
{

    u32 heading = Object_GetById(0)->facing;

    Engine_EventBegin();
    if (heading - 0xA001 <= 0x3FFE) {
        Engine_SanctumOpen(15);
    } else {
        Engine_EventSetMessage((s32)MsgYamaAmTravelingAroundWorldSpread);
        Engine_EventShowMessage(15, 0);
    }
    Engine_EventEnd();
}

void Scene_RunEventTransition(void)
{
    struct FieldActor *record;
    s32 none;

    if (GameFlag_IsSet(0x89a) == 0) {
    } else {
        Engine_EventBegin();
        Actor_SetPosition(10, 0x2180000, 0xd80000);
        Engine_EventSetMessage((s32)MsgYamaYahhSilkRoadBouldersBlock);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_EventWait(20);
        record = Object_GetById(0);
        record->update = (void (*)(union FieldObject *))OverlayObject_SetFacingTowardObject10;
        record = Object_GetById(0);
        if ((record->z.fixed >> 20) == 13) {
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1b8, 200);
        }
        Actor_SetSpeed(10, 0x20000, 0x10000);
        Engine_ActorSetSpritePriority(10, 2);
        Actor_WalkToAndWait(10, 0x198, 216);
        {
            struct FieldActor *record = Object_GetById(10);

            record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
        }
        Engine_EventWait(10);
        Actor_FaceDirection(10, 0x8000, 20);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorStartRepeatedMotion(10, 2);
        Actor_SetAttachedEffect(10, 0x102);
        Engine_EventWait(60);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_ActorEnableActionCallback(10, YamaRama_HsuAction);
        Camera_MoveTo(0x1280000, -1, 0x1580000, 1);
        GameFlag_Set(0x8b0);
        Object_RefreshSelectorById(10);
        Engine_CameraWaitForMove();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, YamaRama_LeaderAction);
        Object_RefreshSelectorById(0);
        Engine_EventWait(10);
        none = 0;
        record = Object_GetById(0);
        record->update = (void (*)(union FieldObject *))none;
        Engine_EventWait(30);
        Engine_ActorRunRepeatedMotion(10, 2);
        Engine_EventWait(20);
        Actor_FaceDirection(10, 0x5000, 120);
        Actor_ShowEmote(10, 0x105, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
        Engine_ActorSetAnimationAndWait(10, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(10, 0, 20);
        Engine_EventEnd();
    }
}

void Scene_RunActorCue(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgYamaDoDoWarriorShouldReturn);
    Actor_ShowEmote(10, 0x105, 60);
    Event_OpenMessage(10, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Scene_AdvanceStep(1);
    }
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(10, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_EventEnd();
}

void Scene_RunActorExchange(void)
{
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
    Engine_CameraMoveToActor(9, 1);
    Engine_CameraWaitForMove();
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage((s32)MsgYamaHsuOkay);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(9, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(9, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(60);
    Actor_ShowEmote(8, 0x102, 60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorStartRepeatedMotion(10, 2);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_EventWait(60);
    Actor_FaceDirection(10, 0xb000, 20);
    Engine_ActorSetAnimation(9, 5);
    Engine_EventEnd();
    GameFlag_Set(0x8b1);
}

void Scene_RunActorSequence(void)
{
    void Engine_EventBegin();

    s32 mask;

    Engine_EventBegin();
    Actor_SetAttachedEffect(8, 0x102);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventWait(60);
    Engine_EventSetMessage((s32)MsgYamaRobinDidLiftBoulder);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_SetAttachedEffect(10, 0x102);
    Engine_ActorJump(10, 4, 0);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Actor_WalkTo(8, 178, 0x114);
    Actor_WalkToAndWait(10, 172, 0x11c);
    Engine_ActorWaitForMove(8);
    Actor_FaceDirection(8, 0x5000, 0);
    Actor_FaceDirection(10, 0xb000, 0);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(20);
    mask = 254;
    Event_ShowMessageAndWait(8, 0, 20);
    Object_GetById(8)->steering_flags &= mask;
    Object_GetById(10)->steering_flags &= mask;
    Actor_SetSpeed(8, 0x3333, 0x1999);
    Actor_SetSpeed(10, 0x3333, 0x1999);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Event_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Event_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Engine_ActorWaitForMove(10);
    Engine_ActorStop(9);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(10, 1);
    Engine_EventWait(50);
    Engine_ActorJump(10, 2, 0);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_AskYesNo(8, 0);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Event_PlayCue(125);
    Actor_SetDestinationOffset(8, 2, 0);
    Actor_SetDestinationOffset(9, 2, 0);
    Actor_SetDestinationOffset(10, 2, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(30);
    Engine_ActorSetAnimation(8, 5);
    Engine_ActorSetAnimation(10, 6);
    Engine_EventWait(20);
    Event_PlayCue(125);
    Actor_SetDestinationOffset(8, 4, 0);
    Actor_SetDestinationOffset(9, 4, 0);
    Actor_SetDestinationOffset(10, 4, 0);
    Engine_ActorWaitForMove(10);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(10, 1);
    Engine_ActorJump(10, 2, 0);
    Engine_EventWait(20);
    Actor_FaceDirection(10, 0xd000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 30);
    Event_ShowMessageAndWait(8, 0, 20);
    {
        struct FieldActor *record = Object_GetById(10);

        record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
    }
    Actor_SetSpeed(10, 0xcccc, 0x6666);
    Actor_WalkToAndWait(10, 168, 0x128);
    Actor_FaceDirection(10, 0xd000, 20);
    Engine_ActorSetAnimation(10, 5);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(10, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    Engine_EventEnd();
    GameFlag_Set(0x8b2);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventRequestExit(6);
}

void FieldScene_RunScriptedSteps0And1A12(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_MessageShowCentered((s32)MsgYamaNorthAltinMineWestLama, 1);
    Engine_EventEnd();
}

void FieldScene_RunPairedLayoutStepsThenSetOne(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Engine_MapCopyCellsTo(5, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Engine_MapCopyCellAttributes(5, 28, 1, 2, fifth, sixth);
    }
    Engine_EventWait(1);
}

void SceneState_RunRect6x28Step(void)
{
    {
        s32 fifth = 1;
        s32 sixth = 2;

        Engine_MapCopyCellsTo(6, 28, 5, 13, fifth, sixth);
    }
    {
        s32 fifth = 5;
        s32 sixth = 13;

        Engine_MapCopyCellAttributes(6, 28, 1, 2, fifth, sixth);
    }
    Engine_EventWait(1);
}

s32 SceneActor_SetFlagBitByRankAgainstActorZero(struct FieldActor *actor)
{
    if (Object_GetById(0)->y.fixed > actor->y.fixed) {
        actor->priority_flags |= 2;
    } else {
        actor->priority_flags &= 0xFD;
    }
}

void SceneActor_UpdateActorFourteenByDepth(void)
{
    struct FieldActor *current = (void *)Object_GetById(0);
    struct FieldActor *other = (void *)Object_GetById(14);

    if (current->z.fixed <= other->z.fixed) {
        Engine_ActorSetSpritePriority(14, 1);
    }
}

void ActorPresentation_PrepareActorFourteenWithCallback(void)
{
    u8 zero;

    zero = 0;
    Engine_EventBegin();

    Object_GetById(14)->priority_flags &= 0xfd;
    Object_GetById(14)->collision_flags &= 0xfd;
    Object_GetById(14)->motion_flags = zero;
    Object_GetById(14)->update = (void (*)(union FieldObject *))SceneActor_SetFlagBitByRankAgainstActorZero;

    Engine_MapCopyCellAttributes(55, 16, 1, 1, 56, 18);
    Engine_MapCopyCellAttributes(55, 16, 1, 1, 20, 18);

    Engine_TaskWait(1);
    Engine_GameFlagSet(512);
    Engine_ActorSetSpritePriority(14, 2);
    Engine_EventEnd();
}

void YamaRama_DisableActorCollision(void)
{
    Engine_EventBegin();
    {
        s32 fifth = 21;
        s32 sixth = 11;

        Engine_MapCopyCellAttributes(14, 6, 1, 2, fifth, sixth);
    }
    Object_GetById(15)->collision_flags = 254;
    Engine_GameFlagSet(0x201);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        return YamaRama_TempleEvents;
    }
    return YamaRama_Events;
}

s32 YamaRama_ApplyEntryState(void)
{
    s32 entrance;

    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    if (gGameState.scene == (s32)&SceneId_YamaRama2) {
        BattleFx_SetQueuedSoundAndPlay(169);
        Engine_ActorSetAnimation(11, 5);
        Engine_ActorSetAnimation(12, 5);
        Engine_ActorSetAnimation(14, 2);
        Map_CopyCellAttributes(21, 9, 1, 1, 21, 73);
        ConfigureAndPlaceActorFourteen();
        if (Engine_GameFlagIsSet(0x8b2)) {
            Actor_SetPosition(13, 0x880000, 0x1000000);
            Engine_ActorFaceDirection(13, 0, 0);
        }
        entrance = gGameState.entrance;
        if (entrance == 2) {
            Engine_GameFlagClear(0x12f);
        } else if (entrance == 3 && !Engine_GameFlagIsSet(0x109)) {
            FieldScene_RunScene3a2SequenceA();
        }
    } else if (gGameState.scene == (s32)&SceneId_YamaRama1) {
        Engine_ActorSetSpriteFlags((struct FieldActor *)Object_GetById(14), 0);
        Object_GetById(14)->priority_flags |= 2;
        if (Engine_GameFlagIsSet(0x200)) {
            Engine_ActorSetAnimation(14, 5);
            ActorPresentation_PrepareActorFourteenWithCallback();
        }
        if (Engine_GameFlagIsSet(0x201)) {
            Engine_ActorSetAnimation(15, 4);
            YamaRama_DisableActorCollision();
        }
        if (gGameState.entrance == 4 || gGameState.entrance == 5) {
            Engine_GameFlagClear(0x12f);
        }
        if (!Engine_GameFlagIsSet(0x89a) && !Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (!GameFlag_IsSet(0x8b2) && Engine_GameFlagIsSet(0x895) && gGameState.entrance == 2) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_GameFlagSet(0x8b2);
            Engine_GameFlagSet(0x8b3);
            Engine_ActorSetPosition(10, 0, 0);
        }
        if (GameFlag_IsSet(0x8b2)) {
            Engine_MapCopyCellsTo(54, 21, 53, 21, 1, 2);
            Engine_MapCopyCellAttributes(18, 20, 1, 3, 17, 21);
            Engine_MapCopyCellsTo(44, 18, 43, 17, 1, 1);
            Engine_MapCopyCellAttributes(8, 17, 1, 1, 7, 17);
        }
        if (Engine_GameFlagIsSet(0x895) && !Engine_GameFlagIsSet(0x8b2)) {
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Actor_SetPosition(8, 0xc00000, 0x1080000);
            Actor_SetPosition(9, 0xa40000, 0x1180000);
            Actor_SetPosition(10, 0xb80000, 0x1300000);
            Actor_FaceDirection(8, 0x5000, 0);
            Actor_FaceDirection(10, 0xb000, 0);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            Object_GetById(9)->scale_x = -0x10000;
        }
        if (!GameFlag_IsSet(0x8b2)) {
            Actor_SetPosition(9, 0xa40000, 0x1180000);
            Engine_ActorEnableActionCallback(9, YamaRama_ActorNineAction);
            Object_GetById(9)->scale_x = -0x10000;
        }
        if (gGameState.entrance == 5 && !Engine_GameFlagIsSet(0x8b1) && !Engine_GameFlagIsSet(0x109) && !Engine_GameFlagIsSet(0x8b2)) {
            Scene_RunActorExchange();
        }
    }
    return 0;
}

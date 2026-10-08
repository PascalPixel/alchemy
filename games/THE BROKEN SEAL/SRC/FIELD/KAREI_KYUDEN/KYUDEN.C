#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "text/MSG_IDS.H"

#define FieldScene_PrepareActors ((void (*)(u8*))FieldScene_PrepareActors)


enum PartyEventsMessage {
    MSG_WHEN_HEARD_WERE_BACK_IVAN = 0x1b21,
    MSG_LORD_HAMMET_WILL_RELEASED_SOON = 0x1b83,
    MSG_HAS_LEGACY_LORD_HAMMETS_SILK = 0x1b88,
    MSG_ROBIN_SNEAKED_INTO_LUNPA_THATS = 0x1b91,
    MSG_ITS_IVAN_HIS_COMPANIONS_PERFECT = 0x2588
};

void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void SceneChannel_ConfigureUniformAndHandoff(s32);
void SceneChannel_ConfigureUniformAndHandoff();
s32 Object_SetActionCallbackAndRefreshById();

static __inline__ void Scene_AdvanceStep(s32 amount)
{

    gEventWork->message += amount;
}

extern u8 Placement_Messages[];






const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiKyuden) {
        return gKareiKyudenEntrances;
    }
    return gKareiKyudenEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_KareiKyuden) {
        FieldScene_PrepareActors(gKareiKyudenPlacements);
        return (const struct ScenePlacement *)gKareiKyudenPlacements;
    }
    return gKareiKyudenPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiKyuden) {
        return gKareiKyudenEvents;
    }
    return gKareiKyudenEventsOther;
}

void SceneDialogue_RunActor13Message1b83(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiLordHammetWillReleasedSoon);
    Engine_EventAskYesNo(13, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor16Message1b88(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgKareiHasLegacyLordHammetsSilk);
    Engine_EventAskYesNo(16, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorEightTurnDialogue(void)
{
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_ActorShowEmote(8, 0x100, 0x3C);
    Engine_EventSetMessage((s32)MsgKareiRobinSneakedIntoLunpaThats);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventShowMessageAndWait(8, 0, 0xA);
    Engine_GameFlagSet(0x913);
    Engine_EventEnd();
}

void KareiKyuden_LeaveThroughDoor(void)
{
    void Map_ClearLayerEntryFlag();

    u32 i;
    s32 record;
    struct EventWork *p5;

    p5 = gEventWork;
    Engine_EventBegin();
    Engine_EventWait(10);
    if (p5->touched_trigger == 4) {
        Audio_PlayCue(188);
    } else {
        Audio_PlayCue(158);
    }
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Engine_EventWait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    if (p5->touched_trigger == 4) {
        Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -16);
    } else {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 3, -16);
    }
    Engine_EventWait(16);
    Engine_EventRequestExit(p5->touched_trigger);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    if (gGameState.scene == (s32)(u32)&SceneId_KareiKyuden) {
        FieldScene_DispatchSceneByIndex();
    }
    return 0;
}

void FieldScene_DispatchSceneByIndex(void)
{
    struct FieldActor *rec;
    s32 h;
    s32 x1 = 0x038a0000;
    s32 z1 = 0x01a60000;

    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);

    switch (gGameState.entrance) {
    case 9:
        if (Engine_GameFlagIsSet(0x941) != 0) {
            rec = Object_GetById(8);
            h = 0x1000;
            rec->facing = h;

            if (Engine_GameFlagIsSet(0x914) == 0) {
                Scene_RunPartySequence();
            }
        } else {
            Engine_ActorSetPosition(9, 0, 0);
            if (Engine_GameFlagIsSet(0x321) != 0) {
                Engine_ActorSetPosition(8, x1, z1);
                rec = Object_GetById(8);
                h = 0xd000;
                rec->facing = h;
            }
        }
        break;

    case 10:
    case 11:
        if (Engine_GameFlagIsSet(0x915) != 0) {
            s32 a5 = 4;
            s32 a6 = 3;
            Engine_MapCopyCellsTo(58, 70, 54, 70, a5, a6);
            {
                s32 b5 = 55;
                s32 b6 = 8;
                Engine_MapCopyCellAttributes(55, 9, 2, 1, b5, b6);
            }
            Engine_MapRedraw();
            Engine_TaskWait(1);
        }
        break;

    case 20:
        Engine_ActorSetPosition(9, 0, 0);
        if (Engine_GameFlagIsSet(0x109) == 0) {
            RunEventScript01();
        }
        break;

    default:
        break;
    }
}

void RunEventScript01(void)
{

    u32 i;
    s32 record;
    struct EventWork *work;
    s32 v5;
    s32 tbl;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    ColorBuffer_ApplySource(0x10002, 0);
    ColorBuffer_ApplyTarget(0x10002, 0);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(1);
    work = gEventWork;
    work->transition_frames = 24;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Actor_SetPosition(8, 0x3580000, 0x1b80000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3580000, 0x1e60000);
    Actor_SetPosition(GERALD, 0x3500000, 0x1f60000);
    Actor_SetPosition(IWAN, 0x3680000, 0x1e60000);
    Actor_SetPosition(MEARI, 0x3700000, 0x1f60000);
    Actor_SetPosition(10, 0x3480000, 0x2060000);
    Actor_SetPosition(11, 0x3780000, 0x2060000);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_EventWait(20);
    Actor_FaceDirection(IWAN, FACING_WEST, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 10);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventWait(60);
    Actor_FaceDirection(8, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Actor_ShowEmote(10, 0x100, 0);
    Engine_ActorStartRepeatedMotion(10, 2);
    Engine_EventWait(60);
    Actor_FaceDirection(10, 0xf000, 20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x3000, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_ActorSetAnimationAndWait(11, 4);
    Engine_ActorSetAnimationAndWait(11, 4);
    ColorBuffer_ApplyTarget(0x10000, 0);
    Engine_ColorBufferInterpolate(40);
    Engine_TaskWait(60);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_EventSetMessage((s32)MsgKareiWhenHeardWereBackIvan);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Engine_ActorSetAnimationAndWait(11, 4);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 10);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Engine_ActorSetAnimation(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x101, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(11, 2);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimationAndWait(10, 3);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 1);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Engine_ActorSetAnimationAndWait(10, 3);
    Actor_FaceDirection(IWAN, FACING_WEST, 10);
    Event_AskYesNo(0x6002, 0);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Actor_ShowEmote(8, 0x105, 60);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Actor_FaceDirection(8, 0x1000, 40);
    Actor_SetSpeed(8, 0x6666, 0x3333);
    Actor_WalkToAndWait(8, 0x37c, 0x1b8);
    Engine_EventWait(40);
    Actor_FaceDirection(8, 0xd000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    ConfigureFourSceneChannelsAndHandoff(60);
    SceneChannel_ConfigureUniformAndHandoff(40);
    Actor_WalkToAndWait(8, 0x358, 0x1b8);
    Engine_EventWait(40);
    Actor_FaceDirection(8, 0x9000, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(10, 0xf000, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 2);
    Actor_FaceDirection(8, 0x5000, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(11, 0x9000, 40);
    Actor_FaceDirection(10, 0xf000, 20);
    Event_ShowMessageAndWait(10, 0, 20);
    Engine_ActorRunRepeatedMotion(11, 2);
    Engine_EventWait(20);
    Actor_FaceDirection(11, 0xb000, 20);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(11, 0x9000, 40);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Actor_FaceDirection(10, 0xd000, 0);
    Actor_FaceDirection(11, 0xb000, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 20);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(8, 2);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 20);
    Event_OpenMessage(GERALD, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    v5 = 1;
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
        v5 = 0;
    }
    Event_ShowMessageAndWait(GERALD, 0, 10);
    if (v5 != 0) {
        gEventWork->message++;
    }
    Actor_FaceDirection(IWAN, FACING_WEST, 10);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Engine_EventWait(60);
    Actor_ShowEmote(8, 0x101, 0);
    Actor_FaceDirection(8, 0x3000, 40);
    Actor_FaceDirection(8, 0x5000, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Actor_FaceDirection(IWAN, FACING_NORTH, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 10);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x107, 60);
    Engine_EventShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 3);
        Event_ShowMessageAndWait(8, 0, 10);
        gEventWork->message++;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(8, 4);
        gEventWork->message++;
        Event_ShowMessageAndWait(8, 0, 10);
    }
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 40);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTH, 10);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    Actor_WalkToAndWait(IWAN, 0x364, 0x1d8);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Actor_ShowEmote(8, 0x105, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_OpenMessage(8, 0);
    Engine_ActorStartRepeatedMotion(GERALD, 1);
    Engine_ActorStartRepeatedMotion(IWAN, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Actor_SetAttachedEffect(8, 0x102);
        Engine_EventWait(40);
        Event_ShowMessageAndWait(8, 0, 10);
        gEventWork->message += 2;
    } else {
        Engine_EventWait(20);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        gEventWork->message++;
        Event_ShowMessageAndWait(GERALD, 0, 10);
        Actor_SetAttachedEffect(8, 0x102);
        Engine_EventWait(40);
        Event_ShowMessageAndWait(8, 0, 10);
    }
    Actor_ShowEmote(MEARI, 0x101, 40);
    Actor_FaceDirection(MEARI, FACING_NORTH, 10);
    Event_ShowMessage(MEARI, 0);
    Actor_FaceDirection(GERALD, FACING_NORTH, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
    Engine_ActorSetAnimation(GERALD, 4);
    Engine_ActorSetAnimation(IWAN, 4);
    Engine_ActorSetAnimationAndWait(MEARI, 4);
    Actor_SetAttachedEffect(8, 0x102);
    Engine_EventWait(60);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Actor_ShowEmote(8, 0x108, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(GERALD, 0x101, 40);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Actor_FaceDirection(8, 0x5000, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(10, 2);
    Engine_ActorSetAnimationAndWait(10, 4);
    Event_ShowMessageAndWait(10, 0, 10);
    Actor_ShowEmote(8, 0x105, 40);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(8, 0x3000, 10);
    Engine_ActorSetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Actor_FaceDirection(8, 0x5000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_ShowEmote(8, 0x100, 40);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_ShowMessageAndWait(8, 0, 10);
    ConfigureFourSceneChannelsAndHandoff(40);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Engine_EventWait(60);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 1);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_ShowEmote(IWAN, 0x105, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 10);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 10);
    Engine_ActorSetAnimation(IWAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Actor_FaceDirection(MEARI, FACING_NORTH, 10);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorSetAnimationAndWait(8, 4);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(10, 2);
    Event_ShowMessageAndWait(10, 0, 10);
    Engine_ActorSetAnimationAndWait(11, 3);
    Event_ShowMessageAndWait(11, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimationAndWait(IWAN, 4);
    Engine_ActorSetAnimation(IWAN, 4);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorSetAnimationAndWait(GERALD, 3);
    Engine_EventShowMessageAndWait(1, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Event_OpenMessage(8, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Engine_ActorSetAnimation(GERALD, 3);
        Engine_ActorSetAnimation(IWAN, 3);
        Engine_ActorSetAnimationAndWait(MEARI, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(GERALD, 0, 10);
        gEventWork->message += 2;
    } else {
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        gEventWork->message++;
        Event_ShowMessageAndWait(GERALD, 0, 10);
        SceneChannel_ConfigureUniformAndHandoff(10);
        Engine_ActorSetAnimation(GERALD, 3);
        Engine_ActorSetAnimation(IWAN, 3);
        Engine_ActorSetAnimationAndWait(MEARI, 3);
        Engine_EventWait(10);
        Event_ShowMessageAndWait(GERALD, 0, 10);
    }
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 40);
    Actor_FaceDirection(8, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(10, 1);
    Engine_EventWait(20);
    Actor_FaceDirection(8, 0x3000, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(10, 3);
    Engine_ActorSetAnimationAndWait(11, 3);
    Actor_SetSpeed(10, 0x10000, 0x8000);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkTo(10, 0x350, 0x21c);
    Actor_WalkToAndWait(11, 0x370, 0x21c);
    Actor_SetPosition(10, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x10000, 0x8000);
    tbl = (s32)KareiKyuden_PartyActions;
    Engine_ActorEnableActionCallback(GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = gEventWork;
    work->transition_frames = 16;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x912);
    Engine_EventEnd();
}

void ConfigureFourSceneChannelsAndHandoff(s32 handoff)
{
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(1, FACING_NORTHEAST, 0);
    Actor_FaceDirection(2, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(3, FACING_NORTHWEST, 0);
    if (handoff != 0) {
        Engine_EventWait(handoff);
    }
}

void SceneChannel_ConfigureUniformAndHandoff(s32 handoff)
{
    Actor_FaceDirection(0, FACING_NORTH, 0);
    Actor_FaceDirection(1, FACING_NORTH, 0);
    Actor_FaceDirection(2, FACING_NORTH, 0);
    Actor_FaceDirection(3, FACING_NORTH, 0);
    if (handoff != 0) {
        Engine_EventWait(handoff);
    }
}

void Scene_RunPartySequence(void)
{

    struct FieldActor *record;
    struct EventWork *work;
    s32 v5;
    s32 tbl;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    Camera_MoveTo(0x3600000, -1, 0x2180000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3600000, 0x2760000);
    gEventWork->start_transition = v5 = 0x100;
    gEventWork->transition_frames = 40;
    Engine_EventOpenScreen();
    Camera_SetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x3600000, -1, 0x1d80000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
    Actor_SetSpeed(MEARI, 0xcccc, 0x6666);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x360, 0x1f2);
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
    }
    record = Object_GetById(0);
    if (record != 0) {
        Actor_SetPosition(MEARI, record->x.fixed, record->z.fixed);
    }
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x358, 0x1e6);
    Actor_WalkTo(GERALD, 0x350, 0x1f6);
    Actor_WalkTo(IWAN, 0x368, 0x1e6);
    Actor_WalkToAndWait(MEARI, 0x370, 0x1f6);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_EventWait(10);
    SceneChannel_ConfigureUniformAndHandoff(10);
    Actor_ShowEmote(9, v5, 20);
    Actor_FaceDirection(9, 0x5000, 20);
    Engine_EventSetMessage((s32)MsgKareiItsIvanHisCompanionsPerfect);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(8, v5, 20);
    Actor_FaceDirection(8, 0x3000, 20);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_ShowEmote(8, 0x107, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GERALD, 0x102);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_EventWait(60);
    Actor_ShowEmote(9, 0x102, 60);
    Actor_FaceDirection(9, 0x7000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x108, 20);
    Event_ShowMessageAndWait(8, 0, 40);
    Engine_ActorRunRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(8, 0x3000, 10);
    Event_OpenMessage(8, 0);
    v5 = 1;
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_ActorSetAnimation(8, 3);
    } else {
        Engine_EventWait(10);
        Scene_AdvanceStep(1);
        Engine_ActorSetAnimation(8, 4);
        v5 = 0;
    }
    Event_ShowMessageAndWait(8, 0, 10);
    if (v5 != 0) {
        Scene_AdvanceStep(1);
    }
    Engine_ActorStartRepeatedMotion(9, 2);
    Actor_SetAttachedEffect(9, 0x102);
    Engine_EventWait(80);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_FaceDirection(8, 0x1000, 10);
    Actor_ShowEmote(8, 0x107, 40);
    Engine_ActorRunRepeatedMotion(IWAN, 3);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Actor_ShowEmote(8, 0x105, 60);
    Actor_FaceDirection(8, 0x3000, 60);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Actor_FaceDirection(9, 0x5000, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(IWAN, FACING_NORTH, 10);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Event_ShowMessageAndWait(0x2002, 0, 20);
    Engine_ActorSetAnimationAndWait(8, 3);
    Event_ShowMessageAndWait(8, 0, 20);
    Actor_FaceDirection(IWAN, FACING_WEST, 20);
    Event_ShowMessageAndWait(0x6002, 0, 10);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 10);
    Actor_ShowEmote(MEARI, 0x105, 40);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorRunRepeatedMotion(IWAN, 2);
    Engine_EventWait(80);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventWait(20);
    Actor_FaceDirection(GERALD, FACING_NORTHEAST, 10);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Event_OpenMessage(GERALD, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Scene_AdvanceStep(1);
    } else {
        Engine_EventWait(20);
        Event_ShowMessageAndWait(GERALD, 0, 10);
    }
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    SceneChannel_ConfigureUniformAndHandoff(20);
    Engine_ActorRunRepeatedMotion(9, 2);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Actor_FaceDirection(9, 0x3000, 10);
    Engine_ActorSetAnimationAndWait(9, 3);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Actor_ShowEmote(IWAN, 0x101, 60);
    Event_ShowMessageAndWait(0x2002, 0, 10);
    Engine_ActorRunRepeatedMotion(9, 1);
    Actor_FaceDirection(9, 0x5000, 10);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    Event_ShowMessageAndWait(GERALD, 0, 10);
    Engine_ActorSetAnimationAndWait(9, 4);
    Event_ShowMessageAndWait(0x2009, 0, 10);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    Event_ShowMessageAndWait(MEARI, 0, 10);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(8, 0, 10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_EventWait(20);
    tbl = (s32)KareiKyuden_PartyActions;
    Engine_ActorEnableActionCallback(GERALD, tbl);
    Engine_ActorEnableActionCallback(2, tbl);
    Object_SetActionCallbackAndRefreshById(3, tbl);
    work = gEventWork;
    work->transition_frames = 16;
    work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
    GameFlag_Set(0x914);
    Engine_EventEnd();
}

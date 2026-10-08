#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "FXBLEND.H"
#include "MAP_SCROLL.H"
#include "HAIDIA_BABI.H"
#include "RAM_BUFFER.H"
#include "FIELD_EVENT.H"
#include "text/MSG_IDS.H"

#define FieldScene_PrepareActors ((s32 (*)(s32))FieldScene_PrepareActors)
#define ObjectDispatch_RegisterChildMetadata ((void (*)(struct FieldActor*,s32))ObjectDispatch_RegisterChildMetadata)



void BattleFx_SetBlock30ValuesMaxZero(void);

void Map_ClearLayerEntryFlag();
void FieldScene_RunPaletteRampSequence();
void FieldScene_RunComplexActorSequence();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();

s32 *HaidiaBabi_GetEntrances(void)
{
    return gHaidiaBabiEntrances;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 HaidiaBabi_SelectExits(void)
{
    if (Engine_GameFlagIsSet(0x834) != 0) {
        return (s32)gHaidiaBabiExits2;
    }
    return (s32)gHaidiaBabiExits;
}

s32 HaidiaBabi_SelectPlacements(void)
{
    u8 *b = (u8 *)&gGameState;
    s32 *tbl;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)gHaidiaBabiPlacements4;
    if (Engine_GameFlagIsSet(0x87a) != 0)
        tbl = gHaidiaBabiPlacements3;
    else if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0)
        tbl = gHaidiaBabiPlacements2;
    else
        tbl = gHaidiaBabiPlacements;
    FieldScene_PrepareActors((s32)tbl);
    return (s32)tbl;
}

void HaidiaBabi_AskAboutKraden(s32 object)
{
    s32 msg = (s32)MsgHaidiaFolksSeemKnow;

    Engine_EventSetMessage(msg);
    Engine_EventOpenMessage(object, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(msg + 1);
    } else {
        Engine_EventSetMessage(msg + 2);
    }
    Engine_EventShowMessage(object, 0);
}

s32 HaidiaBabi_SelectEvents(void)
{
    if (gGameState.entrance == 19) {
        if (Engine_GameFlagIsSet(0x950) != 0) {
            return (s32)gHaidiaBabiEvents6;
        }
        return (s32)gHaidiaBabiEvents5;
    }

    if (Engine_GameFlagIsSet(0x834) != 0) {
        return (s32)gHaidiaBabiEvents4;
    }
    if (Engine_GameFlagIsSet(0x87A) != 0) {
        return (s32)gHaidiaBabiEvents3;
    }
    if (Engine_GameFlagIsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return (s32)gHaidiaBabiEvents2;
    }
    return (s32)gHaidiaBabiEvents;
}

void HaidiaBabi_RunHeyBoyScene(void)
{
    u32 i;
    s32 record;

    Engine_EventBegin();
    Engine_ActorStartRepeatedMotion(16, 2);
    Engine_EventWait(30);
    Engine_EventSetMessage((s32)MsgHaidiaHeyBoy);
    Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, 16, 10);
    Event_ShowMessageAndWait(16, 0, 6);
    Actor_ShowEmote(16, 0x102, 0);
    Engine_ActorStartRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(16, 4);
    Engine_EventWait(20);
    Event_OpenMessage(16, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message++;
    }
    Engine_ActorStartRepeatedMotion(16, 1);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(16, 0, 4);
    Engine_EventEnd();
}

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    struct EventWork *work;

    Engine_EventBegin();
    Engine_ActorFaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
    Engine_EventSetMessage((s32)MsgHaidiaTheMaskedManWasGarcia);
    Engine_EventOpenMessage(0xE, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventShowMessage(0xE, 0);
    } else {
        work = gEventWork;
        work->message = (u16)(work->message + 1);
        Engine_EventAskYesNo(0xE, 0);
    }
    Engine_EventEnd();
}

void HaidiaBabi_CloseScreenAndExit(s32 object)
{
    if (Engine_GameFlagIsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(object);
}

void SceneState_SetValue123Mode1(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(1);
}

void FieldScene_RunStep7BThen2(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(2);
}

void SceneState_SetValue123Mode3(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(3);
}

void FieldScene_RunStep7BThen4(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(4);
}

void FieldScene_RunStep80Then5(void)
{
    Engine_AudioPlayCue(0x80);
    HaidiaBabi_CloseScreenAndExit(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(6);
}

void FieldScene_RunStep80Then7(void)
{
    Engine_AudioPlayCue(0x80);
    HaidiaBabi_CloseScreenAndExit(7);
}

void SceneState_SetValue129Mode8(void)
{
    Engine_AudioPlayCue(0x81);
    HaidiaBabi_CloseScreenAndExit(8);
}

void SceneState_SetValue129Mode9(void)
{
    Engine_AudioPlayCue(0x81);
    HaidiaBabi_CloseScreenAndExit(9);
}

void FieldScene_RunStep7BThen10(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(10);
}

void SceneState_ApplyValues123And11(void)
{
    Engine_AudioPlayCue(0x7B);
    HaidiaBabi_CloseScreenAndExit(11);
}

s32 HaidiaBabi_RestoreEntryState(void)
{
    /* FAKEMATCH: needed to match: the shared cached-cell base keeps the word read before the narrow store. */
    u32 i;
    s32 record;
    struct EventWork **control;

    if (gGameState.entrance == 19) {
        Engine_GameFlagClear(0x12f);
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    } else {
        if (Engine_GameFlagIsSet(0x834) != 0) {
            Engine_ActorSetPosition(11, 0, 0);
            Engine_ActorSetPosition(12, 0, 0);
            Engine_ActorSetPosition(13, 0, 0);
            Engine_ActorSetPosition(14, 0, 0);
            Engine_ActorSetPosition(15, 0, 0);
            Engine_ActorSetPosition(16, 0, 0);
        } else {
            ActorPresentation_SetTwoSceneCells();
        }
        Engine_ActorSetSpritePriority(13, 1);
        if (Engine_GameFlagIsSet(0x87a) != 0) {
            record = (s32)Object_GetById(17);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            if (gGameState.entrance != 6 && gGameState.entrance != 7) {
                goto L_02000550;
            }
            if (Engine_GameFlagIsSet(0x109) != 0) {
                record = Engine_GameFlagIsSet(0x203);
                if (record == 0) {
                    goto L_02000550;
                }
                Map_ClearLayerEntryFlag(12);
                goto L_02000550;
            }
            Map_ClearLayerEntryFlag(11);
            record = (s32)Object_GetById(8);
            Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
            Engine_ActorSetAnimation(8, 10);
        } else {
            if (gGameState.entrance == 21) {
                FieldScene_RunPaletteRampSequence();
            } else {
                if (gGameState.entrance == 20) {
                    Engine_GameFlagSet(0x834);
                    FieldScene_RunComplexActorSequence();
                } else {
                    if (gGameState.entrance == 22) {
                        FieldScene_RunSupplementalSequenceOne();
                    } else {
                        control = &gEventWork;
                        (*control)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
                        if (Engine_GameFlagIsSet(0x834) != 0) {
                            BattleFx_StartTwelveFrameBlend();
                            {
                                struct FieldBlendWork *blend = *(struct FieldBlendWork **)((u8 *)control + 12);
                                u16 *target = (u16 *)&blend->loud;
                                u16 shown = 1;

                                *target = shown;
                            }
                            BattleFx_SetBlock30Values12Zero();
                            Engine_TaskWait(30);
                            Engine_EventOpenScreen();
                            Engine_EventWaitForScreen();
                            BattleFx_SetBlock30Values128One();
                        } else {
                            Engine_MapRedraw();
                            Engine_TaskWait(1);
                        }
                    }
                }
            }
        }
    }
    L_02000550:;
    return 0;
}



struct SceneHalf {
    u16 value;
};

void FieldScene_RunSixStepSequence17e4();
void Map_ClearLayerEntryFlag();
void Map_SetLayerEntryFlag();
void Graphics_EnableObjLayerAndCallbacks();
void ObjectDispatch_StopCallbacksAndHideLayers();
void UiText_ShowCenteredMessage(s32 message, s32 mode, s32 y_offset);
void Object_RefreshSelectorById();
void Ui_SetRenderResultFromObject();
void BattleFx_StartTwelveFrameBlend();
void BattleFx_SetBlock30Values12Zero();
void BattleFx_SetBlock30Values128One();

void FieldScene_RunComplexActorSequence(void)
{
    /* FAKEMATCH: needed to match: a halfword record keeps the actor-flag zero loaded from the pool after the actor lookup. */
    s32 base;
    struct FieldSprite *sprite;
    struct FieldActor *p12;
    struct FieldActor *p89;
    struct MapScrollWork *work;
    struct FieldActor *scene_actor;
    struct EventWork **control;
    struct SceneHalf stopped;
    s32 ground;

    control = (struct EventWork **)Ram_EventWork;
    /* FAKEMATCH: needed to match: stage the root reads before consuming the event record. */
    {
        struct EventWork *event = *control;

        work = *(struct MapScrollWork **)Ram_MapWork;
        scene_actor = event->view_center;
    }
    sprite = Object_GetById(17)->sprite;
    Engine_EventBegin();
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    Engine_ActorSetPosition(14, 0, 0);
    Engine_ActorSetPosition(15, 0, 0);
    Engine_ActorSetPosition(16, 0, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
    Engine_ActorSetAnimation(0, 18);
    ground = 0;
    stopped.value = 0;
    sprite->rotation = 1365;
    p12 = Object_GetById(17);
    p12->motion_flags = stopped.value;
    Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
    Engine_ActorSetPosition(17, 37748736, 42598400);
    Map_ClearLayerEntryFlag(7);
    Engine_ActorSetPosition(8, 34996224, 45088768);
    Graphics_EnableObjLayerAndCallbacks();
    Ui_SetRenderResultFromObject(8);
    base = (s32)MsgHaidiaWake;
    UiText_ShowCenteredMessage(base, 1, 0);
    Engine_EventWait(40);
    MapRender_SetValues(65536, 65536, 65536);
    Ui_SetRenderResultFromObject(8);
    UiText_ShowCenteredMessage(base + 1, 1, 0);
    ObjectDispatch_StopCallbacksAndHideLayers();
    Engine_EventWait(40);
    work->min_x = 0x01480000;
    work->min_y = 0x02580000;
    work->max_x = 0x02700000;
    work->max_y = 0x03300000;
    scene_actor->x.fixed = 0x02340000;
    scene_actor->y.fixed = ground;
    scene_actor->z.fixed = 0x02b30000;
    Engine_MapRedraw();
    Engine_TaskWait(1);
    (*control)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    (*control)->transition_frames = 64;
    BattleFx_StartTwelveFrameBlend();
    (*(struct FieldBlendWork **)((u8 *)control + Ram_SceneMapStateOffset))->loud = 1;
    BattleFx_SetBlock30Values12Zero();
    Engine_TaskWait(30);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    BattleFx_SetBlock30Values128One();
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventSetMessage(base + 2);
    Event_ShowMessageAndWait(36872, 0, 60);
    Engine_ActorRunRepeatedMotion(0, 2);
    Engine_EventWait(40);
    Engine_ActorRunRepeatedMotion(8, 1);
    Engine_EventWait(40);
    Event_ShowMessageAndWait(36872, 0, 20);
    Engine_ActorRunRepeatedMotion(0, 2);
    Map_SetLayerEntryFlag(7);
    Engine_EventWait(20);
    Map_ClearLayerEntryFlag(8);
    Actor_SetSpeed(0, 65536, 32768);
    Engine_ActorSetAnimation(0, 19);
    Actor_MoveToAndWait(0, 557, 679);
    Map_SetLayerEntryFlag(8);
    Map_ClearLayerEntryFlag(9);
    Actor_MoveToAndWait(0, 555, 680);
    Engine_EventWait(30);
    Actor_FaceDirection(8, 53248, 0);
    Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
    Engine_ActorJump(0, 4, 0);
    Actor_WalkToAndWait(0, 543, 674);
    Engine_ActorSetSpritePriority(0, 3);
    Actor_FaceDirection(0, 16384, 40);
    Engine_ActorSetAnimationAndWait(8, 4);
    Engine_EventWait(20);
    Event_ShowMessage(36872, 0);
    FieldScene_RunSixStepSequence17e4();
    Engine_ActorStartRepeatedMotion(8, 2);
    Event_ShowMessageAndWait(36872, 0, 20);
    Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(8, 542, 680);
    Engine_EventWait(1);
    Object_GetById(8)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventWait(10);
    Engine_ActorRunRepeatedMotion(8, 2);
    ObjectDispatch_RegisterChildMetadata(Object_GetById(0), 226);
    Engine_GameFlagSet(33);
    Engine_AudioPlayCue(126);
    Engine_ActorSetChildValue(0, 7);
    Engine_EventWait(10);
    Engine_ActorSetChildValue(0, 0);
    Engine_EventWait(20);
    Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(8, 534, 688);
    Engine_EventWait(1);
    Object_GetById(8)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventWait(20);
    Actor_SetSpeed(8, 98304, 49152);
    Actor_SetSpeed(0, 98304, 49152);
    Engine_CameraFollowActor(8, 1);
    p89 = Object_GetById(0);
    p89->priority_flags |= 0x1;
    Engine_ActorEnableActionCallback(8, gHaidiaBabiSharedAction);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(0, gHaidiaBabiSharedAction);
    Object_RefreshSelectorById(8);
    Actor_WalkToAndWait(8, 419, 661);
    Actor_WalkToAndWait(8, 408, 661);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorSetAnimation(0, 1);
    Actor_FaceDirection(8, 16384, 10);
    Event_OpenMessage(32776, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        (*control)->message++;
    }
    Engine_EventWait(20);
    Event_ShowMessageAndWait(32776, 0, 20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimationAndWait(8, 3);
    Engine_EventWait(20);
    Engine_ActorEnableActionCallback(8, gHaidiaBabiActorExitAction);
    Engine_ActorEnableActionCallback(0, gHaidiaBabiLeaderExitAction);
    Engine_EventWait(20);
    (*control)->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    (*control)->transition_frames = 16;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(20);
}

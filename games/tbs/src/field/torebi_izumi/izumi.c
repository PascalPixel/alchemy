#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "topic.h"
#include "field_scene.h"
#include "types.h"
#include "field_event.h"
#include "call.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "text/msg_ids.h"



void TorebiIzumi_RiseAndFadeIn();
void TorebiIzumi_RunSpringGame();



extern u8 gOamCopyEnabled;
void UiWork_PushValueSlot(s32 value, s32 digits);

static __inline__ void Io_SetBlendControl(s32 value)
{
    REG_BLDCNT = value;
}

static __inline__ void Io_SetBlendAlpha(s32 value)
{
    REG_BLDALPHA = value;
}

u8 *SceneData_GetSceneTableA(void)
{
    return TorebiIzumi_SceneTableA;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetSceneTableB(void)
{
    return TorebiIzumi_SceneTableB;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        return gTorebiIzumiPlacements2;
    }
    return gTorebiIzumiPlacementsOther;
}

void TorebiIzumi_AskForLuckyMedal(s32 object)
{
    s32 question = (s32)MsgTorebiLuckyMedal;
    Engine_EventSetMessage(question);
    Engine_EventOpenMessage(object, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(question + 1);
    } else {
        Engine_EventSetMessage(question + 2);
    }
    Engine_EventShowMessage(object, 0);
}

void TorebiIzumi_AskIfFirstTime(s32 object)
{
    s32 question = (s32)MsgTorebiYerFirstTime;
    Engine_EventSetMessage(question);
    Engine_EventOpenMessage(object, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(question + 1);
    } else {
        Engine_EventSetMessage(question + 2);
    }
    Engine_EventShowMessage(object, 0);
}

void TorebiIzumi_WalkLeaderToSpring(void)
{
    s32 leader = gGameState.selected_actor;

    if (GameFlag_IsSet(0x200) == 0) {
        Engine_GameFlagSet(0x200);
        TorebiIzumi_RiseAndFadeIn();
    }
    Engine_EventBegin();
    Engine_ActorWalkToAndWait(leader, 120, 152);
    ((s32 (*)())Engine_ActorFaceDirection)(leader, 0x4000, 0);
    TorebiIzumi_RunSpringGame();
    Engine_EventEnd();
}

void SceneDialogue_RunMessage0e34(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiLuckyWheelsRulesPullLever);
    Engine_EventOpenMessage(-1, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunMessage0e35(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgTorebiLuckyWheelsPrizesPrizesDetermined);
    Engine_EventOpenMessage(-1, 0);
    Engine_EventEnd();
}

void FieldScene_RunIndexedStep0(void)
{
    TorebiIzumi_OfferLuckyWheels(0);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        return gTorebiIzumiEvents2;
    }
    return gTorebiIzumiEventsOther;
}

s32 TorebiIzumi_OpenScene(void)
{
    s32 diff;
    s8 *list;
    s32 i;
    s32 item;

    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        do { Io_SetBlendControl(0x3f42); } while (0); /* FAKEMATCH: each do/while loads the value before the register address */
        do { Io_SetBlendAlpha(0x80c); } while (0);
        Engine_ActorSetAnimation(24, 2);
        Engine_ActorSetAnimation(25, 2);
        Object_GetById(24)->scale_x = -0x10000;
        Object_GetById(25)->scale_x = -0x10000;
        Object_GetById(24)->priority_flags = 2;
        Object_GetById(25)->priority_flags = 2;
        Engine_EventOpenScreen();
        if (gGameState.entrance == 1) {
            SceneState_InitFourActorRecordsAndInstallTask();
            if (Engine_GameFlagIsSet(0x200)) {
                Io_SetBlendControl(0x3f42);
                Io_SetBlendAlpha(0x1000);
            }
        }
    } else {
        if (Engine_GameFlagIsSet(0x950)) {
            Engine_ActorSetPosition(17, 0, 0);
        }
        gOamCopyEnabled = 1;
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
        if (gGameState.entrance == 10) {
            Engine_ActorSetChildValue(8, 1);
            Engine_ActorSetChildValue(9, 2);
        }
        if (gGameState.entrance == 13 && !Engine_GameFlagIsSet(0x109)) {
            Engine_EventBegin();
            Engine_ActorSetChildValue(8, 1);
            Engine_ActorSetChildValue(9, 2);
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_EventWait(10);
            Engine_ActorWalkToAndWait(0, 120, 112);
            Engine_EventWait(20);
            diff = gGameState.coins - *(s32 *)gSceneState;
            if (diff > 0) {
                if (diff > 19999) {
                    Engine_AudioPlayCue(93);
                } else if (diff > 4999) {
                    Engine_AudioPlayCue(92);
                } else {
                    Engine_AudioPlayCue(91);
                }
                Engine_EventWait(20);
                Engine_EventSetMessage((s32)MsgTorebiYaWonNumber);
                UiWork_PushValueSlot(diff, 5);
                Engine_EventShowMessage(9, 0);
                AudioCommand_WaitForStateByteClear();
            } else if (diff < 0) {
                Engine_EventSetMessage((s32)MsgTorebiYaLostNumber);
                UiWork_PushValueSlot(-diff, 5);
                Engine_EventShowMessage(9, 0);
            }
            Engine_EventEnd();
        }
        if (gGameState.entrance == 12 && !Engine_GameFlagIsSet(0x109)) {
            list = gGameState.won_prizes;
            Engine_EventBegin();
            Engine_EventOpenScreen();
            Engine_EventWaitForScreen();
            Engine_EventWait(10);
            if (list[0] == -1) {
                TorebiIzumi_OfferLuckyWheels(1);
            } else if (list[0] != -2) {
                Engine_EventSetMessage((s32)MsgTorebiCongratulations);
                Engine_EventShowMessage(8, 0);
                if (*list != -1) {
                    for (i = 0; list[i] != -1; i++) {
                        if (i == 0) {
                            Engine_EventSetMessage((s32)MsgTorebiWonItemGo);
                        } else {
                            Engine_EventSetMessage((s32)MsgTorebiWonItemGo2);
                        }
                        item = SceneDialogue_PickTopicVariantId(list[i]);
                        UiWork_PushValueSlot(item, 2);
                        Engine_EventShowMessage(8, 0);
                        Engine_ItemShowFound(item, 3);
                        Engine_PartyGiveItem(item, 0);
                        Engine_EventWait(10);
                        Engine_ActorFaceDirection(0, 0xc000, 0);
                        Engine_EventWait(30);
                    }
                }
                gGameState.won_prizes[0] = -2;
                Engine_EventSetMessage((s32)MsgTorebiComeAgain);
                Engine_EventShowMessage(8, 0);
            }
            Engine_EventEnd();
        }
    }
    return 0;
}

void TorebiIzumi_RevealPrize(s32 item)
{
    Engine_EventBegin();
    Engine_EventWait(30);
    Engine_AudioPlayCue(148);
    Engine_EventWait(100);
    Actor_FaceDirection(0, 0xc000, 0);
    Engine_EventWait(40);

    Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_EventWait(3);
    Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);

    Map_CopyCellsTo(79, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);

    Engine_EventWait(70);
    Engine_AudioPlayCue(126);
    Engine_ItemShowFound(item, 3);
    Engine_PartyGiveItem(item, 0);
    Engine_EventWait(20);

    Map_CopyCellsTo(97, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 29, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(100, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(97, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(94, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(91, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(88, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(85, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(82, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Map_CopyCellsTo(79, 20, 70, 0, 3, 8);
    Engine_AudioPlayCue(154);
    Engine_EventWait(8);
    Engine_EventEnd();
}

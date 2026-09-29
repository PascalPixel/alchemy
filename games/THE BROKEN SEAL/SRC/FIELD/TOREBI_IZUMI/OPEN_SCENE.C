#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
extern u8 MsgTorebiComeAgain[];
extern u8 MsgTorebiCongratulations[];
extern u8 MsgTorebiWonItemGo[];
extern u8 MsgTorebiWonItemGo2[];
extern u8 MsgTorebiYaLostNumber[];
extern u8 MsgTorebiYaWonNumber[];
extern u8 Data_03001d18; /* OAM buffer pending */

void SceneState_InitFourActorRecordsAndInstallTask(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
void AudioCommand_WaitForStateByteClear(void);
void TorebiIzumi_OfferLuckyWheels(s32 mode);
s32 SceneDialogue_PickTopicVariantId(s32 topic);

static __inline__ void Io_SetBlendControl(s32 value)
{
    *(volatile u16 *)0x04000050 = value;
}

static __inline__ void Io_SetBlendAlpha(s32 value)
{
    *(volatile u16 *)0x04000052 = value;
}

/* Opens the Torebi spring scene: stages the blend and actors, reports the coin difference after a game, and hands out each prize item won. */
s32 TorebiIzumi_OpenScene(void)
{
    s32 diff;
    s8 *list;
    s32 i;
    s32 item;

    if (gGameState.scene == (s32)&SceneId_TorebiIzumi2) {
        gEventWork->start_transition = 0x100;
        do { Io_SetBlendControl(0x3f42); } while (0); /* FAKEMATCH: each do/while loads the value before the register address */
        do { Io_SetBlendAlpha(0x80c); } while (0);
        Engine_ActorSetAnimation(24, 2);
        Engine_ActorSetAnimation(25, 2);
        Engine_ActorGet(24)->scale_x = -0x10000;
        Engine_ActorGet(25)->scale_x = -0x10000;
        Engine_ActorGet(24)->priority_flags = 2;
        Engine_ActorGet(25)->priority_flags = 2;
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
        Data_03001d18 = 1;
        gEventWork->start_transition = 0x209;
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

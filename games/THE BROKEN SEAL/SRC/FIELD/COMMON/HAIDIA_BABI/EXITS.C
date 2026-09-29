#include "HAIDIA_BABI.H"

/* The events hook, the villagers' scenes and the house's exits. */

void BattleFx_SetBlock30ValuesMaxZero(void);

s32 HaidiaBabi_SelectEvents(void)
{
    if (gGameState.entrance == 19) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)gHaidiaBabiEvents6;
        }
        return (s32)gHaidiaBabiEvents5;
    }

    if (GameFlag_IsSet(0x834) != 0) {
        return (s32)gHaidiaBabiEvents4;
    }
    if (GameFlag_IsSet(0x87A) != 0) {
        return (s32)gHaidiaBabiEvents3;
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        return (s32)gHaidiaBabiEvents2;
    }
    return (s32)gHaidiaBabiEvents;
}

void HaidiaBabi_RunHeyBoyScene(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Actor_StartRepeatedMotion(16, 2);
    Event_Wait(30);
    Event_SetMessage(MSG_HEY_BOY);
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 16, 10);
    Event_ShowMessageAndWait(16, 0, 6);
    Actor_ShowEmote(16, 0x102, 0);
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Actor_SetAnimationAndWait(16, 4);
    Event_Wait(20);
    Event_OpenMessage(16, 0);
    if (Event_ChooseYesNo(0, 0) == 1) {
        bump_step(1);
    }
    Actor_StartRepeatedMotion(16, 1);
    Event_Wait(20);
    Event_ShowMessageAndWait(16, 0, 4);
    Event_End();
}

void SceneDialogue_RunActorFourteenDialogue11AA(void)
{
    void *work;

    Event_Begin();
    Actor_FaceActor(0xE, ACTOR_PARTY_LEADER, 0xA);
    Event_SetMessage(MSG_THE_MASKED_MAN_WAS_GARCIA);
    Event_OpenMessage(0xE, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Event_ShowMessage(0xE, 0);
    } else {
        work = *(void **)&gEventWork;
        FIELD_AT_OFFSET(work, u16 *, 0x1D8) = (u16)(FIELD_AT_OFFSET(work, u16 *, 0x1D8) + 1);
        Event_AskYesNo(0xE, 0);
    }
    Event_End();
}

void SceneState_SetWork448To521AndRun(s32 object)
{
    if (GameFlag_IsSet(0x834) != 0) {
        BattleFx_SetBlock30ValuesMaxZero();
    }
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_RequestExit(object);
}

void SceneState_SetValue123Mode1(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(1);
}

void FieldScene_RunStep7BThen2(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(2);
}

void SceneState_SetValue123Mode3(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(3);
}

void FieldScene_RunStep7BThen4(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(4);
}

void FieldScene_RunStep80Then5(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(5);
}

void FieldScene_RunStep7BThen6(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(6);
}

void FieldScene_RunStep80Then7(void)
{
    Audio_PlayCue(0x80);
    SceneState_SetWork448To521AndRun(7);
}

void SceneState_SetValue129Mode8(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(8);
}

void SceneState_SetValue129Mode9(void)
{
    Audio_PlayCue(0x81);
    SceneState_SetWork448To521AndRun(9);
}

void FieldScene_RunStep7BThen10(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(10);
}

void SceneState_ApplyValues123And11(void)
{
    Audio_PlayCue(0x7B);
    SceneState_SetWork448To521AndRun(11);
}

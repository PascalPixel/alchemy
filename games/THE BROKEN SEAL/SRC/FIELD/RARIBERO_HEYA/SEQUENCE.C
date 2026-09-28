#include "HEYA.H"

void FieldScene_RunSequenceA(void)
{

    GameFlag_Set(0x9BC);
    Event_Begin();
    Engine_ResetSceneEffectCounter();
    Event_Wait(0xA);
    Camera_MoveTo(0x780000, -1, 0x600000, 1);
    Camera_WaitForMove();
    Event_Wait(0x1E);
    Event_SetMessage(MSG_PLEASE_WAIT_FOR_ME_OUTSIDE);
    Event_ShowMessage(0xC, 0);
    Event_Wait(0xA);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0xC, 0);
    Event_Wait(0x1E);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(0x1E);
    Event_End();
}

void FieldScene_RunThreeCallSequence(void)
{
    void Event_ShowMessage(s32, s32);

    GameFlag_Set(0x9BC);
    Event_SetMessage(MSG_PLEASE_WAIT_FOR_ME_OUTSIDE);
    Event_ShowMessage(0xC, 0);
}

void SceneState_ForwardWord16cAndApply7b(void)
{
    u8 *work = (u8 *)gEventWork;
    s16 *p = (s16 *)(work + 0x16C);

    Event_RequestExit(*p);
    Audio_PlayCue(0x7B);
}

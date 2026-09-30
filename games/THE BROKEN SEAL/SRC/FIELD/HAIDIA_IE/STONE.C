/* The psynergy stone falling on the hut. */
#include "HAIDIA.H"
extern u8 MsgHaidiaAnythingInterestingOnYourTrip[];
extern u8 MsgHaidiaCanIUsePsynergy[];
extern u8 MsgHaidiaIHaveSomePsynergyLeft[];
extern u8 MsgHaidiaTheStoneFellOnThe[];
extern u8 MsgHaidiaYouSawTheWiseOne[];

void SceneDialogue_RunFlagGatedMessageStep(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x87a) != 0) {
        Event_SetMessage((s32)MsgHaidiaAnythingInterestingOnYourTrip);
        Event_OpenMessage(15, 0);
        if (Event_ChooseYesNo(0, 0) == 1) {
            Event_ShowMessage(15, 0);
        } else {
            u8 *p = (u8 *)gEventWork;
            *(u16 *)(p + 472) = *(u16 *)(p + 472) + 1;
            Event_AskYesNo(15, 0);
        }
    } else if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Event_SetMessage((s32)MsgHaidiaYouSawTheWiseOne);
        Event_AskYesNo(11, 0);
    } else {
        Event_SetMessage((s32)MsgHaidiaCanIUsePsynergy);
        Event_AskYesNo(11, 0);
    }
    Event_End();
}

void Scene_StoneFellOnTheHut(void)
{
    Event_Begin();
    Actor_SetAnimation(26, 1);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 20);
    Actor_FaceActor(26, 21, 40);
    Event_SetMessage((s32)MsgHaidiaTheStoneFellOnThe);
    Event_SayThenWait(26, 20);
    Camera_SetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1510000, -1, 0x1100000, 1);
    Event_Wait(20);
    Actor_RunRepeatedMotion(26, 2);
    Event_Wait(20);
    Actor_FaceActor(26, ACTOR_PARTY_LEADER, 10);
    Event_SayThenWait(26, 40);
    Actor_EnableActionCallback(26, 2);
    Event_End();
}

void FieldScene_RunMiddleAuxiliarySequence(void)
{
    u32 i;
    s32 p8;
    u8 *rec8;
    s32 record;
    s32 v2;

    Event_Begin();
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 82, 0x2f8);
    Actor_FaceEachOther(15, ACTOR_PARTY_LEADER, 30);
    Event_SetMessage((s32)MsgHaidiaIHaveSomePsynergyLeft);
    Event_SayThenWait(15, 20);
    Value3(SceneActor_SetPairZeroAndValue, 15, 0xa000, 20);
    Actor_SetAttachedEffect(15, 0x102);
    Event_Wait(20);
    SceneState_ApplyPair140And0();
    for (i = 0; i < 40; i++) {
        OverlayObject_UpdateOnFrameBit1(Engine_ActorGet(15));
        Task_Wait(1);
    }
    Value2(Scheduler_AddOrUpdateCallback, (s32)FieldScene_RunStep15, 0xc80);
    Value2(Scheduler_AddOrUpdateCallback, (s32)FieldScene_RunStep20, 0xc80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 10);
    rec8 = Value1(Engine_ActorGet, 20);
    v2 = rec8[85];
    rec8[85] = 0;
    p8 = v2;
    for (i = 0; i < 40; i++) {
        *(s32 *)(rec8 + 12) += 0x1800;
        Task_Wait(1);
    }
    rec8[85] = p8;
    Call1(Scheduler_RemoveCallback, (s32)FieldScene_RunStep15);
    Call1(Scheduler_RemoveCallback, (s32)FieldScene_RunStep20);
    Task_Wait(1);
    Audio_PlayCue(161);
    Actor_SetChildValue(15, 0);
    Actor_SetChildValue(20, 0);
    Event_Wait(40);
    FieldScene_Forward4dac();
    Actor_FaceEachOther(ACTOR_PARTY_LEADER, 15, 30);
    Event_ShowMessage(15, 0);
    Event_End();
}

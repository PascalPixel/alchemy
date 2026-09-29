#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaHey[];
extern u8 HaidiaArashi_ActorTwentyTwoScriptA[];
extern u8 HaidiaArashi_ActorTwentyTwoScriptB[];
void Event_PrepareObjectAndApplyValue();

void FieldScene_RunScene372SequenceE(void)
{
    s32 record;
    s32 base;
    s32 v6;

    if (GameFlag_IsSet(0x837) != 0) {
    } else {
        Event_Begin();
        Actor_SetAttachedEffect(22, 0x100);
        base = (s32)MsgHaidiaHey;
        Event_SetMessage(base);
        Event_ShowMessage(22, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
        Camera_SetSpeed(0x6666, 0xccc);
        Camera_MoveTo(0x1000000, -1, 0x24c0000, 1);
        Actor_SetSpeed(22, 0x20000, 0x10000);
        Value2((s32 (*)())Object_SetActionCallbackAndRefreshById, 22, (s32)HaidiaArashi_ActorTwentyTwoScriptA);
        Actor_FaceEachOther(ACTOR_PARTY_LEADER, 22, 0);
        Event_Wait(30);
        Value2((s32 (*)())Engine_ActorEnableActionCallback, 22, (s32)HaidiaArashi_ActorTwentyTwoScriptB);
        Event_ShowMessage(22, 0);
        v6 = 128;
        record = Actor_Get(22);
        *(volatile s32 *)(record + 28) = (v6 << 9);
        Actor_RunRepeatedMotion(22, 1);
        Event_Wait(20);
        Event_AskYesNo(22, 0);
        Event_Wait(40);
        Actor_RunRepeatedMotion(22, 1);
        Event_SetMessage((base + 5));
        Event_ShowMessageAndWait(22, 0, 20);
        Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Actor_SetAnimationAndWait(22, 3);
        Event_ShowMessage(22, 0);
        Actor_SetSpeed(22, (v6 << 9), 0x8000);
        Actor_SetAnimation(22, 2);
        record = Value1(Engine_ActorGet, ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(22, *(s16 *)(record + 10), *(s16 *)(record + 18));
        }
        Actor_WaitForMove(22);
        Actor_SetPosition(22, 0, 0);
        Event_PrepareObjectAndApplyValue(1, 1);
        Actor_SetAnimation(21, 3);
        GameFlag_Set(0x837);
        Event_End();
    }
}

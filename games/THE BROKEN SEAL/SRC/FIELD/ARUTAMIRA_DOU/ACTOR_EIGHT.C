#include "ARUTAMIRA.H"
extern u8 MsgArutamiraWait[];

void FieldScene_RunFlagGatedActorEightDialogue(void)
{
    if (GameFlag_IsSet(0x960) == 0)
        return;
    if (GameFlag_IsSet(0x962) != 0)
        return;

    GameFlag_Set(0x961);
    Event_Begin();
    Event_SetMessage((s32)MsgArutamiraWait);
    Event_ShowMessage(8, 0);
    Event_Wait(10);
    Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Event_Wait(30);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
    Event_Wait(30);
    Event_ShowMessage(8, 0);
    Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Event_Wait(20);
    Event_End();
}

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaDontLeaveMeHere[];
extern u8 MsgHaidiaUghHrnghhh[];

void FieldScene_RunScene372SequenceB(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x836) == 0) {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage((s32)MsgHaidiaUghHrnghhh);
            Event_ShowMessageAndWait(22, 0, 20);
            Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x17e, 0x26b);
            Actor_FaceActor(ACTOR_PARTY_LEADER, 22, 0);
            Actor_RunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
            Event_Wait(30);
            Event_ShowMessage(22, 0);
            GameFlag_Set(0x836);
            Event_End();
        }
    }
}

void FieldScene_RunActor22SceneWhenFlag836Only(void)
{
    if (GameFlag_IsSet(0x837) == 0 && GameFlag_IsSet(0x836) != 0) {
        Event_Begin();
        Actor_RunRepeatedMotion(22, 2);
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaDontLeaveMeHere);
        HaidiaArashi_RunCallOutSequence();
        Event_End();
    }
}

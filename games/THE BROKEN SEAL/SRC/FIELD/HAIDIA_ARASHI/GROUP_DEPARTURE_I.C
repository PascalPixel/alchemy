#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaOh[];
extern u8 MsgHaidiaTwoDontEnough[];

void SceneDialogue_RunActorTenFlag30dDialogue(void)
{
    s32 v2000 = 0x2000;
    u8 *tbl;

    Event_Begin();
    Actor_SetAnimation(10, 1);
    Event_Wait(10);
    Actor_FaceEachOther(10, ACTOR_PARTY_LEADER, 20);
    if (GameFlag_IsSet(0x30d) != 0) {
        Event_SetMessage((s32)MsgHaidiaTwoDontEnough);
        Event_ShowMessageAndWait(10, 0, 10);
    } else {
        Event_SetMessage((s32)MsgHaidiaOh);
        Actor_StartRepeatedMotion(10, 1);
        Event_ShowMessageAndWait(10, 0, 10);
        Actor_StartRepeatedMotion(10, 2);
        Event_ShowMessageAndWait(10, 0, 10);
    }
    Actor_FaceDirection(10, v2000, 20);
    Actor_SetAnimation(10, 5);
    Event_Wait(10);
    {
        u8 *rec;
        s32 v;
        rec = Actor_Get(10);
        v = IwramUnsignedRemainder(Random_Next(), 0x5A) + 60;
        tbl = HaidiaArashi_ActorEightScript;
        *(u16 *)(rec + 0x64) = v;
        Actor_EnableActionCallback(10, tbl);
    }
    Event_Wait(20);
    GameFlag_Set(0x30d);
    Event_End();
}

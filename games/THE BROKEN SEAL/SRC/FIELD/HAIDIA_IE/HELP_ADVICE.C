/* The villager who points the way the others went. */
#include "HAIDIA.H"
extern u8 MsgHaidiaSureHelp[];
extern u8 MsgHaidiaWentOffWay[];

void Villager_PointTheWay(void)
{
    Event_Begin();
    Actor_FaceEachOther(16, ACTOR_PARTY_LEADER, 10);
    if (GameFlag_IsSet(0x840) != 0) {
        Event_SetMessage((s32)MsgHaidiaSureHelp);
        Event_ShowMessage(16, 0);
    } else {
        Event_SetMessage((s32)MsgHaidiaWentOffWay);
        Event_ShowMessage(16, 0);
    }
    Event_End();
}

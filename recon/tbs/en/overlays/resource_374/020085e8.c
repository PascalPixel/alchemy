/* Draft of resource_374 0x020085e8 (SceneDialogue_ShowLineEB1OrEB0): it
 * matches the ROM byte for byte now that the messages it loads from the
 * literal pool have catalogue names (MsgHaidiaSureHelp, MsgHaidiaWentOffWay).
 * The listing keeps these rows until the draft is adopted. */
#include "HAIDIA.H"
extern u8 MsgHaidiaWentOffWay[];
extern u8 MsgHaidiaSureHelp[];

void SceneDialogue_ShowLineEB1OrEB0(void)
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

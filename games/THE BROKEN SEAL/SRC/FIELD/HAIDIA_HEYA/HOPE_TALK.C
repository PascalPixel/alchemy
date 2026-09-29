/* A villager hopes the party did not get sick on its travels. */
#include "TIMED_EVENTS.H"
extern u8 MsgHaidiaHopeDidntGet[];

void HaidiaHeya_TalkHopeDidntGetSick(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaHopeDidntGet);
    Event_ShowMessage(0x800b, 0);
    Event_End();
}

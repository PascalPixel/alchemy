/* Draft of resource_372 0x0200c618 (FieldScene_RunScriptedStep1120): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgHaidiaSChestValuables). The listing
 * keeps these rows until the draft is adopted. */

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaSChestValuables[];

void FieldScene_RunScriptedStep1120(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgHaidiaSChestValuables, 1);
    Event_End();
}

/* Draft of resource_376 0x02008298 (FieldScene_RunScene376_02000298): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgHaidiaHopeDidntGet). The listing
 * keeps these rows until the draft is adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/TIMED_EVENTS.H"
extern u8 MsgHaidiaHopeDidntGet[];

void FieldScene_RunScene376_02000298(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage((s32)MsgHaidiaHopeDidntGet);
    Event_ShowMessage(0x800b, 0);
    Event_End();
}

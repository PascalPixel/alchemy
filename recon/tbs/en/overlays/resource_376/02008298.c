/* Draft of resource_376 0x02008298..0x020082bc (36 bytes with pool),
 * FieldScene_RunScene376_02000298; the listing keeps the rows. Remaining
 * difference: the reference loads message 0x1c40 from its literal pool, as a
 * link-time message symbol is loaded; the constant compiles to movs #113 /
 * lsls #6 (23 bytes differ). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_HEYA/TIMED_EVENTS.H"

void FieldScene_RunScene376_02000298(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    Event_SetMessage(MSG_HOPE_YOU_DIDNT_GET_SICK);
    Event_ShowMessage(0x800b, 0);
    Event_End();
}

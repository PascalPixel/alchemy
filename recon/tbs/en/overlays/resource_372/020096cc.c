/* NONMATCHING: resource_372 at 0x020096cc, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_D.C, stays listing.
 *
 * Remaining difference: the ROM loads message 0xed0 from its literal pool, so the source named it by a link-time symbol; the main image has no name for it, and a plain constant builds it with a move and a shift.
 */

#include "GROUP_DEPARTURE.H"

void FieldScene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x841) != 0) {
        Event_Begin();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_SetMessage(MSG_ROBIN_BOULDER_WE_NEED_GET);
        Event_ShowMessage(22, 0);
        Actor_FaceDirection(22, 0xe000, 10);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage(MSG_DUMP_MY_STUFF);
            Func_02002e5e();
            Event_End();
        }
    }
}

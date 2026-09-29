/* NONMATCHING: resource_372 at 0x020096cc, from FIELD/HAIDIA_ARASHI/GROUP_DEPARTURE_D.C, stays listing.
 *
 * Remaining difference: its messages have catalogue names now and its bytes
 * match the ROM, but it names a symbol no link defines (Func_02002e5e).
 */

#include "GROUP_DEPARTURE.H"
extern u8 MsgHaidiaBoulderNeedGet[];
extern u8 MsgHaidiaWantDumpStuff[];

void FieldScene_RunScene372SequenceC(void)
{
    u32 i;
    s32 record;

    if (GameFlag_IsSet(0x841) != 0) {
        Event_Begin();
        Actor_FaceActor(22, ACTOR_PARTY_LEADER, 0);
        Event_Wait(20);
        Event_SetMessage((s32)MsgHaidiaBoulderNeedGet);
        Event_ShowMessage(22, 0);
        Actor_FaceDirection(22, 0xe000, 10);
        Event_End();
    } else {
        if (GameFlag_IsSet(0x837) == 0) {
            Event_Begin();
            Event_SetMessage((s32)MsgHaidiaWantDumpStuff);
            Func_02002e5e();
            Event_End();
        }
    }
}

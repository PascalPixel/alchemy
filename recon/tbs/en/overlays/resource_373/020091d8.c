/* Draft of resource_373 0x020091d8 (FieldScene_RunScene373SequenceD): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgHaidiaRrruffRrrruff). The listing
 * keeps these rows until the draft is adopted. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.H"
extern u8 MsgHaidiaRrruffRrrruff[];

void FieldScene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (GameFlag_IsSet(0x808) == 0) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        base5_f4d = (s32)MsgHaidiaRrruffRrrruff;
        Event_SetMessage(base5_f4d);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Message_ShowCentered((base5_f4d + 2), 1);
        Event_Wait(6);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
        Event_End();
    }
}

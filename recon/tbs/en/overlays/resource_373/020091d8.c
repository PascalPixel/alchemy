/* Draft of resource_373 0x020091d8..0x02009244 (108 bytes with pool),
 * FieldScene_RunScene373SequenceD; the listing keeps the rows. Remaining
 * difference: the reference keeps message 0xf4d in a saved register loaded
 * after the preceding calls, as a link-time message symbol is loaded; the
 * constant is loaded from the pool earlier (14 bytes differ, same size). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/HAIDIA_MURA/STAGED_MOTION.H"

void FieldScene_RunScene373SequenceD(void)
{
    u32 i;
    s32 record;
    s32 base5_f4d;

    if (GameFlag_IsSet(0x808) == 0) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        base5_f4d = MSG_RRRUFF_RRRRUFF;
        Event_SetMessage(base5_f4d);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Message_ShowCentered((base5_f4d + 2), 1);
        Event_Wait(6);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
        Event_End();
    }
}

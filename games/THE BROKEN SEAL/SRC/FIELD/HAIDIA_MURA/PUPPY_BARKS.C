/* Until flag 0x808 is set, the two puppies bark and the leader walks on. */
#include "STAGED_MOTION.H"
extern u8 MsgHaidiaRrruffRrrruff[];

void FieldScene_RunPuppyBarks(void)
{
    s32 msg;

    if (GameFlag_IsSet(0x808) == 0) {
        Event_Begin();
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
        msg = (s32)MsgHaidiaRrruffRrrruff;
        Event_SetMessage(msg);
        Event_ShowMessageAndWait(15, 0, 2);
        Event_ShowMessageAndWait(16, 0, 2);
        Message_ShowCentered(msg + 2, 1);
        Event_Wait(6);
        Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 69, 0x366);
        Event_End();
    }
}

/* Draft of resource_3bf 0x0200c704 (FieldScene_RunActorTwentyOneSequence): it
 * matches the ROM byte for byte now that the message it loads from the
 * literal pool has a catalogue name (MsgRunpaLeftGuardHearsSomeone). The
 * listing keeps these rows until the draft is adopted. */
#include "FORTRESS.H"
extern u8 MsgRunpaLeftGuardHearsSomeone[];

void FieldScene_RunActorTwentyOneSequence(void)
{

    s32 base5_2411;

    Actor_ShowEmote(21, 0x101, 30);
    Actor_FaceDirection(21, 0xd000, 0);
    Event_Wait(50);
    Actor_FaceDirection(21, 0xb000, 0);
    Event_Wait(50);
    Actor_FaceDirection(21, 0x5000, 0);
    Event_Wait(50);
    base5_2411 = (s32)MsgRunpaLeftGuardHearsSomeone;
    Event_SetMessage(base5_2411);
    Event_ShowMessage(21, 0);
    Actor_SetAnimation(21, 4);
    Event_Wait(60);
    Actor_FaceDirection(21, 0xb000, 0);
    Event_Wait(40);
    Event_SetMessage((base5_2411 + 1));
    Event_ShowMessage(21, 0);
}

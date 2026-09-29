/* Draft of FieldScene_RunActorTwentyOneSequence, resource_3bf at 0x0200c704, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

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
    base5_2411 = 0x2411;
    Event_SetMessage(base5_2411);
    Event_ShowMessage(21, 0);
    Actor_SetAnimation(21, 4);
    Event_Wait(60);
    Actor_FaceDirection(21, 0xb000, 0);
    Event_Wait(40);
    Event_SetMessage((base5_2411 + 1));
    Event_ShowMessage(21, 0);
}

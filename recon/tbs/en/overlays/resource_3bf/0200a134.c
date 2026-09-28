/* Draft of RunActor12InteractionSequence, resource_3bf at 0x0200a134, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

void RunActor12InteractionSequence(void)
{
    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 256, 60);
    {
        u8 *t = 0x240d;

        Event_SetMessage((s32)t);
        Event_ShowMessage(12, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 50);
        Event_SetMessage((s32)(t + 1));
    }
    Event_ShowMessage(12, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(548);
}

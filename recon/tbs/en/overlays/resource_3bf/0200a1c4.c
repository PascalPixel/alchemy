/* Draft of FieldScene_RunScene3bf_020021c4, resource_3bf at 0x0200a1c4, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

void FieldScene_RunScene3bf_020021c4(void)
{
    u32 i;
    s32 record;
    s32 base5_240d;

    Event_Begin();
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 0);
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(13, 0x100, 60);
    Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    base5_240d = 0x240d;
    Event_SetMessage(base5_240d);
    Event_ShowMessage(13, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 30);
    Event_SetMessage((base5_240d + 1));
    Event_ShowMessage(13, 0);
    Event_CloseScreen();
    Event_Wait(60);
    Event_RequestExit(60);
    Event_End();
    GameFlag_Set(0x225);
}

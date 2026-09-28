/* Draft of FieldScene_RunSupplementalSequenceOne, resource_3bf at 0x02009e94, built with
 * games/THE BROKEN SEAL/SRC/FIELD/RUNPA_JO/FORTRESS.H.
 * Remaining difference: a message number held across calls is a C constant,
 * which GCC's second scheduling pass hoists above the calls before its first
 * use; the ROM loads it after them, as it would a link-time value.
 * The listing keeps these rows. */
#include "FORTRESS.H"

/* Runs a scripted beat on the objects indexed 12, 13 and 14, stepping
 * through the entries at 0x2438 as it goes, then sets the scene
 * phase word and a status byte at +0x22b of the record at gGameState
 * before handing off to the next step. */
void FieldScene_RunSupplementalSequenceOne(void)
{

    s32 sequence_2438;

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Actor_SetAnimation(12, 1);
    Actor_SetAnimation(13, 1);
    Actor_SetAnimation(14, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(12, 0x100, 0);
    Event_Wait(30);
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
    sequence_2438 = 0x2438;
    Event_SetMessage(sequence_2438);
    Event_ShowMessage(12, 0);
    Actor_ShowEmote(13, 0x100, 0);
    Actor_ShowEmote(14, 0x100, 0);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Event_Wait(65);
    Actor_FaceDirection(13, 0x5000, 0);
    Actor_FaceDirection(14, 0xd000, 0);
    Event_SetMessage((sequence_2438 + 1));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((sequence_2438 + 2));
    Event_ShowMessage(14, 0);
    Event_SetMessage((sequence_2438 + 3));
    Event_ShowMessage(12, 0);
    Actor_RunRepeatedMotion(13, 1);
    Event_SetMessage((sequence_2438 + 4));
    Event_ShowMessage(13, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_SetMessage((sequence_2438 + 5));
    Event_ShowMessage(14, 0);
    Actor_SetAnimationAndWait(14, 3);
    Event_Wait(60);
    Actor_FaceActor(13, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(14, ACTOR_PARTY_LEADER, 0);
    Event_Wait(70);
    Actor_WalkTo(12, 0x2a0, 88); /* object_id 12, x 0x2a0, z 88 */
    Actor_WaitForMove(12);
#if !defined(TBS_EDITION_JA)
    /* The localized sequence adds this turn after actor 12 is placed. */
    Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
#endif
    Actor_SetAnimationAndWait(12, 3);
    Event_Wait(30);
    Event_SetMessage((sequence_2438 + 6));
    Event_ShowMessage(12, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    Party_SetFields1ceAnd1d0(0xa1, 31);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    BattleFx_SetWeightedResult(98, 3);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(14, 0, 0);
    Event_End();
    GameFlag_Set(0x94a); /* main:080770c8 */
}

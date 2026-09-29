/* The Lunpa fortress: the end of a sequence and an empty object. */
#include "FORTRESS.H"

void FieldScene_RunSequenceTail(void)
{
    Event_Begin();
    Actor_SetPosition(12, 45088768, 5767168); /* object_id 12, x, z */
    Actor_SetPosition(13, 46137344, 5767168); /* object_id 13, x, z */
    Actor_SetPosition(14, 47185920, 6291456); /* object_id 14, x, z */
    Actor_SetAnimation(12, 5); /* object_id 12, action 5 */
    Actor_SetAnimation(13, 5); /* object_id 13, action 5 */
    Actor_SetAnimation(14, 5); /* object_id 14, action 5 */
    Actor_FaceActor(ACTOR_PARTY_LEADER, 13, 0);
    Event_End();
    Event_OpenScreen(); /* main:0808a360 */
}

void InspectEmptySceneObject(void)
{

    Actor_SetAnimation(ACTOR_PARTY_LEADER, 1);
    Audio_PlayCue(113);
    Actor_ShowEmote(16, 256, 60);
    TurnActorToSceneDirection(16);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    gGameState.unknown_1f8[0x22b - 0x1f8] = 3;
    BattleFx_SetWeightedResult(98, 2);
    Actor_SetPosition(16, 0, 0);
    Event_End();
    GameFlag_Set(2379);
}

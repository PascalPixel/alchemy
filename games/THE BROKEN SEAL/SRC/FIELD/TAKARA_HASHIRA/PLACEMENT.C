#include "HASHIRA.H"

void FieldScene_RunTransitionOrFallback(void)
{
    Event_Begin();
    if (FieldScene_RunScene3b3SequenceD() == 0)
        StagedActor_AdvancePair();
    Event_End();
}

/*
 * Scene state reset for overlay resource_3b3. The callee name refers to its
 * own call word rather than to a shared runtime address.
 */
void SceneState_ApplyPlacementResult(void)
{
    StagedActorMovementRequest out;

    Event_Begin();
    if (StagedActor_FindClearPosition((struct StagedActorProbe *)&out) != 0)
        SceneActor_MoveAndRedraw(out);
    Event_End();
}

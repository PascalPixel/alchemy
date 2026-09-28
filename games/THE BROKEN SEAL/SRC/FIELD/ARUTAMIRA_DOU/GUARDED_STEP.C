#include "ARUTAMIRA.H"

void FieldScene_RunGuardedSixWordStep(void)
{
    StagedActorMovementRequest s;

    Event_Begin();
    if (StagedActor_FindClearPosition((struct StagedActorProbe *)&s) != 0) {
        SceneActor_MoveAndRedraw(s);
    }
    Event_End();
}

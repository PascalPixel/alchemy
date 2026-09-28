#include "ENTRY_SETUP.H"

void FieldScene_RunGuardedRectStep(void)
{
    s32 x;
    s32 y;

    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        x = 45;
        y = 43;
        Map_CopyCellAttributes(109, 43, 7, 5, x, y);
        RunStagedActorTransition();
    }
    Event_End();
    VinasuHeya_RunCellPushScene();
}

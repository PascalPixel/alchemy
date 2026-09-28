#include "ENTRY_SETUP.H"

void SceneState_RunConditionalStep(void)
{
    Event_Begin();
    if (SceneActor_TryMoveActorZeroTwoTilesAhead() == 0) {
        s32 k5 = 44, k6 = 39;
        Map_CopyCellAttributes(108, 39, 13, 7, k5, k6);
        RunStagedActorTransition();
    }
    Event_End();
    VinasuHeya_SettlePushedBlocks();
}

s32 SceneActor_SetHeightAboveLinkedRecord(Struct_22a4 *obj)
{
    Struct_22a4b *rec;

    rec = Actor_Get(((s16 *)obj)[50]);
    ((s32 *)obj)[3] = rec->unkC + 0x100000;
    return 0;
}

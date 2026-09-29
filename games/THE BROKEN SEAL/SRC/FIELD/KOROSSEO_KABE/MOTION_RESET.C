#include "TASK.H"

/* Reset the selected actor's motion fields after refreshing it. */
void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    u8 *actor = (u8 *)Engine_ActorGet(slot);
    ObjectDispatch_InitFromTable6(actor);
    *(s32 *)(actor + 36) = 0;
    *(s32 *)(actor + 44) = 0;
    *(s32 *)(actor + 56) = (s32)0x80000000;
    *(s32 *)(actor + 64) = (s32)0x80000000;
}

void SceneState_InitCursorWhenUnset(void)
{
    if (Korosseo_MarkerSlot == -1) {
        Korosseo_MarkerSlot = Resource_LoadFixedBlockBIntoFreeSlot();
    }
}

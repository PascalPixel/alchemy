#include "task.h"

void StagedActor_ResetMotionAfterRefresh(s32 slot)
{
    struct FieldActor *actor = Object_GetById(slot);
    ObjectDispatch_InitFromTable6(actor);
    actor->velocity_x = 0;
    actor->velocity_z = 0;
    actor->target_x = (s32)0x80000000;
    actor->target_z = (s32)0x80000000;
}

void SceneState_InitCursorWhenUnset(void)
{
    if (Korosseo_MarkerSlot == -1) {
        Korosseo_MarkerSlot = Resource_LoadFixedBlockBIntoFreeSlot();
    }
}

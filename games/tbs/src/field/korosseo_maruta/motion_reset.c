#include "log_rolling.h"

void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{
    extern void ObjectDispatch_InitFromTable6();

    struct FieldActor *record;

    record = Object_GetById(selector);
    ObjectDispatch_InitFromTable6();

    record->velocity_x = 0;
    record->velocity_z = 0;
    record->target_x = 0x80000000;
    record->target_z = 0x80000000;
}

void ColossoLogRollingStage_EnsurePaletteHandle(void)
{
    extern s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

    s16 *cursor = &Korosseo_MarkerSlot;

    if (*cursor == -1) {
        *cursor = Resource_LoadFixedBlockBIntoFreeSlot();
    }
}

/* Resetting an actor's motion and claiming the stage's palette handle. */
#include "LOG_ROLLING.H"

void ColossoLogRollingStage_ResetActorMotion(s32 selector)
{
    extern void ObjectDispatch_InitFromTable6();

    u8 *record;

    record = Object_GetById(selector);
    ObjectDispatch_InitFromTable6();

    *(u32 *)(record + 36) = 0;
    *(u32 *)(record + 44) = 0;
    *(u32 *)(record + 56) = 0x80000000;
    *(u32 *)(record + 64) = 0x80000000;
}

void ColossoLogRollingStage_EnsurePaletteHandle(void)
{
    extern s32 Resource_LoadFixedBlockBIntoFreeSlot(void);

    s16 *cursor = &Korosseo_MarkerSlot;

    if (*cursor == -1) {
        *cursor = Resource_LoadFixedBlockBIntoFreeSlot();
    }
}

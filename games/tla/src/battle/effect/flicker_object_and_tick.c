#include "types.h"
#include "frame.h"
#include "object_dispatch.h"
#include "ram_buffer.h"
#include "arc_effect.h"

void BattleFx_WaitForArcGroups(void)
{
    struct ArcEffectWork *work = Ram_HeapSlots->block62;
    s32 i = 0;

    while (work->age[0] <= 31 || work->age[1] <= 31) {
        WaitFrames(1);
        i++;
        if (i > 89)
            return;
    }
}

void BattleFx_FlickerObjectAndTick(s32 arg0)
{
    if ((gFrameCount & 2) != 0) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, 7);
    } else {
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, 0);
    }
    if ((gFrameCount & 15) == 0) {
        BattleFx_SpawnScaledArcObjects(arg0);
    }
}

void BattleFx_CycleObjectAndTick(s32 arg0)
{
    if (gFrameCount & 1)
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, (gFrameCount >> 1) % 6);
    if ((gFrameCount & 15) == 0)
        BattleFx_SpawnScaledArcObjects(arg0);
}

void BattleFx_SetObjectTickMode(struct DispatchObject *object, s32 mode)
{
    if (mode == 0) {
        object->update = 0;
        Animation_ApplyChildValuesFar(object, 0);
    } else if (mode == 1) {
        object->update = (void (*)(struct DispatchObject *))BattleFx_FlickerObjectAndTick;
    } else {
        object->update = (void (*)(struct DispatchObject *))BattleFx_CycleObjectAndTick;
    }
}

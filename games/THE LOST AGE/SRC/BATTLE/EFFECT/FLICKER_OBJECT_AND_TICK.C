#include "TYPES.H"
#include "FRAME.H"
#include "OBJECT_DISPATCH.H"
#include "RAM_BUFFER.H"
#include "ARC_EFFECT.H"

/* Waits until both arc groups are older than 31 frames, or 90 frames. */
void BattleFx_WaitForArcGroups(void)
{
    struct ArcEffectWork *work = Ram_HeapSlots->block_62;
    s32 i = 0;

    while (work->age[0] <= 31 || work->age[1] <= 31) {
        WaitFrames(1);
        i++;
        if (i > 89)
            return;
    }
}

/* Flickers the object between child values 7 and 0 every two frames and
   spawns its arc objects every sixteen. */
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

/* Steps the object through its six child values every other frame and
   spawns its arc objects every sixteen. */
void BattleFx_CycleObjectAndTick(s32 arg0)
{
    if (gFrameCount & 1)
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, (gFrameCount >> 1) % 6);
    if ((gFrameCount & 15) == 0)
        BattleFx_SpawnScaledArcObjects(arg0);
}

/* Sets how the object animates each frame: still (mode 0), flickering
   (mode 1) or cycling. */
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

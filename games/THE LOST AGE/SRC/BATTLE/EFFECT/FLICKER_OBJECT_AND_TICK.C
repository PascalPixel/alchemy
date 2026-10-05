#include "TYPES.H"
#include "FRAME.H"
#include "OBJECT_DISPATCH.H"

void BattleFx_SpawnScaledArcObjects(s32 object);

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

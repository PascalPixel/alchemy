#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "OBJECT_RUNTIME.H"

/* Turns the object by a step that grows by 0x50 each frame until it
   reaches 0x1000, on top of a constant 0x1000. */
void BattleFx_AdvanceObjectField6WithRamp(struct ObjectRuntime *object)
{
    u32 step;

    step = object->action * 0x50;
    object->angle += step + 0x1000;
    if (step < 0x1000)
        object->action++;
}

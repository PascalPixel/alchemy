#include "TYPES.H"
#include "SCENE.H"
#include "MOTION_OBJECT.H"
#include "OBJECT_RUNTIME.H"

void BattleFx_AdvanceAngleWithRamp(struct ObjectRuntime *object)
{
    u32 step;

    step = object->action * 80;
    object->angle += step + 0x1000;
    if (step < 0x1000)
        object->action++;
}

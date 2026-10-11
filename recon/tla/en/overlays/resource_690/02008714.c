/* Draft, differ 6 of 33: the native loads y before building 0x8000 and loads
 * scale_x between `movs r2, #160` and its shift; the draft builds both
 * constants before the loads. A local for the step changes nothing. */
#include "types.h"
#include "object_runtime.h"

void Func_02002000(struct ObjectRuntime *object);

void Func_02000714(struct ObjectRuntime *object)
{
    object->x += object->action << 8;
    object->y += 0x8000;
    object->scale_x += 0x51e;
    object->scale_y += 0x51e;
    object->action += 2;
    if (--*(s32 *)&object->linked_object == 0)
        Func_02002000(object);
}

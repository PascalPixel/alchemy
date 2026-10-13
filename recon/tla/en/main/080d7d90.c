/* Near miss of Func_080d7d90, differ 2 of 19: the draft loads scale_y before
 * the step constant, the native the constant first. The twin of The Broken
 * Seal's BattleFx_ShrinkObjectAndDestroySlow; its plain source (scale =
 * scale_x + step first) is differ 4: the scale's add comes before the
 * scale_y store instead of after the angle's load. Tried: scale_y, scale_x,
 * angle statement orders (differ 4), a step variable, 0xFFFFFE40 + scale_y. */
#include "types.h"
#include "motobj.h"

void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    struct MotionObject *object = obj;
    s32 scale_y = object->scale_y + 0xFFFFFE40;
    s32 scale = object->scale_x;
    s32 angle;

    object->scale_y = scale_y;
    angle = object->angle;
    /* FAKEMATCH: the barrier keeps the angle's load ahead of the scale's add; without it the add comes before the scale_y store (two reordered pairs, differ 4). */
    asm volatile("" : "+l"(angle));
    scale += 0xFFFFFE40;
    object->angle = angle + 0x2000;
    object->scale_x = scale;
    if (scale < 0x3000)
        Object_Destroy();
}

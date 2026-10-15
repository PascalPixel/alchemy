/* Near miss of Func_080d7d90, differ 2 of 19: the draft loads scaleY before
 * the step constant, the native the constant first. The twin of The Broken
 * Seal's BattleFx_ShrinkObjectAndDestroySlow; its plain source (scale =
 * scaleX + step first) is differ 4: the scale's add comes before the
 * scaleY store instead of after the angle's load. Tried: scaleY, scaleX,
 * angle statement orders (differ 4), a step variable, 0xFFFFFE40 + scaleY. */
#include "types.h"
#include "motobj.h"

void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    struct MotionObject *object = obj;
    s32 scaleY = object->scaleY + 0xFFFFFE40;
    s32 scale = object->scaleX;
    s32 angle;

    object->scaleY = scaleY;
    angle = object->angle;
    /* FAKEMATCH: the barrier keeps the angle's load ahead of the scale's add; without it the add comes before the scaleY store (two reordered pairs, differ 4). */
    asm volatile("" : "+l"(angle));
    scale += 0xFFFFFE40;
    object->angle = angle + 0x2000;
    object->scaleX = scale;
    if (scale < 0x3000)
        Object_Destroy();
}

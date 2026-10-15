#include "types.h"
#include "object_runtime.h"

// Raises the object's scale in sixteenths to full size and commits its
// position.
void BattleFx_SnapScaleToFull(struct ObjectRuntime *object)
{
    s32 next;
    s32 scale;
    s32 limit;
    s32 step;

    if (object != NULL)
    {
        limit = 0xff;
        scale = object->scaleX;
        /* FAKEMATCH: the native starts the 0xffff limit, loads the scale, then finishes the limit; as a constant the load comes after its shift (differ 2). */
        asm volatile("" : "+l"(limit));
        limit = (limit << 8) + 0xff;
        if (scale <= limit)
        {
            /* FAKEMATCH: the native copies the limit to ip for the loop; as a free temporary the bound shares r3 with the limit and the loop takes other registers (differ 21). */
            register s32 bound asm("r12");

            step = 0x1000;
            /* FAKEMATCH: the step is kept in a register ahead of the copy, as the native hoists it; as a constant it is built around the copy (differ 4). */
            asm("" : "+l"(step));
            bound = limit;
            do
            {
                next = scale + step;
                scale = next;
            } while (next <= bound);
            object->scaleX = next;
            object->scaleY = next;
        }
        Object_CommitPosition(object);
    }
}

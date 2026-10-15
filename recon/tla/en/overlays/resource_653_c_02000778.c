/* Near miss, differ 2 of 78 (one reordered pair): the native lowers the
 * stack (sub sp, #12) after it loads the object's x; the draft lowers it
 * one instruction earlier. One-pass blocks after the focus load or the
 * first store leave differ 8 and 4. Also tried: the array declared first,
 * a struct for the position, braces around the first three stores, the
 * focus pinned to r8 (differ 17), x read first with the array pointer pinned
 * to r5 (2); a 5-minute permute found nothing better. In sched2 the stack
 * adjustment and the focus copy tie at priority 47 and the adjustment has
 * more dependents. */
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

struct EventWork1cc {
    u8 unk000[0x1cc];
    struct ObjectRuntime *focus;
};

void Func_02000778(struct ObjectRuntime *object, s32 dz)
{
    struct ObjectRuntime *focus = ((struct EventWork1cc *)Ram_HeapSlots->eventWork)->focus;
    s32 position[3];

    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z + dz;
    Func_02005180(object, position);
    position[0] = object->x;
    position[1] = object->y;
    position[2] = object->z + 0x80000;
    if (Func_02005180(object, position) > 0)
        return;
    position[0] = object->x + 0x5b333;
    position[1] = object->y;
    position[2] = object->z + 0x5b333;
    if (Func_02005180(object, position) > 0)
        return;
    position[0] = object->x - 0x5b333;
    position[1] = object->y;
    position[2] = object->z + 0x5b333;
    if (Func_02005180(object, position) > 0)
        return;
    focus->z += dz;
    object->z += dz;
}

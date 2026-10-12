/* Near miss, differ 2 of 78 (one reordered pair): the native lowers the
 * stack (sub sp, #12) after it loads the object's x; the draft lowers it
 * one instruction earlier. One-pass blocks after the focus load or the
 * first store leave differ 8 and 4. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

struct EventWork1cc {
    u8 unknown_000[0x1cc];
    struct ObjectRuntime *focus;
};

void Func_02000778(struct ObjectRuntime *object, s32 dz)
{
    struct ObjectRuntime *focus = ((struct EventWork1cc *)Ram_HeapSlots->event_work)->focus;
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

/* Draft, differ 31 of 34: the native sets r0 to 1 once and subtracts the test (eors #255, negs, orrs, lsrs #31, subs r0 r0 r3); this form branches on cmp #255. Tried == 0xff, 1 - (!= 0xff) and a result variable. */
#include "../../../../games/tla/src/field/overlay_697/ov697.h"

s32 Func_02000df8(s32 x, s32 z)
{
    struct ObjectRuntime *object = Object_GetById(15);
    struct Ov697MapWork *map = (struct Ov697MapWork *)Ram_HeapSlots->map_work;
    s32 result;

    if (object->x >> 20 == x && object->z >> 20 == z)
        return 1;
    result = 1;
    if (map->layers[0].cells[((x + (z << 7)) << 2) + 2] != 0xff)
        result = 0;
    return result;
}

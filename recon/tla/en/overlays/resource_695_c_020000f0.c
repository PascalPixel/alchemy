/* Near miss, differ 8 of 39, registers only: clearing the priority flags,
 * the native addresses the byte through r3 and builds the zero in r2; the
 * draft uses r2 and r3 the other way round. A whole-byte bit-field, a cast
 * store, a pointer local and an and-assignment were tried. */
#include "../../../../games/tla/src/field/overlay_695/local.h"

void Func_020000f0(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 x = object->x >> 20;

    if (x == 38) {
        GameFlag_SetBit(0x9a8);
        object->priority_flags = 0;
        Func_02000098(id);
        Func_02001060(40, 19, 1, 1, x, 19);
    }
    Func_02001060_(99, 6, 4, 3, 99, 18);
}

/* Near miss, differ 2 of 12: the native loads the target before it builds
   0x80000000 (ldr r2, [r0, #56]; movs r3, #128); this draft starts the
   constant first. Two direct compares, an inverted test, a result local and
   two early returns are no closer; only an empty asm after the load matches,
   and no device is spent on it. */
#include "../../../../../games/tla/src/field/overlay_661/scene.h"

s32 Func_020021f0(struct FieldActor *object)
{
    s32 x = object->target_x;

    if (x == ACTOR_NO_TARGET && object->target_z == x)
        return 1;
    return 0;
}

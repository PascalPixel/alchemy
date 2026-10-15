/* Near miss, differ 2: the listing loads y before it builds the rise (ldr r3, [r0, #12]; movs r2, #128); a
 * one-pass block between the two stores moves the zero argument instead, and so does an empty asm there
 * (movs r1, #0 falls from the first store to the call; a zero local is folded back). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Func_080daecc(s32 id, s32 a1);

void Func_080db444(s32 id, s32 force)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 y;

    if (object != NULL) {
        if (object->motionFlags == 0) {
            object->z += 0xfff00000;
            y = object->y;
            object->y = y + 0x400000;
            Func_080daecc(id, 0);
        } else if (force) {
            Func_080daecc(id, 0);
        }
    }
}

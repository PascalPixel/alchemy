/* Draft, differ 4 of 34: the native builds the last argument's 10 before it loads object->x and the second argument's 10 after; this form loads x first. Tried: Call6, x in a local, a one-pass block or asm("") after the store (differ 4-8). */
#include "../../../../games/tla/src/field/overlay_6a9/ov6a9.h"

void Func_02000ba8(void)
{
    struct FieldActor *object;

    Object_GetById(4)->priority_flags |= 0x20;
    Func_0200114c();
    Func_02001154(0, 0x200, 8, 9);
    object = Object_GetById(10);
    object->motion_flags = 0;
    Func_020010ac(16, 10, 1, 1, object->x >> 20, 10);
}

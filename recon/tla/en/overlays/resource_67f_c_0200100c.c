/* Near miss, differ 8 of 40: written with goto the loop keeps the listing's shape (one test at
 * the bottom, 0xffff rebuilt on every pass; as a while loop the test is copied and the constant
 * hoisted to r8). Left: the register that holds the offset 4, where object is copied for the
 * flag byte, and where 0xffff is shifted. */
#include "../../../../games/tla/src/field/overlay_67f/ov67f.h"

void Func_0200100c(u16 *list)
{
    struct ObjectRuntime *object;
    s32 flag;

    goto test;
next:
    object = Object_GetById(list[0]);
    flag = (s16)list[2];
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Func_02001428(object, GameFlag_Test((u16)flag) + 1);
    object->motion_flags = 0;
    object->collision_flags &= 0xfe;
    object->action = flag;
    list += 3;
    Func_02000f30(object);
test:
    if (list[0] != 0xffff)
        goto next;
}

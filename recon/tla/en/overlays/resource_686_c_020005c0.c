/* Draft, differ 2 of 28: the native loads the -0x40000 literal before the
 * object's y (ldr r2 =; ldr r3 [r5, #12]); this form loads y first. Tried:
 * += (s32)0xfffc0000, constant first, a y temporary, a constant temporary,
 * and the y update before the motion_flags store (differ 13). Round five: the
 * step behind a "+l" barrier gives the native pair but moves the object's
 * copy ahead of the first call's flags (differ 2); pinned to r2 with a
 * one-pass block the load stays late (differ 2), with a clobber barrier the
 * zero byte and its offset trade places (differ 2). */
#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

s32 Func_020005c0(struct ObjectRuntime *object)
{
    ObjectDispatch_SetSingleChildFlags(object, 0);
    ((struct FieldSprite *)object->animation)->priority = 2;
    object->priority_flags &= 0xfe;
    object->motion_flags = 0;
    object->y -= 0x40000;
    return 0;
}

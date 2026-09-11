#include "types.h"
#include "scene.h"
#include "abi/object/reset_and_clear_field59.h"


s32 Object_ResetAndClearField59(void *obj)
{
    Obj_Apply(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}

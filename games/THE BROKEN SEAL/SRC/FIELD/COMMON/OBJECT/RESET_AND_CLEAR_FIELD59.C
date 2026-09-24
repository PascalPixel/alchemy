#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 ObjectDispatch_SetSingleChildField26Far(void *, s32);

s32 Object_ResetAndClearField59(void *obj)
{
    ObjectDispatch_SetSingleChildField26Far(obj, 0);
    FIELD_AT_OFFSET(obj, s8 *, 0x59) = 0;
    return 0;
}

#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

s32 Func_080a10d0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 UiIcon_CreateWithResourceVariant(s32 arg0, s32 arg1, s32 arg2);

s32 UiMenu_CreateCursor(void *work)
{
    s32 handle;
    s32 zero = 0;
    s32 state;
    void *object;

    FIELD_AT_OFFSET(work, s32 *, 0x10) = zero;
    Func_080a10d0((s32 *)((u8 *)work + 0x10), zero, zero, 13, 5, 2);
    handle = FIELD_AT_OFFSET(work, s32 *, 0x10);
    object = (void *)UiIcon_CreateWithResourceVariant(handle, -8, 11);
    FIELD_AT_OFFSET(object, u8 *, 5) = 13;
    FIELD_AT_OFFSET(work, u8 *, 0x1C) = 255;
    FIELD_AT_OFFSET(work, u8 *, 0x1D) = zero;
    FIELD_AT_OFFSET(work, void **, 0x14) = object;
    state = 254;
    FIELD_AT_OFFSET(object, u8 *, 0x0F) = state;
    state -= 255;
    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x18), s8 *, 0x0F) = state;
    return handle;
}

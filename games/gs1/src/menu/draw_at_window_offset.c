#include "types.h"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080153c8(s32, s32, s32, s32, s32);

s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    Func_080153c8(FIELD_AT_OFFSET(win, u16 *, 0xC) + arg1 + 1, FIELD_AT_OFFSET(win, u16 *, 0xE) + arg2 + 1, arg3, arg4, arg5);
}

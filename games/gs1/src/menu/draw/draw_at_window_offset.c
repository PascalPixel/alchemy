#include "types.h"
#include "scene.h"
#include "abi/menu/draw/draw_at_window_offset.h"


s32 Menu_DrawAtWindowOffset(void *win, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
    Menu_SetRange(FIELD_AT_OFFSET(win, u16 *, 0xC) + arg1 + 1, FIELD_AT_OFFSET(win, u16 *, 0xE) + arg2 + 1, arg3, arg4, arg5);
}

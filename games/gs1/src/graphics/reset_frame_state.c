#include "types.h"
#include "global_cells.h"

/*
 * Func_080072f0 names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */
s32 Func_080072f0(s32, s32, s32, s32);

void Graphics_ResetFrameState(void)
{
    *(s8 *)ADDR_03001D00 = 0;
    Func_080072f0(ADDR_03001400, 0x400, ADDR_03001D00, 0x03000164);
}

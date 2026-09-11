#include "types.h"
#include "scene.h"
#include "global_cells.h"

/*
 * Sys_SetMode names a bx rN veneer slot, so this is an indirect call
 * through the register loaded just before it -- the relocated routine at
 * 0x03000164. Its argument count is not established.
 */

void Graphics_ResetFrameState(void)
{
    *(s8 *)ADDR_03001D00 = 0;
    Sys_SetMode(ADDR_03001400, 0x400, ADDR_03001D00, 0x03000164);
}

#include "types.h"
#include "scene.h"
#include "abi/shop/cursor/step.h"
#include "global_cells.h"

void Shop_StepCursor(void)
{
    Sys_Check(*(s32 *)ADDR_03001F2C + 0x380);
}

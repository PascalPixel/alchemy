#include "types.h"
#include "global_cells.h"

void Func_080cd488(void)
{
    u8 *base = *(u8 **)ADDR_03001EEC;
    *(u32 *)0x04000028 = *(u32 *)(base + 0x77D0);
    *(u32 *)0x0400002C = *(u32 *)(base + 0x77D4);
}

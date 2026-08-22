#include "types.h"
#include "global_cells.h"

void Func_080e3a14(void)
{
    u8 *state = *(u8 **)ADDR_03001EEC;
    u8 *object = *(u8 **)(state + 0x7828);

    if (*(s16 *)(object + 0x24) > 0x7f)
        (void)*(volatile s32 *)ADDR_03001AE8;
}

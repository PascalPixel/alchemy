#include "types.h"
#include "scene.h"
#include "abi/shop/effect/init.h"
#include "global_cells.h"

void Shop_InitEffect(void)
{
    Effect_Apply(*(s32 *)ADDR_03001EBC + 0x236, 1);
    Effect_Check(0x10);
}

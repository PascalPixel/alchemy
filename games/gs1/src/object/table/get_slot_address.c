#include "types.h"
#include "global_cells.h"

void *ObjectTable_GetSlotAddress(u32 index)
{
    return *(u8 **)ADDR_03001EBC + index * 4 + 20;
}

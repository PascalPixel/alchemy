#include "types.h"
#include "global_cells.h"

s32 Resource_ResetEntry(u32 index);
void Resource_ResetPendingTransfer(void)
{
    u32 offset = 0x30c;
    u8 *work = *(u8 **)ADDR_03001E98 + offset;

    if (*(u16 *)(work + 0x0a) != 0) {
        Resource_ResetEntry(*(u16 *)(work + 0x0c));
        *(u16 *)(work + 0x0a) = offset = 0;
    }
}

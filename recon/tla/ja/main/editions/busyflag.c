#include "types.h"
#include "ram_buffer.h"

void UiWork_SetBusyFlags(u32 flags)
{
    u8 *work;

    work = Ram_HeapSlots->windowTiles;
    if (work == 0)
        return;
    if (flags & 1)
        work[0x138a] = 1;
    if (flags & 2)
        work[0x138b] = 1;
}

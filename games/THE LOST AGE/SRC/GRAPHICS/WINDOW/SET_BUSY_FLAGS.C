#include "TYPES.H"
#include "RAM_BUFFER.H"

/* Raises the window work's two busy flags selected by bits 0 and 1. */
void UiWork_SetBusyFlags(u32 flags)
{
    u8 *work;

    work = Ram_HeapSlots->window_tiles;
    if (work == 0)
        return;
    if (flags & 1)
        work[0x138a] = 1;
    if (flags & 2)
        work[0x138b] = 1;
}

#include "types.h"
#include "ram_buffer.h"

void Func_08041f70(s32 value)
{
    *(u16 *)(Ram_HeapSlots->window_tiles + 0xf3e) = value & 0xf;
}

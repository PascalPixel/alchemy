#include "types.h"
#include "ram_buffer.h"

void Func_08041f90(s32 value)
{
    u8 *tiles = Ram_HeapSlots->window_tiles;

    asm volatile(""); /* FAKEMATCH: ⚓️ loads the window work before it builds the field offset; without the barrier the offset's first half is scheduled ahead of the load (differ 2) */
    *(u16 *)(tiles + 0xf3c) = value;
}

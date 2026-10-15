#include "types.h"
#include "window.h"
#include "ram_buffer.h"

void Func_08041f90(s32 value)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    /* FAKEMATCH: ⚓️ loads the window work before it builds the field offset; without the barrier the offset's first half is scheduled ahead of the load (differ 2) */
    asm volatile("");
    work->outline = value;
}

/* Draft, differ 4 of 12: the native masks the value after the heap slot's
 * address is shifted and loads the window work before it starts the field
 * offset (ands r0, r2 after lsls r3; ldr r3 before movs r2, #240); this
 * masks first and starts the offset first. A barrier cannot separate the
 * mask from the offset: both belong to the one store. Twin of The Broken
 * Seal's UiWork_SetParamNibble. */
#include "types.h"
#include "window.h"
#include "ram_buffer.h"

void Func_08041f70(s32 param)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    work->colour = param & 15;
}

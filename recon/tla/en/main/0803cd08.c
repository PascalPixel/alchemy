#include "types.h"
#include "ram_buffer.h"
#include "window.h"

u32 UiRender_LookupNamedValue(u32 name, u32 clear)
{
    struct UiRenderWork *work =
        (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
    u32 index;
    u32 value = 0;

    for (index = 0; index < 8; index++) {
        if (work->names[index] == name) {
            value = work->values[index];
            if (clear != 0) {
                work->values[index] = 0;
                work->names[index] = 0;
            }
            break;
        }
    }
    return value;
}

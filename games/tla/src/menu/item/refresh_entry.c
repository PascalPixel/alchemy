#include "m7_interfaces.h"
#include "ram_buffer.h"

void ItemMenu_RefreshEntry(s32 layout)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    struct RenderOutput *object;
    s32 index;
    s32 origin_y;
    s32 base;

    base = (s32)Ram_HeapSlots->menu_runtime;
    origin_y = 0x38;
    if (layout != 1) {
        origin_y = 0x28;
    }
    slot = (struct RenderOutput **)(base + 0x4c);
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}

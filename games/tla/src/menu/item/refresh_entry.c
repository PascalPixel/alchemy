#include "m7_interfaces.h"
#include "ram_buffer.h"

void ItemMenu_RefreshEntry(s32 layout)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    struct RenderOutput *object;
    s32 index;
    s32 originY;
    s32 base;

    base = (s32)Ram_HeapSlots->menuRuntime;
    originY = 0x38;
    if (layout != 1) {
        originY = 0x28;
    }
    slot = (struct RenderOutput **)(base + 0x4c);
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 0x74, originY, 5);
        }
        index++;
        slot++;
    } while (index <= 0xE);
}

#include "M7_INTERFACES.H"
#include "RAM_BUFFER.H"

void ItemMenu_RefreshEntry(s32 layout)
{
    struct RenderOutput **slot;
    struct RenderOutput **scan;
    struct RenderOutput *object;
    s32 index;
    s32 origin_y;
    s32 base;

    base = (s32)Ram_HeapSlots->menu_runtime;
    origin_y = 56;
    if (layout != 1) {
        origin_y = 40;
    }
    slot = (struct RenderOutput **)(base + 76);
    index = 0;
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            ItemMenu_PosOwner(slot, index, 116, origin_y, 5);
        }
        index++;
        slot++;
    } while (index <= 14);
}

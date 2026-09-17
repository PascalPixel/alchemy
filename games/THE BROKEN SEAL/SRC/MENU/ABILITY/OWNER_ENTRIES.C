#include "TYPES.H"
#include "M7_INTERFACES.H"
#include "GLOBAL_CELLS.H"

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns);

void PsynergyMenu_RefreshOwnerEntriesDefault(void)
{
    PsynergyMenu_RefreshOwnerEntries(0x6C, 0x28, 8);
}

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 columns)
{
    struct Object080a1c **slot;
    struct Object080a1c **scan;
    s32 index;

    index = 0;
    slot = (struct Object080a1c **)(*(s32 *)ADDR_03001F2C + 0x48);
    scan = slot;
    do {
        if (*scan++ != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, columns);
        }
        index++;
        slot++;
    } while (index <= 0x1F);
}

void PsynergyMenu_PositionOwnerEntry(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y,
    s32 columns) {
    struct Object080a1c *object;

    if (index > 0x1F) {
        index = 0;
    }
    object = *slot;
    object->y = (index / columns) * 0x10 + origin_y;
    object->x = (index % columns) * 0x10 + origin_x;
    Func_080a17c4(object);
}

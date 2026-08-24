#include "m7_interfaces.h"
#include "global_cells.h"

void PsynergyMenu_RefreshOwnerEntries(s32 origin_x, s32 origin_y, s32 phase) {
    s32 _v1d16;
    s32 _v1t36;
    s32 _v1d75;
    struct Object080a1c **scan;
    struct Object080a1c **slot;
    struct Object080a1c *object;
    s32 index;

    index = 0;
    _v1t36 = (*(s32 *)ADDR_03001F2C);
    slot = (struct Object080a1c **)(_v1t36 + 0x48);
    scan = slot;
    do {
        object = *scan++;
        if (object != NULL) {
            PsynergyMenu_PositionOwnerEntry(slot, index, origin_x, origin_y, phase);
        }
        _v1d75 = 0; index += 1;
        _v1d16 = 0; slot++;
    } while (index <= 0x1F);
}

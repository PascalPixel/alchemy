#include "M7_INTERFACES.H"

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
    UiIcon_PrepareObject(object);
}

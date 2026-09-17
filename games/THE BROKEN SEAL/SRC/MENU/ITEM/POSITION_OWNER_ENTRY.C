#include "M7_INTERFACES.H"

void ItemMenu_PosOwner(struct Object080a1c **slot, s32 index,
    s32 origin_x, s32 origin_y, s32 columns)
{
    struct Object080a1c *object;

    if (index > 15)
        index = 0;
    object = *slot;
    object->y = index / columns * 16 + origin_y;
    object->x = index % columns * 24 + origin_x;
    Func_080a17c4(object);
}

#include "ov697.h"

void Func_0200062c(s32 id)
{
    struct Ov697MapWork *map = (struct Ov697MapWork *)Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        u8 *cells = map->layers[0].cells;
        u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);
        s32 bit = 128; /* FAKEMATCH: ⚓️ builds the bit in the register it ORs into */

        cell[3] = bit | cell[3];
    }
}

void Func_02000660(s32 id)
{
    struct Ov697MapWork *map = (struct Ov697MapWork *)Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != NULL) {
        u8 *cells = map->layers[0].cells;
        u8 *cell = cells + (((object->x >> 20) + ((object->z >> 20) << 7)) << 2);

        cell[3] &= 0x7f;
    }
}

s32 Func_02000694(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = object->linked_object;

    object->x = owner->x;
    object->y = owner->y + (OBJECT_BYTE(object, 99) << 16);
    object->z = owner->z;
    object->terrain_height = owner->terrain_height;
    return 1;
}

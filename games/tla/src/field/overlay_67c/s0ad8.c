#include "local.h"

void Func_02000ad8(void)
{
    Func_02003d24(106, 14, 14, 13, 81, 14);
    Func_02003d24(44, 78, 14, 12, 17, 78);
    Func_02003d1c(81, 14, 14, 13, 17, 14);
    GameFlag_SetBit(0xa6a);
}

void Func_02000b24(s32 id, s32 value)
{
    struct MapWork *map = Ram_HeapSlots->map_work;
    struct ObjectRuntime *object = Object_GetById(id);

    if (object != 0) {
        struct MapCell *cells = map->layers[0].cells;
        struct MapCell *cell = &cells[(object->x >> 20) + (object->z >> 20) * 128];

        cell->collision_code = value;
        cell->attribute_b |= 0x80;
    }
}

void Func_02000b5c(void)
{
    struct ObjectRuntime *object = Object_GetById(12);

    Func_02003d1c_(64, 43, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000b84(void)
{
    struct ObjectRuntime *object = Object_GetById(12);

    Func_02003d1c_(64, 42, 1, 1, object->x >> 20, object->z >> 20);
}

void Func_02000bac(void)
{
    struct ObjectRuntime *object = Object_GetById(12);

    Func_02003d1c_(64, 41, 1, 1, object->x >> 20, object->z >> 20);
}

/* Draft, differ 8 of 31: entered at its test (goto); left is the cell store's registers (see resource_661_c_0200095c.c, its twin, at 2 with integer address arithmetic). */
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "map.h"

void Func_02000918(u16 *list)
{
    struct MapWork *map = Ram_HeapSlots->map_work;

    goto next;
    do
    {
        struct FieldActor *object = Object_GetById(list[0]);

        if (GameFlag_Test(list[2]))
            map->layers[2].cells[Func_020041e0(object)].collision_code = 255;
        list += 3;
    next:;
    } while (*list != 0xffff);
}

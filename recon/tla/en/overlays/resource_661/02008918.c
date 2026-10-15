/* Near miss, differ 2 of 31: the native reads the id between the first two
   halves of 0xffff in the loop test (movs r2, #255; ldrh r3, [r5]; lsls r2,
   r2, #8); this draft shifts first. An id temporary is further away. */
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "map.h"

void Func_02000918(u16 *list)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;

    /* FAKEMATCH: entering the loop at its test keeps the test single and 0xffff built on every pass */
    goto test;
    do {
        struct FieldActor *object = Object_GetById(list[0]);

        if (GameFlag_Test(list[2])) {
            s32 index = Func_020041e0(object);

            struct MapCell *cell = map->layers[2].cells + index;

            cell->collisionCode = 255;
        }
        list += 3;
test:;
    } while (list[0] != 0xffff);
}

/* Near miss, differ 4 of 25: the native advances the list before the cell
   store (adds r5, #6; strb r2, [r3, #2]) and reads the id between the first
   two halves of 0xffff in the loop test; this draft stores first and shifts
   first. */
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "map.h"

void Func_0200095c(u16 *list)
{
    struct MapWork *map = Ram_HeapSlots->mapWork;

    /* FAKEMATCH: entering the loop at its test keeps the test single and 0xffff built on every pass; as a for or while loop the test is copied above the loop and 0xffff is kept in r8 */
    goto test;
    do {
        s32 index = Func_020041e0(Object_GetById(list[0]));

        struct MapCell *cell = map->layers[2].cells + index;

        cell->collisionCode = 232;
        list += 3;
test:;
    } while (list[0] != 0xffff);
}

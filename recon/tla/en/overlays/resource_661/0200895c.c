/* Near miss, differ 14 of 31: the native enters the loop by a branch to its
   test at the bottom and builds 0xffff inside the loop on every pass; this
   draft duplicates the exit test and keeps the constant in r8. while (1)
   with a break, a for loop and a plain while all compile the same. */
#include "../../../../../games/tla/src/field/overlay_661/scene.h"

struct Cell { u8 a, b, c, d; };

void Func_0200095c(u16 *list)
{
    u8 *map = (u8 *)Ram_HeapSlots->map_work;

    for (; list[0] != 0xffff; list += 3) {
        s32 index = Func_020041e0(Object_GetById(list[0]));

        (*(struct Cell **)(map + 424))[index].c = 232;
    }
}

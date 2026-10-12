/* Near miss, differ 8 of 25. Entering the loop at its test (goto) gives the
   native loop: one test, 0xffff rebuilt on every pass. What is left: the
   native adds the index into the cell pointer in r3 (ldr r3, [r3]; adds r3,
   r3, r0), builds 232 in r2 and advances the list before the store; and its
   test loads the id between the two halves of 0xffff. An array index, pointer
   arithmetic and a cell pointer local do not give that. */
#include "../../../../../games/tla/src/field/overlay_661/scene.h"

struct Cell { u8 a, b, c, d; };

void Func_0200095c(u16 *list)
{
    u8 *map = (u8 *)Ram_HeapSlots->map_work;

    /* FAKEMATCH: entering the loop at its test keeps the test single and 0xffff built on every pass; as a for or while loop the test is copied above the loop and 0xffff is kept in r8 */
    goto test;
    do {
        s32 index = Func_020041e0(Object_GetById(list[0]));

        u8 *cell = *(u8 **)(map + 424);

        cell += index * 4;
        cell[2] = 232;
        list += 3;
test:;
    } while (list[0] != 0xffff);
}

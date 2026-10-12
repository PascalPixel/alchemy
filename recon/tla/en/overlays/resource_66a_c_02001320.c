/* Near miss, differ 2 of 75 (one reordered pair): the native loads the
 * object's priority flags before it builds the zero for the cell state;
 * the draft builds the zero first. A flags temporary with a one-pass block
 * gives the order but swaps r2 and r3 in the or (differ 8); a one-pass
 * block between the two statements leaves differ 4. */
#include "../../../../games/tla/src/field/overlay_66a/local.h"

void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer)
{
    struct ObjectRuntime *object = Object_GetById(id);
    struct Ov66aMapCell *cell = ((struct Ov66aMapWork *)Ram_HeapSlots->map_work)->layers[layer].cells;

    cell += Func_02001588();
    if (GameFlag_Test(flag)) {
        cell->state = 0;
        object->priority_flags |= 2;
        switch (kind) {
        case 0:
            Func_020013f0(object, 2);
            break;
        case 1:
            Func_020013f0(object, 4);
            break;
        case 2:
            Func_020013f0(object, 6);
            break;
        }
    } else {
        cell->state = 255;
        switch (kind) {
        case 0:
            Func_020013f0(object, 1);
            break;
        case 1:
            Func_020013f0(object, 3);
            break;
        case 2:
            Func_020013f0(object, 5);
            break;
        }
    }
}

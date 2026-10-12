/* Near miss, differ 8 of 75: where the flag is set the listing loads the priority byte into
 * r2 before it clears the cell and ORs it into the constant's register; this draft clears the cell
 * first and uses r2 and r3 the other way round. */
#include "../../../../games/tla/src/field/overlay_67f/ov67f.h"

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer)
{
    struct ObjectRuntime *object = Object_GetById(id);
    u8 *cell = ((struct Ov67fMapWork *)Ram_HeapSlots->map_work)->layers[layer].cells;

    cell += Func_02001508(object) * 4;
    if (GameFlag_Test(flag)) {
        cell[2] = 0;
        object->priority_flags |= 2;
        switch (kind) {
        case 0:
            Func_02001428(object, 2);
            break;
        case 1:
            Func_02001428(object, 4);
            break;
        case 2:
            Func_02001428(object, 6);
            break;
        }
    } else {
        cell[2] = 255;
        switch (kind) {
        case 0:
            Func_02001428(object, 1);
            break;
        case 1:
            Func_02001428(object, 3);
            break;
        case 2:
            Func_02001428(object, 5);
            break;
        }
    }
}

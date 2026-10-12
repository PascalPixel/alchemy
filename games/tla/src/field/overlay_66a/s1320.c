#include "local.h"

void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer)
{
    struct ObjectRuntime *object = Object_GetById(id);
    struct Ov66aMapCell *cell = ((struct Ov66aMapWork *)Ram_HeapSlots->map_work)->layers[layer].cells;

    cell += Func_02001588();
    if (GameFlag_Test(flag)) {
        /* FAKEMATCH: the flags are read before the cell store and ORed into the register that holds 2, as the native does; as one |= the 0 for the cell is built before the read */
        u8 flags = object->priority_flags;
        s32 bits;

        cell->state = 0;
        bits = 2;
        bits |= flags;
        object->priority_flags = bits;
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

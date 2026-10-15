/* Near miss, differ 4 of 21: the listing loads the table's address, scales the id, then loads
 * object->x between the subtraction and the table read; here a one-pass block around the table
 * read keeps the address first (as in Func_02000280, s01f4.c) but ends the block before x is
 * loaded. The table is s16 (data.c). Tried: no block (differ 6), x or the whole test inside the
 * block (6, 4), x read first (8), a table pointer or an index local (8, 6), an asm barrier after
 * the call (6), permuter 3 minutes (4). */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

extern s16 Data_02005b5c[];

void Func_020002b8(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 kind;

    do {
        kind = Data_02005b5c[id - 18];
    } while (0);
    if (object->x >> 20 == S16_AT(object, 100) >> 4)
        Func_02005598(kind, 0, 0);
}

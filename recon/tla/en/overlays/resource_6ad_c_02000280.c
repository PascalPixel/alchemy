/* Near misses of overlay 6ad. Func_02000280: differ 2 of 26 (one reordered row): the native loads the
 * table's address before it doubles the id (ldr r3, =Data_02005b5c; lsls r5, r5, #1; subs r5, #36;
 * ldrsh r4, [r3, r5]); the draft doubles first. Func_020002b8: differ 6, the same pair and the
 * subtraction scheduled after the object's x load. The table must be a true array of s16
 * (data.c has it as u16): through a cast or a pointer local the 36 is folded into the literal or
 * added after the base. Tried: byte offsets, an offset local, a pointer local with and without a
 * "+l" barrier, a pointer to array. */
#include "../../../../games/tla/src/field/overlay_6ad/room.h"

extern s16 Data_02005b5c[];

void Func_02000280(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);

    Func_02005598(Data_02005b5c[id - 18], S16_AT(object, 100) << 16, (S16_AT(object, 102) << 16) - 0x80000);
}

void Func_020002b8(s32 unused, s32 id)
{
    struct ObjectRuntime *object = Object_GetById(id);
    s32 kind = Data_02005b5c[id - 18];

    if (object->x >> 20 == object->action >> 4)
        Func_02005598(kind, 0, 0);
}

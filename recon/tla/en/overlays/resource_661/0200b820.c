/* Near miss, differ 6 of 51 (26 before, 2026-10-12): the loop is written with goto, so 0xffff is built again for each test as the native does (loop.c moves it out of a do-while). Three rows are left in another order: both tests load the list entry between movs r2, #255 and its shift (this draft shifts first), and the native advances the entry (adds r7, #6) before the last call where this draft does it after; written before the call in the source it is still emitted after. Tried earlier: while, guarded do-while, a separate cursor, a 6-byte struct (the ABI pads it to 8). */
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_02003910(s32 id, u16 x, u16 z, s32 layer);

void Func_02003820(const u16 *list)
{
    const u16 *entry;

    if (list[0] == 0xffff)
        return;
    entry = list;
next:
    {
        s32 id = entry[0];
        struct ObjectRuntime *object = Object_GetById(id);
        s32 x = (s16)entry[1];
        s32 z = (s16)entry[2];

        object->collisionFlags &= ~1;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02003910(id, x, z, object->terrainId);
        entry += 3;
    }
    if (entry[0] != 0xffff)
        goto next;
}

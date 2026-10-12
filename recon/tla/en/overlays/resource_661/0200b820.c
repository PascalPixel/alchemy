/* Near miss, differ 26: the native rebuilds 0xffff for the loop test on every pass and
 * keeps only r8 above r7; this draft keeps 0xffff in r10 across the loop. The native also
 * tests the list head through r0 before copying the list to r7, and reads x before the
 * collision flags. Tried: while, guarded do-while, a separate cursor, a 6-byte struct
 * (the ABI pads it to 8). */
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
    do {
        s32 id = entry[0];
        struct ObjectRuntime *object = Object_GetById(id);
        s32 x = (s16)entry[1];
        s32 z = (s16)entry[2];

        object->collision_flags &= ~1;
        ObjectDispatch_SetSingleChildFlags(object, 0);
        Func_02003910(id, x, z, object->terrain_id);
        entry += 3;
    } while (entry[0] != 0xffff);
}

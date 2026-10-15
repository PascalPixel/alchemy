/* Draft of Func_02000b2c (overlay 680, 66 instructions), differ 28: places actor id
 * on the cell under (x, z), lifted by y, and marks that map cell (byte 2 = 255).
 * Written from the listing (the twin map's ☀️ SceneActor_PlaceAtTile is a false
 * match). Register allocation differs: the native keeps y in r8 and the cell x in
 * r7 and loads the cell table into r0; here y takes r7, the cell x r0 and the
 * table ip, so the native's r8 save is missing.
 * Measured: the cell store in a one-pass block (differ 30). alchemy permute,
 * 5 minutes: differ 16 at best, only with id as a short (sign-extended), which
 * the callers do not suggest. */
#include "types.h"
#include "object_runtime.h"
#include "ram_buffer.h"

struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

void Func_02000b2c(s32 id, s32 x, s32 y, s32 z)
{
    struct ObjectRuntime *actor = Object_GetById(id);
    u8 *cells = *(u8 **)((u8 *)Ram_HeapSlots->map_work + 424);
    s32 cellX = x / 0x100000;
    s32 cellZ = (z - y + 0x7fff) / 0x100000;

    cells[(cellZ * 128 + cellX) * 4 + 2] = 255;
    actor->x = (x & 0xfff00000) + 0x80000;
    actor->z = (z & 0xfff00000) + 0x80000;
    actor->y = actor->z - (cellZ << 20);
    actor->motion_flags = 0;
    actor->priority_flags |= 2;
    ObjectDispatch_SetSingleChildFlags(actor, 0);
}

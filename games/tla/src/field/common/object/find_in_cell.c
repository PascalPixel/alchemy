#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "eventwrk.h"

// Returns the first of objects 8 to 63 standing in the same 16-pixel cell
// as position (x, y and z), or NULL.
struct ObjectRuntime *Object_FindInCell(s32 *position)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    /* FAKEMATCH: the counter pinned to r4; without the pin global allocation gives r4 to the object pointer first (differ 12). */
    register u32 i asm("r4");
    void **objects;
    struct ObjectRuntime *object;

    i = 8;
    objects = &work->objects[8];
    /* FAKEMATCH: a goto loop and a volatile read of position[0]; as a for loop loop.c, and as a goto loop PRE, keep position[0] in a register across passes where the native reloads it (differ 32 and 22). */
next:
    object = *objects++;
    if (*(volatile s32 *)&position[0] >> 20 == object->x >> 20
        && position[1] >> 20 == object->y >> 20
        && position[2] >> 20 == object->z >> 20)
        return object;
    i++;
    if (i < 64)
        goto next;
    return NULL;
}

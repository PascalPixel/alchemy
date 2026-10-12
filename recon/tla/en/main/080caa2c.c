/* Near miss, differ 4 of 14: the native adds 20 to the scaled id and indexes
   the event work with it (adds r3, #20; str r2, [r6, r3]); this draft adds the
   work pointer and stores at offset 20. A byte offset and an index of id + 5
   compile the same. */
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"

void Object_Destroy(struct ObjectRuntime *object);

void Func_080caa2c(s32 id)
{
    u8 *event = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = ObjectTable_Get(id);

    if (object != 0) {
        Object_Destroy(object);
        ((s32 *)event)[id + 5] = 0;
    }
}

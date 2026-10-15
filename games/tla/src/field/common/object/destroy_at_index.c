#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"

void Object_Destroy(struct ObjectRuntime *object);

// Destroys the object in slot object_id of the event work's table and
// empties the slot.
void Func_080caa2c(u32 object_id)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        Object_Destroy(object);
        work->objects[object_id] = NULL;
    }
}

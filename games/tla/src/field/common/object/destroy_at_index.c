#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "object_runtime.h"

void Object_Destroy(struct ObjectRuntime *object);

// Destroys the object in slot objectId of the event work's table and
// empties the slot.
void Func_080caa2c(u32 objectId)
{
    struct EventWork *work = Ram_HeapSlots->eventWork;
    struct ObjectRuntime *object = ObjectTable_Get(objectId);

    if (object != NULL) {
        Object_Destroy(object);
        work->objects[objectId] = NULL;
    }
}

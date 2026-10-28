#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"

void Actor_Destroy(struct Actor *object);

/// Destroys the object in slot objectId of the event work's table and
/// empties the slot.
void Func_080caa2c(u32 objectId) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object = ObjectTable_Get(objectId);

  if (object != NULL) {
    Actor_Destroy(object);
    work->objects[objectId] = NULL;
  }
}

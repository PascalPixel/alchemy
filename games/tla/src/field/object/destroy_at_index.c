#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "actor.h"

void Actor_Destroy(struct Actor *object);

/// Releases the actor in slot actorIndex of the event work's table, if there
/// is one, and empties the slot.
void Actor_ReleaseAtIndex(u32 actorIndex) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *actor = ObjectTable_Get(actorIndex);

  if (actor != NULL) {
    Actor_Destroy(actor);
    work->objects[actorIndex] = NULL;
  }
}

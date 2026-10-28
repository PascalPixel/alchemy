#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_work.h"

struct Actor *Actor_GetById(s32 id);

s32 Func_02000064(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  work->screenEffect = 513;
  Actor_SetSpritePriority(8, 2);
  Actor_GetById(8)->collisionFlags |= 4;
  Actor_GetById(8)->collisionFlags |= 16;
  return 0;
}

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

struct Actor *Actor_GetById(s32 id);

s32 Func_02000590(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  *(s32 *)(event + 428) = 516;
  *(s32 *)(event + 436) = 24;
  if (gSequenceParentWork.entrance <= 89) {
    Actor_GetById(11)->scaleY = 0x1b333;
    Actor_GetById(11)->scaleX = 0x1b333;
  }
  return 0;
}

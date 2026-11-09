#include "types.h"
#include "ram_buffer.h"
#include "event_inline.h"

void Func_020047b8(void);

void Func_02004718(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 256;
  FieldTask_Remove(Func_020047b8);
}

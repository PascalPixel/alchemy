#include "types.h"
#include "system/iwram_call.h"
#include "system/ram_buffer.h"
#include "field/sprite.h"
#include "system/io_write_queue.h"
#include "system/task.h"
#include "field/event_inline.h"

extern struct Actor *gData_02006c08[];

void Func_020036a8(void);

/// While the object system runs, the world map's blend is queued for the
/// next frames.
void Func_020038e4(void) {
  if ((u16)Ram_HeapSlots->objectSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, 12, 0x20000);
  }
}

void Func_02003960(void) {
  Iwram_ClearWords(gData_02006c08, 0x200);
  Task_AddFrame(Func_020036a8, TASK_PASS_VBLANK);
}

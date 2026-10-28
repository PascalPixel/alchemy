#include "types.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "animspr.h"
#include "io_write_queue.h"

extern u8 gData_02006c08[];

s32 Task_Add(void (*callback)(void), s32 order);
void Func_020036a8(void);

/// While the object system runs, the world map's blend is queued for the
/// next frames.
void Func_020038e4(void) {
  if ((u16)Ram_HeapSlots->objectSystem->suspended == 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, 12, 0x20000);
  }
}

static inline void ClearWords(void *destination, s32 size) {
  /* FAKEMATCH: through an inline, as the ☀️ IWRAM call layer, the entry's
   * address loads between the size's two halves; called directly the size is
   * finished first (differ 2). */
  Iwram_ClearWordsEntry(destination, size);
}

void Func_02003960(void) {
  ClearWords(gData_02006c08, 0x200);
  Task_Add(Func_020036a8, 0x480);
}

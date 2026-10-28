#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "mem.h"
#include "task.h"

void Func_08108030(void *panel);

struct ScreenWork {
  u8 unk000[0x5a4];
  u8 panel[0x84];
};

void Func_08041204(void) {
  struct ScreenWork *work = Ram_HeapSlots->unkd0[0];

  Func_08108030(work->panel);
}

void Func_0804121c(void) {
  void *work = HeapSlot_AllocEwram(208, 0x628);

  Dma_Fill32(0, work, 0x628);
  Task_Add(Func_08041204, TASK_PASS_VBLANK);
}

void Func_08041254(void) {
  Task_Remove(Func_08041204);
  HeapSlot_Release(208);
}

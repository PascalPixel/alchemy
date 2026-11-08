#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "system/io_write_queue.h"
#include "system/irq_lock.h"

extern s32 gData_02002c5c;
extern u16 gData_02002c60[];
extern u16 gData_02002c80[];

struct PaletteCycleWork {
  u8 unk000[0x166];
  s16 unk166;
  u8 unk168[0xca4 - 0x168];
  s8 unkca4;
};

void Func_02000d14(void) {
  struct PaletteCycleWork *work =
      (struct PaletteCycleWork *)Ram_HeapSlots->eventWork;
  s32 *frame;
  s32 phase, i;

  if (work->unk166 != 0 || work->unkca4 != 0)
    return;
  frame = &gData_02002c5c;
  if (*frame == -1) {
    u16 *colors = gData_02002c60;
    s32 (*copy)(void *, const void *, s32) = Iwram_CopyWordsEntry;

    copy(colors, (void *)SPLIT_CONSTANT(0x5000060), 32);
    copy(gData_02002c80, colors, 32);
    *frame = 0;
  }
  phase = (*frame / 8) % 4;
  for (i = 0; i <= 3; i++)
    gData_02002c80[12 + (phase + i) % 4] = gData_02002c60[12 + i];
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;

    IRQ_LOCK(saved);
    count = q->count;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *entry++ = (u32)gData_02002c80;
      q->count = count + 1;
      *entry++ = 0x5000060;
      *entry = 0x84000008;
    }
    IRQ_UNLOCK(saved);
  }
  gData_02002c5c++;
}

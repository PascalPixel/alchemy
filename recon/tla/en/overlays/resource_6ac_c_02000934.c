// Near miss, differ 2 of 159 (tla-en, 2026-10-31): inside the written-out
// BLDALPHA push the native shifts the value's high half (lsls r3, r3, #8)
// before it adds the queue's base to the entry's offset (adds r2, r2, r4);
// here the two tie in sched2 (same priority, class and dependants) and RTL
// order puts the add first. Computing the entry after the count store
// (16), after the value (40), as &q->entries[0][0] + count * 3 or as a byte
// offset (2) does not move it; permute, 4 minutes: nothing better.
// Draft: rewritten from the listing. Sets room 1's blend for its entrance
// (none, or 15 and 16 with a step of (cell - 20) / 2), changes the cells the
// step selects, queues BLDALPHA and BLDCNT writes and schedules Func_0200055c.
// As in this room's adopted Func_020006c8 (s06c8.c), the cell calls go through
// Call6, a pointer taken after the first call loads the blend's address before
// the entrance tests, and empty asms with memory clobbers keep GCSE from
// carrying the step's load across the tests (which also keeps them word
// compares). The BLDALPHA push is IO_WRITE_QUEUE_PUSH written out so a
// scheduling barrier can sit between storing the value and building the
// register's address: the native finishes the value's store before it builds
// the address, which sched2 only does across a barrier (through the macro,
// differ 6; IO_WRITE_QUEUE_PUSH_IME 10, a reversed or precomputed value 6 and
// 28; one-pass blocks around the stores 8).
#include "types.h"
#include "io_write_queue.h"
#include "call.h"

extern s16 Data_02003a7c;
extern s16 gData_02003a7e;
extern s16 gData_02003a80;
extern s16 gData_02003a82;

void Func_0200055c(void);
void Func_02003158(void);
void Func_02003178(s32 a, s32 b, s32 c, s32 d, s32 x, s32 z);
s32 Task_Add(void (*func)(void), s32 priority);

void Func_02000934(s32 entrance) {
  s32 cell = -1;
  s16 *blend;

  Func_02003158();
  blend = &Data_02003a7c;
  if (entrance == 8)
    cell = 20;
  if (entrance == 10)
    cell = 22;
  if (entrance == 12)
    cell = 24;
  if (entrance == 14)
    cell = 26;
  if (cell == -1) {
    *blend = 0;
    gData_02003a7e = 0;
    gData_02003a80 = cell;
    gData_02003a82 = cell;
  } else {
    s32 step;

    *blend = 15;
    gData_02003a7e = 16;
    step = (cell - 20) / 2;
    gData_02003a80 = step;
    gData_02003a82 = cell;
    if (step == 0)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 67);
    asm("" : : : "memory");
    if (gData_02003a80 == 1)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 72);
    asm("" : : : "memory");
    if (gData_02003a80 == 2)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 17, 72);
    asm("" : : : "memory");
    if (gData_02003a80 == 3)
      Call6((void (*)())Func_02003178, 0, 125, 3, 3, 8, 67);
  }
  {
    struct IoWriteQueue *q = &gIoWriteQueue;
    u32 saved;
    s32 count;
    u16 *count_ = &q->count;

    IRQ_LOCK(saved);
    count = *count_;
    if (count <= 31) {
      u32 *entry = q->entries[count];

      *count_ = count + 1;
      *entry++ = Data_02003a7c | (16 - Data_02003a7c) << 8;
      /* FAKEMATCH(barrier): an empty asm with a memory clobber after the
       * value's store; as IO_WRITE_QUEUE_PUSH(0x04000052, ...), 6 of 159
       * instructions differ in tla-en. */
      asm("" : : : "memory");
      *entry++ = 0x04000052;
      *entry = 0x20000;
    }
    IRQ_UNLOCK(saved);
  }
  IO_WRITE_QUEUE_PUSH(0x04000050, 0x3f42, 0x20000);
  Task_Add(Func_0200055c, 0x480);
}

#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_write_queue.h"
#include "flag_call.h"
#include "frame.h"

struct WorldTint {
  struct MapWork map;
  u8 unk970[3];
  s8 region;
  s8 tint;
  s8 target;
};

void Func_0802d088(s32 region, s32 gradual) {
  struct WorldTint *work = (struct WorldTint *)Ram_HeapSlots->mapWork;

  if (work->region != region) {
    work->region = region;
    if (region == 9)
      work->target = 12;
    else if (region == 18 && !GameFlag_Test(0x8ef))
      work->target = -6;
    else
      work->target = 0;
  }
  if (GameFlag_Test(0x106))
    return;
  if (gradual == 0) {
    work->tint = work->target;
  } else {
    region = work->target - work->tint;
    if (region == 0)
      return;
    if (gFrameCount & 15)
      return;
    if (region < 0)
      region = -1;
    else
      region = 1;
    work->tint += region;
  }
  if (work->tint > 0) {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3f8e, 0x20000);
    {
      struct IoWriteQueue *q = &gIoWriteQueue;
      u32 saved;
      s32 count;

      IRQ_LOCK(saved);
      count = q->count;
      if (count <= 31) {
        u32 *entry;
        /* FAKEMATCH(temporary): the entry's byte offset held in offset and
         * scaled after the count store; as u32 *entry = q->entries[count]
         * before the store, 21 of 210 instructions differ in tla-en. */
        s32 offset = count * 3;

        q->count = count + 1;
        offset *= sizeof(u32);
        /* FAKEMATCH(barrier): an empty asm keeps the tint load after the
         * offset's scaling; as the same push with no asm (or as
         * IO_WRITE_QUEUE_PUSH(0x4000054, (u16)work->tint, 0x20000)), 2 of
         * 210 instructions differ in tla-en. */
        asm("");
        entry = (u32 *)((u8 *)q->entries + offset);
        *entry++ = (u16)work->tint;
        *entry++ = 0x4000054;
        *entry = 0x20000;
      }
      IRQ_UNLOCK(saved);
    }
  } else {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)-work->tint, 0x20000);
  }
}

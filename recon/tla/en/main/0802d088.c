// Near miss, differ 10 of 210 (2026-10-18): sets the world map's region (map
// work 0x973) and steps its screen tint (0x974) toward the region's target
// (0x975) one level every 16 frames, or at once when gradual is 0, queueing
// BLDCNT (brighten 0x3f8e or darken 0x3fce) and BLDY. Region 9 tints 12,
// region 18 tints -6 until flag 0x8ef; flag 0x106 holds the tint. Left: four
// instruction-order spots where the native issues a load (the map work, a
// tint byte, a queue entry's address) a step earlier. Six minutes of
// permute: differ 8 at best, by a block around one queue push.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "io_write_queue.h"
#include "flag_call.h"
#include "frame.h"

struct WorldTint {
  u8 unk000[0x973];
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
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)work->tint, 0x20000);
  } else {
    IO_WRITE_QUEUE_PUSH(0x4000050, 0x3fce, 0x20000);
    IO_WRITE_QUEUE_PUSH(0x4000054, (u16)-work->tint, 0x20000);
  }
}
